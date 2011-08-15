#pragma once

#include "../../qrgui/kernel/ids.h"
#include "../models/models.h"
#include "graphicaldifference.h"
#include "logicaldifference.h"
#include "elementtype.h"

namespace qReal
{
namespace diffManager
{
namespace details
{
class Difference
{
public:
	Difference(models::Models *oldModel, models::Models *newModel, qReal::Id const &graphicalId, qReal::Id const &logicalId, ElementType type);
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
}
