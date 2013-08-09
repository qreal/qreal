#include "graphicalPartModelTest.h"

#include <../qrrepo/repoApi.h>

using namespace qrguiTests;
using namespace qReal;
using namespace qReal::models::details;

Id const element("editor", "diagram", "element", "id");
Id const graphicalElement("editor", "diagram", "element", "graphicalId");

void GraphicalPartModelTest::SetUp()
{
	mRepoApi = new qrRepo::RepoApi("test.qrs");
	mGraphicalPartModel = new GraphicalPartModel(*mRepoApi);
	mRepoApi->addChild(Id::rootId(), element);
	mRepoApi->addChild(Id::rootId(), graphicalElement, element);
}

void GraphicalPartModelTest::TearDown()
{
	delete mRepoApi;
	delete mGraphicalPartModel;
}

TEST_F(GraphicalPartModelTest, rowCountTest)
{
	ASSERT_EQ(0, mGraphicalPartModel->rowCount());

	mGraphicalPartModel->addGraphicalPart(graphicalElement, 0);

	ASSERT_EQ(1, mGraphicalPartModel->rowCount());
}

TEST_F(GraphicalPartModelTest, columnCountTest)
{
	ASSERT_EQ(1, mGraphicalPartModel->columnCount());
}

TEST_F(GraphicalPartModelTest, indexParentTest)
{
	mGraphicalPartModel->addGraphicalPart(graphicalElement, 0);

	ASSERT_FALSE(mGraphicalPartModel->index(1, 0).isValid());
	ASSERT_TRUE(mGraphicalPartModel->index(0, 0).isValid());

	QModelIndex const elementIndex = mGraphicalPartModel->index(0, 0);
	ASSERT_EQ(NULL, elementIndex.internalPointer());

	QModelIndex const partIndex = mGraphicalPartModel->index(0, 0, elementIndex);

	ASSERT_TRUE(partIndex.isValid());
	ASSERT_FALSE(mGraphicalPartModel->index(0, 1, elementIndex).isValid());
	ASSERT_FALSE(mGraphicalPartModel->index(1, 0, elementIndex).isValid());

	ASSERT_NE(static_cast<void *>(NULL), partIndex.internalPointer());

	QModelIndex const wrongIndex = mGraphicalPartModel->index(0, 0, partIndex);
	ASSERT_FALSE(wrongIndex.isValid());

	mGraphicalPartModel->addGraphicalPart(graphicalElement, 5);
	ASSERT_FALSE(mGraphicalPartModel->index(0, 1, elementIndex).isValid());
	ASSERT_TRUE(mGraphicalPartModel->index(1, 0, elementIndex).isValid());
	ASSERT_NE(static_cast<void *>(NULL), mGraphicalPartModel->index(1, 0, elementIndex).internalPointer());

	QModelIndex const newWrongIndex = mGraphicalPartModel->index(0, 0, partIndex);
	ASSERT_FALSE(newWrongIndex.isValid());
}

TEST_F(GraphicalPartModelTest, addGraphicalPartFindIndexTest)
{
	mGraphicalPartModel->addGraphicalPart(graphicalElement, 0);
	QModelIndex const index = mGraphicalPartModel->findIndex(graphicalElement, 0);
	ASSERT_TRUE(index.isValid());
}

TEST_F(GraphicalPartModelTest, dataSetDataTest)
{
	QModelIndex const index = mGraphicalPartModel->addGraphicalPart(graphicalElement, 0);
	ASSERT_TRUE(mGraphicalPartModel->setData(index, QPointF(10, 20), GraphicalPartModel::positionRole));

	ASSERT_EQ(QPointF(10, 20), mGraphicalPartModel->data(index, GraphicalPartModel::positionRole).toPointF());

	// Just in case
	QModelIndex const newIndex = mGraphicalPartModel->findIndex(graphicalElement, 0);
	ASSERT_EQ(QPointF(10, 20), mGraphicalPartModel->data(newIndex, GraphicalPartModel::positionRole).toPointF());
}
