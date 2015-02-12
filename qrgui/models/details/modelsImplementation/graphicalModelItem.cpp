#include "graphicalModelItem.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace models::details::modelsImplementation;

GraphicalModelItem::GraphicalModelItem(const Id &id, const Id &logicalId, GraphicalModelItem *parent)
	: AbstractModelItem(id, parent), mLogicalId(logicalId)
{
}

Id GraphicalModelItem::logicalId() const
{
	return mLogicalId;
}
