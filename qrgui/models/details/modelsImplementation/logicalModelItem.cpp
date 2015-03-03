#include "logicalModelItem.h"

#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace models::details::modelsImplementation;

LogicalModelItem::LogicalModelItem(const Id &id, LogicalModelItem *parent)
	: AbstractModelItem(id, parent)
{
}
