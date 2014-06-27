#include "model.h"

#include <qrkernel/settingsManager.h>

#include "physics/simplePhysicsEngine.h"
#include "physics/realisticPhysicsEngine.h"

using namespace twoDModel;

Model::Model(interpreterBase::robotModel::RobotModelInterface &robotModel
		, QObject *parent)
	: QObject(parent)
	, mRobotModel(robotModel, this)
	, mPhysicsEngine(nullptr)
{
//	connect(mTimeline, SIGNAL(tick()), this, SLOT(recalculateParams()), Qt::UniqueConnection);
//	connect(mTimeline, SIGNAL(nextFrame()), this, SLOT(nextFragment()), Qt::UniqueConnection);
}

Model::~Model()
{
	delete mPhysicsEngine;
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
