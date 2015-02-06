#pragma once

#include "models/details/modelsImplementation/abstractModelItem.h"

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class GraphicalModelItem : public modelsImplementation::AbstractModelItem
{
public:
	GraphicalModelItem(const Id &id, const Id &logicalId, GraphicalModelItem *parent);
	Id logicalId() const;

private:
	const Id mLogicalId;
};

}
}
}
}
