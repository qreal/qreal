#include "model.h"

#include <qrkernel/settingsManager.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <interpreterBase/interpreterControlInterface.h>

#include "commonTwoDModel/robotModel/nullTwoDRobotModel.h"
#include "src/engine/constraints/constraintsChecker.h"

using namespace twoDModel::model;

Model::Model(QObject *parent)
	: QObject(parent)
	, mChecker(nullptr)
	, mErrorReporter(nullptr)
{
}

Model::~Model()
{
}

void Model::init(qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	mErrorReporter = &errorReporter;
	mChecker.reset(new constraints::ConstraintsChecker(errorReporter, *this));
	connect(mChecker.data(), &constraints::ConstraintsChecker::success, [&]() {
		errorReporter.addInformation(tr("The task is accomplished!"));
		interpreterControl.stopRobot();
	});
	connect(mChecker.data(), &constraints::ConstraintsChecker::fail, [&](const QString &message) {
		errorReporter.addError(message);
		interpreterControl.stopRobot();
	});
	connect(mChecker.data(), &constraints::ConstraintsChecker::checkerError, [&errorReporter](const QString &message) {
		errorReporter.addCritical(tr("Error in checker: %1").arg(message));
	});
}

WorldModel &Model::worldModel()
{
	return mWorldModel;
}

Timeline &Model::timeline()
{
	return mTimeline;
}

QList<RobotModel *> Model::robotModels() const
{
	return mRobotModels;
}

Settings &Model::settings()
{
	return mSettings;
}

qReal::ErrorReporterInterface *Model::errorReporter()
{
	return mErrorReporter;
}

QDomDocument Model::serialize() const
{
	QDomDocument save;
	QDomElement root = save.createElement("root");
	save.appendChild(root);
	root.appendChild(mWorldModel.serialize(save, QPoint(0, 0)));

	QDomElement robots = save.createElement("robots");
	for (RobotModel *robotModel : mRobotModels) {
		QDomElement robot = robotModel->serialize(save);
		robots.appendChild(robot);
	}

	root.appendChild(robots);
	mChecker->serializeConstraints(root);

	return save;
}

void Model::deserialize(const QDomDocument &xml)
{
	const QDomNodeList worldList = xml.elementsByTagName("world");
	const QDomNodeList robotsList = xml.elementsByTagName("robots");
	const QDomElement constraints = xml.documentElement().firstChildElement("constraints");

	if (worldList.count() != 1) {
		/// @todo Report error
		return;
	}

	mWorldModel.deserialize(worldList.at(0).toElement());

	if (robotsList.count() != 1) {
		// need for backward compatibility with old format
		const QDomNodeList robotList = xml.elementsByTagName("robot");

		if (robotList.count() != 1) {
			/// @todo Report error
			return;
		}

		mRobotModels.at(0)->deserialize(robotList.at(0).toElement());
		mRobotModels.at(0)->configuration().deserialize(robotList.at(0).toElement());

		return;
	}

	QMutableListIterator<RobotModel *> iterator(mRobotModels);

	const bool oneRobot = robotsList.at(0).toElement().elementsByTagName("robot").size() == 1
			&& mRobotModels.size() == 1;

	while(iterator.hasNext()) {
		bool exist = false;
		RobotModel *robotModel = iterator.next();

		for (QDomElement element = robotsList.at(0).firstChildElement("robot"); !element.isNull();
				element = element.nextSiblingElement("robot")) {
			if (robotModel->info().robotId() == element.toElement().attribute("id")) {
				robotModel->deserialize(element);
				robotModel->configuration().deserialize(element);
				exist = true;
				robotsList.at(0).removeChild(static_cast<QDomNode>(element));
				break;
			}
		}

		if (!exist && !oneRobot) {
			iterator.remove();
			emit robotRemoved(robotModel);
			delete robotModel;
		}
	}

	if (oneRobot && !robotsList.at(0).firstChildElement("robot").isNull()) {
		QDomElement element = robotsList.at(0).firstChildElement("robot");
		mRobotModels.at(0)->deserialize(element);
	} else {
		for (QDomElement element = robotsList.at(0).firstChildElement("robot"); !element.isNull();
				element = element.nextSiblingElement("robot")) {
			twoDModel::robotModel::NullTwoDRobotModel *robotModel = new twoDModel::robotModel::NullTwoDRobotModel(
					element.attribute("id"));
			addRobotModel(*robotModel);
			mRobotModels.last()->deserialize(element);
		}
	}

	if (mChecker) {
		/// @todo: should we handle if it returned false?
		mChecker->parseConstraints(constraints);
	}
}

void Model::addRobotModel(robotModel::TwoDRobotModel &robotModel, const QPointF &pos)
{
	RobotModel *robot = new RobotModel(robotModel, mSettings, this);
	robot->setPosition(pos);

	connect(&mTimeline, &Timeline::started, robot, &RobotModel::reinit);
	connect(&mTimeline, &Timeline::stopped, robot, &RobotModel::stopRobot);

	connect(&mTimeline, &Timeline::tick, robot, &RobotModel::recalculateParams);
	connect(&mTimeline, &Timeline::nextFrame, robot, &RobotModel::nextFragment);

	auto resetPhysics = [this, robot]() { robot->resetPhysics(mWorldModel, mTimeline); };
	connect(&mSettings, &Settings::physicsChanged, resetPhysics);
	resetPhysics();

	mRobotModels.append(robot);

	emit robotAdded(robot);
}

void Model::removeRobotModel(const twoDModel::robotModel::TwoDRobotModel &robotModel)
{
	const int index = findModel(robotModel);

	if (index == -1) {
		return;
	}

	RobotModel *robot = mRobotModels.at(index);
	mRobotModels.removeOne(mRobotModels.at(index));
	emit robotRemoved(robot);
	delete robot;
}

void Model::replaceRobotModel(const twoDModel::robotModel::TwoDRobotModel &oldModel
		, twoDModel::robotModel::TwoDRobotModel &newModel)
{
	const int index = findModel(oldModel);

	if (index == -1) {
		return;
	}

	const QPointF pos = mRobotModels.at(index)->position();
	removeRobotModel(oldModel);
	addRobotModel(newModel, pos);
}

int Model::findModel(const twoDModel::robotModel::TwoDRobotModel &robotModel)
{
	for (int i = 0; i < mRobotModels.count(); i++) {
		if (mRobotModels.at(i)->info().robotId() == robotModel.robotId()) {
			return i;
		}
	}

	return -1;
}
