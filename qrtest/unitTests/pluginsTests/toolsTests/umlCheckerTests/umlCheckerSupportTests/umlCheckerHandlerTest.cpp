/* Copyright 2007-2017 QReal Research Group
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

#include "umlCheckerHandlerTest.h"

#include "gtest/gtest.h"

using namespace qReal;
using namespace qrTest;

using ::testing::Exactly;
using ::testing::_;

void UmlCheckerHandlerTest::SetUp() {

	mOrdinaryRepoApi = new qrRepo::RepoApi(mRepositoriesPath + "/perfect", true);

	mHandler = new UmlCheckerHandler();
}

void UmlCheckerHandlerTest::TearDown() {
	delete mHandler;
}


void UmlCheckerHandlerTest::openRepositories(const QString &perfect, const QString &ordinary)
{
	mOrdinaryRepoApi->open(mRepositoriesPath + ordinary);
}

TEST_F(UmlCheckerHandlerTest, oneClass) {
	openRepositories("perfectOneClass.qrs", "ordinaryOneClass.qrs");

	bool matchResult = mHandler->matchingResult();

	ASSERT_EQ(matchResult, true);
}

TEST_F(UmlCheckerHandlerTest, twoClassesWithEdge) {
	openRepositories("perfectTwoClassesWithEdge.qrs", "ordinaryTwoClassesWithEdge.qrs");

	bool matchResult = mHandler->matchingResult();

	ASSERT_EQ(matchResult, true);
}

TEST_F(UmlCheckerHandlerTest, threeClassesWithThreeEdges) {
	openRepositories("perfect3ClassesWith3Edges.qrs", "ordinary3ClassesWith3Edges.qrs");

	bool matchResult = mHandler->matchingResult();

	ASSERT_EQ(matchResult, true);
}
