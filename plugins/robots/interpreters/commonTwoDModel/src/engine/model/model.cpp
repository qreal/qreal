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
	QDomNodeList const robotList = xml.elementsByTagName("robot");
	if (worldList.count() != 1 || robotList.count() != 1) {
		/// @todo Report error
		return;
	}

	mWorldModel.deserialize(worldList.at(0).toElement());

	for (RobotModel const &robot : mRobotModels) {
		robot.deserialize(robotList.at(0).toElement());
		robot.configuration().deserialize(robotList.at(0).toElement());
	}
}

void Model::addRobotModel(robotModel::TwoDRobotModel const &robotModel, QPointF const &pos)
{
	RobotModel *robot = new RobotModel(robotModel, mSettings, this);

	robot->setPosition(pos);

	connect(&mTimeline, &Timeline::started, robot, &RobotModel::reinit);
	connect(&mTimeline, &Timeline::stopped, robot, &RobotModel::stopRobot);

	connect(&mTimeline, &Timeline::tick, robot, &RobotModel::recalculateParams);
	connect(&mTimeline, &Timeline::nextFrame, robot, &RobotModel::nextFragment);

	auto resetPhysics = [this]() { robot->resetPhysics(mWorldModel); };
	connect(&mSettings, &Settings::physicsChanged, resetPhysics);
	resetPhysics();

	mRobotModels.append(*robot);

	emit robotAdded();
}

void Model::removeRobotModel(twoDModel::robotModel::TwoDRobotModel const &robotModel)
{
	int const i = findModel(robotModel);

	if (i == -1) {
		return;
	}

	RobotModel *robot = *mRobotModels.at(i);

	mRobotModels.removeOne(i);

	delete robot;

	emit robotDeleted();
}

void Model::replaceRobotModel(twoDModel::robotModel::TwoDRobotModel const &oldModel
		, twoDModel::robotModel::TwoDRobotModel const &newModel)
{
	int const i = findModel(oldModel);

	if (i == -1) {
		return;
	}

	QPointF const pos = mRobotModels.at(i).position();

	removeRobotModel(oldModel);
	addRobotModel(newModel, pos);
}

int Model::findModel(twoDModel::robotModel::TwoDRobotModel const &robotModel)
{
	for (RobotModel const &model : mRobotModels) {
		if (model.info().name() == robotModel.name()) {
			return mRobotModels.indexOf(model);
		}
	}
	return -1;
}
