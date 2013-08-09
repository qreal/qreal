#include "repoApiTest.h"

#include <QtCore/QPointF>

using namespace qrTest;
using namespace qrRepo;
using namespace qReal;

Id const logicalElement("editor", "diagram", "element", "id");
Id const graphicalElement("editor", "diagram", "element", "graphicalId");

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
