#include "graphicalModelItem.h"

#include "../../qrkernel/exception/exception.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

GraphicalModelItem::GraphicalModelItem(Id const &id, Id const &logicalId, GraphicalModelItem *parent)
	: AbstractModelItem(id, parent), mLogicalId(logicalId)
{
}

Id GraphicalModelItem::logicalId() const
{
	return mLogicalId;
}
