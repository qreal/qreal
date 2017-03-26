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

#include <plugins/tools/umlChecker/umlCheckerSupport/umlCheckerHandler.h>
#include <mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterMock.h>
#include <qrrepo/repoApi.h>

#include <gtest/gtest.h>

namespace qrTest {

class UmlCheckerHandlerTest : public testing::Test
{
protected:
	virtual void SetUp();

	virtual void TearDown();

	void openRepositories(const QString &perfect, const QString &ordinary);

	qReal::UmlCheckerHandler *mHandler;
	qrRepo::RepoApi *mOrdinaryRepoApi;


	QString mTestFilesPath = "/home/julia/qreal/qreal/qrtest/unitTests";
	QString mRepositoriesPath = mTestFilesPath + "/pluginsTests/toolsTests/umlCheckerTests/umlCheckerSupportTests/cases/";
	ErrorReporterMock mErrorReporter;
};

}
