#include "difference.h"

using namespace qReal;
using namespace qReal::diffManager::details;

//repo <-> old object
//working copy <-> new object

Difference::Difference(models::Models *oldModel, models::Models *newModel, Id const &graphicalId, Id const &logicalId, ElementType type)
	: mGraphicalId(graphicalId), mLogicalId(logicalId), mType(type)
{
	switch(type)
	{
	case GraphicalAndLogical:
		mGraphicalDifference = new GraphicalDifference(oldModel, newModel, graphicalId);
		mLogicalDifference = new LogicalDifference(oldModel, newModel, logicalId);
		break;
	case PurelyGraphical:
		mGraphicalDifference = new GraphicalDifference(oldModel, newModel, graphicalId);
		break;
	case PurelyLogical:
		mLogicalDifference = new LogicalDifference(oldModel, newModel, logicalId);
		break;
	default:
		qDebug() << "Unknown Element Type";
		qDebug() << type;
		break;
	}
}

ElementType Difference::elementType() const
{
	return mType;
}

qReal::Id Difference::graphicalId() const
{
	return mGraphicalId;
}

qReal::Id Difference::logicalId() const
{
	return mLogicalId;
}

GraphicalDifference *Difference::graphicalDifference() const
{
	return mGraphicalDifference;
}

LogicalDifference *Difference::logicalDifference() const
{
	return mLogicalDifference;
}
