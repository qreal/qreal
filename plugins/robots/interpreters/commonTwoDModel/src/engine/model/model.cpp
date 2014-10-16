#include "model.h"

#include <QDebug>

#include <qrkernel/settingsManager.h>

#include "include/commonTwoDModel/robotModel/nullTwoDRobotModel.h"

using namespace twoDModel::model;

Model::Model(QObject *parent)
	: QObject(parent)
{
}

WorldModel &Model::worldModel()
{
	return mWorldModel;
}

Timeline &Model::timeline()
{
	return mTimeline;
}

QList<RobotModel *> Model::robotModels()
{
	return mRobotModels;
}

Settings &Model::settings()
{
	return mSettings;
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

	return save;
}

void Model::deserialize(QDomDocument const &xml)
{
	QDomNodeList const worldList = xml.elementsByTagName("world");
	QDomNodeList const robotList = xml.elementsByTagName("robots");

	if (worldList.count() != 1 || robotList.count() != 1) {
		/// @todo Report error
		return;
	}

	mWorldModel.deserialize(worldList.at(0).toElement());

	QDomNodeList robots = xml.elementsByTagName("robots").at(0).toElement().elementsByTagName("robot");
	QMutableListIterator<RobotModel *> iterator(mRobotModels);
	QList<int> presentRobots;

	while(iterator.hasNext()) {
		bool exist = false;
		RobotModel *robotModel = iterator.next();

		for (int i = 0; i < robots.size(); i++) {
			if (robotModel->info()->robotId() == robots.at(i).toElement().attribute("id")) {
				robotModel->deserialize(robots.at(i).toElement());
				robotModel->configuration().deserialize(robots.at(i).toElement());
				exist = true;
				presentRobots.append(i);
				break;
			}

			if (!exist) {
				iterator.remove();
				emit robotRemoved(robotModel);
				delete robotModel;
			}
		}
	}

	for (int i = 0; i < robots.size(); i++) {
		if (presentRobots.indexOf(i) == -1) {
			twoDModel::robotModel::NullTwoDRobotModel *robotModel = new twoDModel::robotModel::NullTwoDRobotModel(
					robots.at(i).toElement().attribute("id"));
			addRobotModel(*robotModel);
			mRobotModels.last()->deserialize(robots.at(i).toElement());
			mRobotModels.last()->configuration().deserialize(robots.at(i).toElement());
		}
	}
}

void Model::addRobotModel(robotModel::TwoDRobotModel &robotModel, QPointF const &pos)
{
	RobotModel *robot = new RobotModel(robotModel, mSettings, this);
	robotModel.setParent(robot);
	robot->setPosition(pos);

	connect(&mTimeline, &Timeline::started, robot, &RobotModel::reinit);
	connect(&mTimeline, &Timeline::stopped, robot, &RobotModel::stopRobot);

	connect(&mTimeline, &Timeline::tick, robot, &RobotModel::recalculateParams);
	connect(&mTimeline, &Timeline::nextFrame, robot, &RobotModel::nextFragment);

	auto resetPhysics = [this, robot]() { robot->resetPhysics(mWorldModel); };
	connect(&mSettings, &Settings::physicsChanged, resetPhysics);
	resetPhysics();

	mRobotModels.append(robot);

	emit robotAdded(robot);
}

void Model::removeRobotModel(twoDModel::robotModel::TwoDRobotModel const &robotModel)
{
	int const i = findModel(robotModel);

	if (i == -1) {
		return;
	}

	RobotModel *robot = mRobotModels.at(i);

	mRobotModels.removeOne(mRobotModels.at(i));

	emit robotRemoved(robot);

	delete robot;
}

void Model::replaceRobotModel(twoDModel::robotModel::TwoDRobotModel const &oldModel
		, twoDModel::robotModel::TwoDRobotModel &newModel)
{
	int const i = findModel(oldModel);

	if (i == -1) {
		return;
	}

	QPointF const pos = mRobotModels.at(i)->position();

	removeRobotModel(oldModel);
	addRobotModel(newModel, pos);
}

int Model::findModel(twoDModel::robotModel::TwoDRobotModel const &robotModel)
{
	for (RobotModel *model : mRobotModels) {
		if (model->info()->robotId() == robotModel.robotId()) {
			return mRobotModels.indexOf(model);
		}
	}
	return -1;
}
