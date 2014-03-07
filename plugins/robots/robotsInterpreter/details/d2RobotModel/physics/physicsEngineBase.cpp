#include "physicsEngineBase.h"
#include "details/d2RobotModel/worldModel.h"

using namespace qReal::interpreters::robots::details::d2Model;
using namespace physics;

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
