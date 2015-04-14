/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <models/details/modelsImplementation/modelIndexesInterface.h>
#include <gmock/gmock.h>

namespace qrTest {

class ModelIndexesInterfaceMock : public qReal::models::details::modelsImplementation::ModelIndexesInterface {
public:
	MOCK_CONST_METHOD3(index, QModelIndex(int row, int column, const QModelIndex &parent));
};

}
