#pragma once

#include <QtCore/QModelIndex>

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class ModelIndexesInterface
{
public:
	virtual ~ModelIndexesInterface() {}
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const = 0;
};

}
}
}
}
