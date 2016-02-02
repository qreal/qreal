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

#include "repoApiTest.h"

#include <QtCore/QPointF>

using namespace qrTest;
using namespace qrRepo;
using namespace qReal;

const Id logicalElement("editor", "diagram", "element", "id");
const Id graphicalElement("editor", "diagram", "element", "graphicalId");

void RepoApiTest::SetUp()
{
	mRepoApi = new RepoApi("test.qrs");

	mRepoApi->addChild(Id::rootId(), logicalElement);
	mRepoApi->addChild(Id::rootId(), graphicalElement, logicalElement);
}

void RepoApiTest::TearDown()
{
	delete mRepoApi;
}

TEST_F(RepoApiTest, graphicalPartPropertyTest)
{
	mRepoApi->createGraphicalPart(graphicalElement, 0);
	mRepoApi->setGraphicalPartProperty(graphicalElement, 0, "position", QPointF(10, 20));
	QPointF const position = mRepoApi->graphicalPartProperty(graphicalElement, 0, "position").toPointF();
	ASSERT_FLOAT_EQ(10.0, position.x());
	ASSERT_FLOAT_EQ(20.0, position.y());
}
