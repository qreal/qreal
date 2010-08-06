#include "graphicalModelItem.h"

#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace models;
using namespace details;

GraphicalModelItem::GraphicalModelItem(Id const &id, Id const &logicalId, GraphicalModelItem *parent)
	: AbstractModelItem(id, parent), mLogicalId(logicalId)
{
}

Id GraphicalModelItem::logicalId() const
{
	return mLogicalId;
}
