/* Copyright 2018 QReal Research Group
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

#include <QtCore/QFuture>

#include "plugins/robots/generators/generatorBase/include/generatorBase/generatorCustomizer.h"
//#include "plugins/robots/generators/generatorBase/include/generatorBase/generatorFactoryBase.h"
//#include <plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelManagerInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class GeneratorCustomizerMock : public QObject, public generatorBase::GeneratorCustomizer
{
	Q_OBJECT

public:
	MOCK_METHOD1(isInitialNode, bool(const qReal::Id &block));
	MOCK_METHOD1(isFinalNode, bool(const qReal::Id &block));
	MOCK_METHOD1(isSubprogramCall, bool(const qReal::Id &block));
	MOCK_METHOD1(isConditional, bool(const qReal::Id &block));
	MOCK_METHOD1(isLoop, bool(const qReal::Id &block));
	MOCK_METHOD1(isSwitch, bool(const qReal::Id &block));
	MOCK_METHOD1(isFork, bool(const qReal::Id &block));
	MOCK_METHOD1(isJoin, bool(const qReal::Id &block));

	MOCK_METHOD0(factory, generatorBase::GeneratorFactoryBase *());
};

}


