#include "logicalModelItem.h"

#include "../../kernel/exception/exception.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

LogicalModelItem::LogicalModelItem(Id const &id, LogicalModelItem *parent)
	: AbstractModelItem(id, parent)
{
}
