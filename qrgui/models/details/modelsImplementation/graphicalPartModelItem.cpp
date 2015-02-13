#include "graphicalPartModelItem.h"

using namespace qReal;
using namespace qReal::models::details::modelsImplementation;

GraphicalPartModelItem::GraphicalPartModelItem(const Id &id, int index)
	:mId(id), mIndex(index)
{
}

Id GraphicalPartModelItem::id() const
{
	return mId;
}

int GraphicalPartModelItem::index() const
{
	return mIndex;
}
