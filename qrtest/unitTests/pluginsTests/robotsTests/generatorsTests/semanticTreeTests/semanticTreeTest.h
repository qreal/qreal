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

#include <gtest/gtest.h>

#include <QtCore/QScopedPointer>

#include <testUtils/qrguiFacade.h>
#include <qrtest/unitTests/mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterMock.h>

#include <qrtest/unitTests/mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/generatorCustomizerMock.h>
//#include "../trik/trikGeneratorBase/src/trikGeneratorCustomizer.h"
//#include <generatorBase/generatorCustomizer.h>
#include <generatorBase/primaryControlFlowValidator.h>
#include "src/readableControlFlowGenerator.h"
#include <qrrepo/repoApi.h>

namespace generatorBase {
class ReadableControlFlowGenerator;
}

namespace qrTest {

class QrguiFacade;
class TestRegistry;
class TestRobotModel;

namespace robotsTests {
namespace SemanticTreeTests {

/// Test case for Semantic Trees.
class SemanticTreeTest : public testing::Test
{
protected:
	void SetUp() override;

public:
	QScopedPointer<generatorBase::ReadableControlFlowGenerator> mReadableControlFlowGenerator;

	QScopedPointer<qrRepo::RepoApi> mRepoApi;
	ErrorReporterMock mErrorReporterMock;
	GeneratorCustomizerMock mGeneratorCustomizerMock;
	QScopedPointer<generatorBase::PrimaryControlFlowValidator> mPrimaryControlFlowValidator;
};

}
}
}
