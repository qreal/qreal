#include "model.h"

#include <qrkernel/settingsManager.h>

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

QList<RobotModel> Model::robotModel()
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

	for (RobotModel const &robot : mRobotModels) {
		robot.serialize(save);
	}

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

	QMutableListIterator<RobotModel *> iterator(mRobotModels);
	QList<int> presentRobots;

	while(iterator.hasNext()) {
		bool exist = false;
		RobotModel *robot = iterator.next();

		for (int i = 0; i < robotList.size(); i++) {
			if (robot->robotId() == robotList.at(i).toElement().attribute("id")) {
				robot->deserialize(robotList.at(i).toElement());
				robot->configuration().deserialize(robotList.at(i).toElement());
				exist = true;
				presentRobots.append(i);
				break;
			}

			if (!exist) {
				iterator.remove();
				emit robotRemoved();
				delete robot;
			}
		}
	}

	for (int i = 0; i < robotList.size(); i++) {
		if (presentRobots.indexOf(i) == -1) {
			addRobotModel(nullptr, robotList.at(i).toElement().attribute("id"));
			mRobotModels.last()->deserialize(robotList.at(i).toElement());
			mRobotModels.last()->configuration().deserialize(robotList.at(i).toElement());
		}
	}
}

void Model::addRobotModel(robotModel::TwoDRobotModel *robotModel, QPointF const &pos
		, QString const &robotId)
{
	RobotModel *robot = robotModel ?  new RobotModel(robotModel, mSettings, this)
			: new RobotModel(robotId, mSettings, this);

	robot->setPosition(pos);

	connect(&mTimeline, &Timeline::started, robot, &RobotModel::reinit);
	connect(&mTimeline, &Timeline::stopped, robot, &RobotModel::stopRobot);

	connect(&mTimeline, &Timeline::tick, robot, &RobotModel::recalculateParams);
	connect(&mTimeline, &Timeline::nextFrame, robot, &RobotModel::nextFragment);

	auto resetPhysics = [this]() { robot->resetPhysics(mWorldModel); };
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

	mRobotModels.removeOne(i);

	emit robotRemoved(robot);

	delete robot;
}

void Model::replaceRobotModel(twoDModel::robotModel::TwoDRobotModel const &oldModel
		, twoDModel::robotModel::TwoDRobotModel const &newModel)
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
		if (model->robotId() == robotModel.robotId()) {
			return mRobotModels.indexOf(model);
		}
	}
	return -1;
}
