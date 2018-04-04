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

#include <gmock/gmock.h>

#include "semanticTreeTest.h"

using namespace qrTest::robotsTests::SemanticTreeTests;
using namespace qrTest;
using namespace ::testing;


void qrTest::robotsTests::SemanticTreeTests::SemanticTreeTest::SetUp()
{
	const QString empty = "";
	const QStringList pathsToTemplates = {};
	const QString workingFile = "diagrams/oneTest.qrs";

	mRepo.reset(new qrRepo::RepoApi(workingFile));
	qDebug() << mRepo->workingFile();
	mToolbox.reset(new qrtext::lua::LuaToolbox());

	ON_CALL(mModelManager, model()).WillByDefault(ReturnRef(mModel));

	mEditor.reset(new qReal::EditorManager(empty));


	mParserErrorReporter.reset(new utils::ParserErrorReporter(*mToolbox
			, mErrorReporterMock
			, *mEditor
	));

	mProcessor.reset(new generatorBase::lua::LuaProcessor(mErrorReporterMock
			, *mToolbox
			, *mParserErrorReporter));

	mCustomizer.reset(new trik::TrikGeneratorCustomizer(*mRepo
			, mErrorReporterMock
			, mModelManager
			, *mProcessor
			, pathsToTemplates));

	mCustomizer->factory()->initialize();

	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{47bae389-f76d-4510-999b-c8160d1dfc33}";
	const qReal::Id diagramId = qReal::Id::loadFromString(mainIdName);

	mPrimaryControlFlowValidator.reset(new generatorBase::PrimaryControlFlowValidator(*mRepo
			, mErrorReporterMock
			, *mCustomizer
	));

	mReadableControlFlowGenerator.reset(new generatorBase::ReadableControlFlowGenerator(
			*mRepo
			, mErrorReporterMock
			, *mCustomizer
			, *mPrimaryControlFlowValidator.data()
			, diagramId
	));
}

TEST_F(SemanticTreeTest, dummyTest) {

	EXPECT_TRUE(true);
}


TEST_F(SemanticTreeTest, smallDiagramTest) {
	generatorBase::semantics::SemanticTree *tree = mReadableControlFlowGenerator->generate();
	const QString scheme = tree->treeScheme();
	qDebug() << scheme;

	EXPECT_TRUE(scheme == "root@simple;simple;final");
	delete tree;
}

