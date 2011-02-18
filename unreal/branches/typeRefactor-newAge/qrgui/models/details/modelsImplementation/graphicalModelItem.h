#pragma once

#include "abstractModelItem.h"

namespace qReal {

namespace models {

namespace details {

namespace modelsImplementation {

class GraphicalModelItem : public modelsImplementation::AbstractModelItem {
public:
	GraphicalModelItem(Id const &id, Id const &logicalId, NewType const &type, GraphicalModelItem *parent);
	Id logicalId() const;
private:
	Id const mLogicalId;
	NewType const mType;
};

}

}

}

}
