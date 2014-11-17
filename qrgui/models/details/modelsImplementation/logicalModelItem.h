#pragma once

#include "qrgui/models/details/modelsImplementation/abstractModelItem.h"

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {


class LogicalModelItem : public AbstractModelItem
{
public:
	LogicalModelItem(Id const &id, LogicalModelItem *parent);
};

}
}
}
}
