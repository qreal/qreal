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

#include <interpreterCore/managers/blocksFactoryManagerInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class BlocksFactoryManagerInterfaceMock : public interpreterCore::BlocksFactoryManagerInterface
{
public:
	MOCK_METHOD2(addFactory, void(kitBase::blocksBase::BlocksFactoryInterface * const factory
			, kitBase::robotModel::RobotModelInterface const *robotModel));
	MOCK_METHOD2(block, qReal::interpretation::BlockInterface *(qReal::Id const &element
			, kitBase::robotModel::RobotModelInterface const &robotModel));
	MOCK_CONST_METHOD1(enabledBlocks, QSet<qReal::Id>(
			kitBase::robotModel::RobotModelInterface const &robotModel));
	MOCK_CONST_METHOD1(visibleBlocks, QSet<qReal::Id>(
			kitBase::robotModel::RobotModelInterface const &robotModel));
	MOCK_CONST_METHOD0(commonBlocks, QSet<qReal::Id>());
};

}
