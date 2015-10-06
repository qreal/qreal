/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "twoDModel/engine/model/model.h"

#include <qrkernel/settingsManager.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <kitBase/interpreterControlInterface.h>

#include "src/engine/constraints/constraintsChecker.h"
#include "src/robotModel/nullTwoDRobotModel.h"

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
		, kitBase::InterpreterControlInterface &interpreterControl)
{
	mErrorReporter = &errorReporter;
	mChecker.reset(new constraints::ConstraintsChecker(errorReporter, *this));
	connect(mChecker.data(), &constraints::ConstraintsChecker::success, [&]() {
		errorReporter.addInformation(tr("The task is accomplished!"));
		// Stopping cannot be performed immediately because we still have constraints to check in event loop
		// and they need scene to be alive (in checker stopping interpretation means deleting all).
		QTimer::singleShot(0, &interpreterControl, SLOT(stopRobot()));
	});
	connect(mChecker.data(), &constraints::ConstraintsChecker::fail, [&](const QString &message) {
		errorReporter.addError(message);
		// Stopping cannot be performed immediately because we still have constraints to check in event loop
		// and they need scene to be alive (in checker stopping interpretation means deleting all).
		QTimer::singleShot(0, &interpreterControl, SLOT(stopRobot()));
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
	root.appendChild(mWorldModel.serialize(save));

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

	if (mChecker) {
		/// @todo: should we handle if it returned false?
		mChecker->parseConstraints(constraints);
	}

	if (worldList.count() != 1) {
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

bool Model::hasConstraints() const
{
	return mChecker->hasConstraints();
}

void Model::setConstraintsEnabled(bool enabled)
{
	mChecker->setEnabled(enabled);
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
