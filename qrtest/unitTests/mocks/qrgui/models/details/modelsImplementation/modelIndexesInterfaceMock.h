#pragma once

#include <models/details/modelsImplementation/modelIndexesInterface.h>
#include <gmock/gmock.h>

namespace qrTest {

class ModelIndexesInterfaceMock : public qReal::models::details::modelsImplementation::ModelIndexesInterface {
public:
	MOCK_CONST_METHOD3(index, QModelIndex(int row, int column, const QModelIndex &parent));
};

}
