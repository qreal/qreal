#include "graphicalModelItem.h"

#include "../../kernel/exception/exception.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

GraphicalModelItem::GraphicalModelItem(Id const &id, Id const &logicalId, NewType const &type, GraphicalModelItem *parent)
	: AbstractModelItem(id, parent), mLogicalId(logicalId), mType(type)
{
}

Id GraphicalModelItem::logicalId() const
{
	return mLogicalId;
}
