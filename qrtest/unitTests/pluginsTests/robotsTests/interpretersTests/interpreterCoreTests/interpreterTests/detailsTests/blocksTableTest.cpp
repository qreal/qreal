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

#include "blocksTableTest.h"

#include <QtCore/QSharedPointer>

#include "interpreterCore/interpreter/details/blocksTable.h"
#include "support/dummyBlocksFactory.h"

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace qrTest::robotsTests::interpreterCoreTests::detailsTests;
using namespace interpreterCore::interpreter::details;

using namespace ::testing;

void BlocksTableTest::SetUp()
{
	QSharedPointer<DummyBlockFactory> blocksFactory(new DummyBlockFactory());

	ON_CALL(mBlocksFactoryManager, addFactory(_, _)).WillByDefault(Return());
	EXPECT_CALL(mBlocksFactoryManager, addFactory(_, _)).Times(0);

	ON_CALL(mBlocksFactoryManager, block(_, _)).WillByDefault(
			Invoke([=] (qReal::Id const &id, kitBase::robotModel::RobotModelInterface const &robotModel) {
					Q_UNUSED(robotModel)
					return blocksFactory->block(id);
			} )
			);

	EXPECT_CALL(mBlocksFactoryManager, block(_, _)).Times(AtLeast(0));

	ON_CALL(mBlocksFactoryManager, enabledBlocks(_)).WillByDefault(
			Invoke([=] (kitBase::robotModel::RobotModelInterface const &robotModel) {
					Q_UNUSED(robotModel)
					return blocksFactory->providedBlocks().toSet();
			} )
			);
	EXPECT_CALL(mBlocksFactoryManager, enabledBlocks(_)).Times(0);

	ON_CALL(mModelManager, model()).WillByDefault(ReturnRef(mModel));
	EXPECT_CALL(mModelManager, model()).Times(AtLeast(1));
}

TEST_F(BlocksTableTest, blocksCreation)
{
	BlocksTable table(mBlocksFactoryManager, mModelManager);
	qReal::interpretation::BlockInterface *block1 = table.block(qReal::Id("a", "b", "c", "d"));
	qReal::interpretation::BlockInterface *block2 = table.block(qReal::Id("a", "b", "c", "d"));
	qReal::interpretation::BlockInterface *block3 = table.block(qReal::Id("a", "b", "c", "e"));
	ASSERT_NE(nullptr, block1);
	ASSERT_EQ(block1, block2);
	ASSERT_NE(nullptr, block3);
	ASSERT_NE(block1, block3);
}

TEST_F(BlocksTableTest, clear)
{
	BlocksTable table(mBlocksFactoryManager, mModelManager);
	qReal::interpretation::BlockInterface *block1 = table.block(qReal::Id("a", "b", "c", "d"));
	qReal::interpretation::BlockInterface *block2 = table.block(qReal::Id("a", "b", "c", "d"));
	qReal::interpretation::BlockInterface *block3 = table.block(qReal::Id("a", "b", "c", "e"));
	ASSERT_NE(nullptr, block1);
	ASSERT_EQ(block1, block2);
	ASSERT_NE(nullptr, block3);
	ASSERT_NE(block1, block3);
	block1->setObjectName("block1");
	block3->setObjectName("block3");
	ASSERT_EQ(QString("block1"), block1->objectName());
	ASSERT_EQ(QString("block3"), block3->objectName());
	table.clear();
	qReal::interpretation::BlockInterface *block4 = table.block(qReal::Id("a", "b", "c", "d"));
	qReal::interpretation::BlockInterface *block5 = table.block(qReal::Id("a", "b", "c", "d"));
	qReal::interpretation::BlockInterface *block6 = table.block(qReal::Id("a", "b", "c", "e"));
	ASSERT_NE(nullptr, block4);
	ASSERT_EQ(block4, block5);
	ASSERT_NE(nullptr, block6);
	ASSERT_NE(block4, block6);
	ASSERT_NE(QString("block1"), block4->objectName());
	ASSERT_NE(QString("block1"), block5->objectName());
	ASSERT_NE(QString("block3"), block6->objectName());
}
