#include "physicsEngineBase.h"

#include "src/engine/worldModel.h"

using namespace twoDModel;
using namespace twoDModel::physics;

PhysicsEngineBase::PhysicsEngineBase(WorldModel const &worldModel)
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
