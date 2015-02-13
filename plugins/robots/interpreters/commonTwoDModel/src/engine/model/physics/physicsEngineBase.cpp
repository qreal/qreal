#include "physicsEngineBase.h"

#include "src/engine/model/worldModel.h"

using namespace twoDModel::model::physics;

PhysicsEngineBase::PhysicsEngineBase(const WorldModel &worldModel)
	: mWorldModel(worldModel)
{
}

PhysicsEngineBase::~PhysicsEngineBase()
{
}

QVector2D PhysicsEngineBase::shift() const
{
	return mPositionShift;
}

qreal PhysicsEngineBase::rotation() const
{
	return mRotation;
}
