#include "logicalModelItem.h"

#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace models;
using namespace details;

LogicalModelItem::LogicalModelItem(Id const &id, LogicalModelItem *parent)
	: AbstractModelItem(id, parent)
{
}
