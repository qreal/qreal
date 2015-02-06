#pragma once

#include "models/details/modelsImplementation/abstractModelItem.h"

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {


class LogicalModelItem : public AbstractModelItem
{
public:
	LogicalModelItem(const Id &id, LogicalModelItem *parent);
};

}
}
}
}
