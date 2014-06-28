#include "model.h"

#include <qrkernel/settingsManager.h>

using namespace twoDModel::model;

Model::Model(interpreterBase::robotModel::RobotModelInterface &robotModel
		, QObject *parent)
	: QObject(parent)
	, mRobotModel(robotModel, mSettings, this)
{
	connect(&mTimeline, &Timeline::started, &mRobotModel, &RobotModel::reinit);
	connect(&mTimeline, &Timeline::stopped, &mRobotModel, &RobotModel::stopRobot);

	connect(&mTimeline, &Timeline::tick, &mRobotModel, &RobotModel::recalculateParams);
	connect(&mTimeline, &Timeline::nextFrame, &mRobotModel, &RobotModel::nextFragment);

	connect(&mSettings, &Settings::physicsChanged, [this](bool isRealistic) {
		mRobotModel.resetPhysics(isRealistic, mWorldModel);
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

RobotModel &Model::robotModel()
{
	return mRobotModel;
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
	mRobotModel.serialize(save);
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
	mRobotModel.deserialize(robotList.at(0).toElement());
	mRobotModel.configuration().deserialize(robotList.at(0).toElement());
}
