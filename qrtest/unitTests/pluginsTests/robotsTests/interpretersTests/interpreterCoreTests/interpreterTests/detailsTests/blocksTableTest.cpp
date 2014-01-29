#include "blocksTableTest.h"

#include "src/interpreter/details/blocksTable.h"

#include "support/dummyBlocksFactory.h"

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace qrTest::robotsTests::interpreterCoreTests::detailsTests;
using namespace interpreterCore::interpreter::details;

using namespace ::testing;

TEST_F(BlocksTableTest, blocksCreation)
{
	DummyBlockFactory *factory = new DummyBlockFactory();
	BlocksTable table(factory);
	interpreterBase::blocksBase::BlockInterface *block1 = table.block(qReal::Id("a", "b", "c", "d"));
	interpreterBase::blocksBase::BlockInterface *block2 = table.block(qReal::Id("a", "b", "c", "d"));
	interpreterBase::blocksBase::BlockInterface *block3 = table.block(qReal::Id("a", "b", "c", "e"));
	ASSERT_NE(nullptr, block1);
	ASSERT_EQ(block1, block2);
	ASSERT_NE(nullptr, block3);
	ASSERT_NE(block1, block3);
}

TEST_F(BlocksTableTest, clear)
{
	DummyBlockFactory *factory = new DummyBlockFactory();
	BlocksTable table(factory);
	interpreterBase::blocksBase::BlockInterface *block1 = table.block(qReal::Id("a", "b", "c", "d"));
	interpreterBase::blocksBase::BlockInterface *block2 = table.block(qReal::Id("a", "b", "c", "d"));
	interpreterBase::blocksBase::BlockInterface *block3 = table.block(qReal::Id("a", "b", "c", "e"));
	ASSERT_NE(nullptr, block1);
	ASSERT_EQ(block1, block2);
	ASSERT_NE(nullptr, block3);
	ASSERT_NE(block1, block3);
	block1->setObjectName("block1");
	block3->setObjectName("block3");
	ASSERT_EQ(QString("block1"), block1->objectName());
	ASSERT_EQ(QString("block3"), block3->objectName());
	table.clear();
	interpreterBase::blocksBase::BlockInterface *block4 = table.block(qReal::Id("a", "b", "c", "d"));
	interpreterBase::blocksBase::BlockInterface *block5 = table.block(qReal::Id("a", "b", "c", "d"));
	interpreterBase::blocksBase::BlockInterface *block6 = table.block(qReal::Id("a", "b", "c", "e"));
	ASSERT_NE(nullptr, block4);
	ASSERT_EQ(block4, block5);
	ASSERT_NE(nullptr, block6);
	ASSERT_NE(block4, block6);
	ASSERT_NE(QString("block1"), block4->objectName());
	ASSERT_NE(QString("block1"), block5->objectName());
	ASSERT_NE(QString("block3"), block6->objectName());
}
