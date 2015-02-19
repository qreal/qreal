#pragma once

#include "graphicalDifference.h"
#include "logicalDifference.h"
#include "elementType.h"

namespace versioning
{
namespace details
{

class Difference
{
public:
	Difference(qReal::models::Models *oldModel
			, qReal::models::Models *newModel, qReal::Id const &graphicalId
			, qReal::Id const &logicalId, ElementType type);

	qReal::Id graphicalId() const;
	qReal::Id logicalId() const;
	ElementType elementType() const;
	GraphicalDifference *graphicalDifference() const;
	LogicalDifference *logicalDifference() const;

private:
	qReal::Id mGraphicalId;
	qReal::Id mLogicalId;
	ElementType mType;
	GraphicalDifference *mGraphicalDifference;
	LogicalDifference *mLogicalDifference;
};

}
}
