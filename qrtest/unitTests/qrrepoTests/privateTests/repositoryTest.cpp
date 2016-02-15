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

#include <QtCore/QFile>
#include <QtCore/QDir>

#include "repositoryTest.h"
#include "../../../qrrepo/private/classes/logicalObject.h"
#include "../../../qrkernel/exception/exception.h"
#include "../../../qrkernel/settingsManager.h"

using namespace qrRepo;
using namespace details;
using namespace qReal;
using namespace qrTest;

const Id parent("editor1", "diagram1", "element1", "parent");
const Id root("editor1", "diagram1", "element2", "root");

const Id child1("editor1", "diagram2", "element3", "child1");
const Id child2("editor1", "diagram2", "element3", "child2");
const Id child3("editor2", "diagram3", "element4", "child3");

const Id child1_child("editor2", "diagram3", "element5", "child1_child");
const Id child2_child("editor2", "diagram4", "element6", "child2_child");
const Id child3_child("editor2", "diagram4", "element7", "child3_child");

const Id rootLogicalId("editor2", "diagram3", "element5", "child1_child");
const Id child1LogicalId("editor2", "diagram3", "element5", "child1_child");
const Id child1_childLogicalId("editor2", "diagram4", "element6", "child2_child");
const Id child2_childLogicalId("editor2", "diagram4", "element6", "child2_child");

const Id fakeParent("fake", "fake", "fake", "fake");
const Id notExistingId("1", "1", "1", "1");

const Id newId1("editor1", "diagram1", "element1", "id1");
const Id newId2("editor2", "diagram2", "element2", "id2");


void RepositoryTest::removeDirectory(QString const &dirName)
{
	QDir const dir(dirName);

	foreach (QFileInfo info, dir.entryInfoList(QDir::Hidden
			| QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsFirst))
	{
		if (info.isDir()) {
			removeDirectory(info.absoluteFilePath());
		} else {
			QFile::remove(info.absoluteFilePath());
		}
	}

	dir.rmdir(dir.absolutePath());
}

void RepositoryTest::SetUp() {
	LogicalObject parentObj(parent);
	parentObj.setParent(fakeParent);

	GraphicalObject rootObj(root, parent, rootLogicalId);
	GraphicalObject child1Obj(child1, root, child1LogicalId);

	LogicalObject child2Obj(child2);
	child2Obj.setParent(root);

	LogicalObject child3Obj(child3);
	child3Obj.setParent(root);

	GraphicalObject child1_childObj(child1_child, child1, child1_childLogicalId);
	GraphicalObject child2_childObj(child2_child, child2, child2_childLogicalId);

	LogicalObject child3_childObj(child3_child);
	child3_childObj.setParent(child3);

	parentObj.addChild(root);
	rootObj.addChild(child1);
	rootObj.addChild(child2);
	rootObj.addChild(child3);
	child1Obj.addChild(child1_child);
	child2Obj.addChild(child2_child);
	child3Obj.addChild(child3_child);

	rootObj.setProperty("property1", "value1");
	child3_childObj.setProperty("property2", "value2");
	child2Obj.setProperty("property3", "val3");
	child1_childObj.setProperty("property2", "logicalVal");

	parentObj.setProperty("name", "parent");
	rootObj.setProperty("name", "root");
	child1Obj.setProperty("name", "child1");
	child2Obj.setProperty("name", "child2");
	child3Obj.setProperty("name", "child3");
	child1_childObj.setProperty("name", "child1_child");
	child2_childObj.setProperty("name", "child2_child");
	child3_childObj.setProperty("name", "child3_child");

	QList<Object *> list;
	list.push_back(&parentObj);
	list.push_back(&rootObj);
	list.push_back(&child1Obj);
	list.push_back(&child2Obj);
	list.push_back(&child3Obj);
	list.push_back(&child1_childObj);
	list.push_back(&child2_childObj);
	list.push_back(&child3_childObj);

	Serializer serializer("saveFile");
	serializer.saveToDisk(list, QHash<QString, QVariant>());
	mRepository = new Repository("saveFile.qrs");

	serializer.clearWorkingDir();

	LogicalObject newObj1(newId1);
	LogicalObject newObj2(newId2);

	newObj1.setProperty("property1", "value1");
	newObj2.setProperty("property2", "value2");

	QList<Object *> newList;
	newList.push_back(&newObj1);
	newList.push_back(&newObj2);

	serializer.setWorkingFile("newSaveFile");
	serializer.saveToDisk(newList, QHash<QString, QVariant>());
}

void RepositoryTest::TearDown() {
	delete mRepository;

	QFile::remove("saveFile.qrs");
	QFile::remove("newSaveFile.qrs");
}

TEST_F(RepositoryTest, replacePropertiesTest) {
	IdList toReplace;
	toReplace << root << child1 << child2 << child3 << child1_child << child2_child << child3_child;
	mRepository->replaceProperties(toReplace, "value", "replacedValue");

	EXPECT_EQ(mRepository->property(root, "property1").toString(), "replacedValue");
	EXPECT_EQ(mRepository->property(child3_child, "property2").toString(), "replacedValue");
	EXPECT_EQ(mRepository->property(child2, "property3").toString(), "val3");
}

TEST_F(RepositoryTest, findElementsByNameTest) {
	IdList list = mRepository->findElementsByName("child3", false, false);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->findElementsByName("child2", false, false);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(child2_child));

	list = mRepository->findElementsByName("child1", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(child1));
	EXPECT_TRUE(list.contains(child1_child));

	list = mRepository->findElementsByName("ChilD1", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(child1));
	EXPECT_TRUE(list.contains(child1_child));

	list = mRepository->findElementsByName("ChiLd1", true, false);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->findElementsByName("C.*D1.*", true, true);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->elementsByPropertyContent("c.*1.*", false, true);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(child1));
	EXPECT_TRUE(list.contains(child1_child));
}

TEST_F(RepositoryTest, elementsByPropertyTest) {
	IdList list = mRepository->elementsByProperty("property1", false, false);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(root));

	list = mRepository->elementsByProperty("ProPerTy1", false, false);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(root));

	list = mRepository->elementsByProperty("ProPerTy1", true, false);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->elementsByProperty("ProP.*", true, true);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->elementsByProperty("prop.*[1,2]", false, true);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(root));
	EXPECT_TRUE(list.contains(child1_child));
}

TEST_F(RepositoryTest, elementsByPropertyContentTest) {
	IdList list = mRepository->elementsByPropertyContent("value", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(root));
	EXPECT_TRUE(list.contains(child3_child));

	list = mRepository->elementsByPropertyContent("VaLuE", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(root));
	EXPECT_TRUE(list.contains(child3_child));

	list = mRepository->elementsByPropertyContent("VaLuE", true, false);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->elementsByPropertyContent("Val.*1", true, true);
	EXPECT_EQ(list.size(), 0);

	list = mRepository->elementsByPropertyContent("val.*1", false, true);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(root));
}

TEST_F(RepositoryTest, parentOperationsTest) {
	EXPECT_EQ(mRepository->parent(child1), root);
	EXPECT_EQ(mRepository->parent(child2), root);
	EXPECT_EQ(mRepository->parent(child3), root);

	mRepository->setParent(child1, parent);

	EXPECT_EQ(mRepository->parent(child1), parent);
	ASSERT_EQ(mRepository->children(parent).size(), 2);
	EXPECT_TRUE(mRepository->children(parent).contains(root));
	EXPECT_TRUE(mRepository->children(parent).contains(child1));

	mRepository->removeParent(child1);

	EXPECT_EQ(mRepository->parent(child1), Id());
	ASSERT_EQ(mRepository->children(parent).size(), 1);
	EXPECT_TRUE(mRepository->children(parent).contains(root));

	EXPECT_THROW(mRepository->parent(notExistingId), Exception);
	EXPECT_THROW(mRepository->setParent(child1, notExistingId), Exception);
	EXPECT_THROW(mRepository->setParent(notExistingId, child1), Exception);
	EXPECT_THROW(mRepository->removeParent(notExistingId), Exception);
	EXPECT_THROW(mRepository->removeParent(parent), Exception);
}

TEST_F(RepositoryTest, childOperationsTest) {
	ASSERT_EQ(mRepository->children(parent).size(), 1);
	EXPECT_TRUE(mRepository->children(parent).contains(root));

	ASSERT_EQ(mRepository->children(root).size(), 3);
	EXPECT_TRUE(mRepository->children(root).contains(child1));
	EXPECT_TRUE(mRepository->children(root).contains(child2));
	EXPECT_TRUE(mRepository->children(root).contains(child3));

	ASSERT_EQ(mRepository->children(child1).size(), 1);
	EXPECT_TRUE(mRepository->children(child1).contains(child1_child));
	ASSERT_EQ(mRepository->children(child2).size(), 1);
	EXPECT_TRUE(mRepository->children(child2).contains(child2_child));
	ASSERT_EQ(mRepository->children(child3).size(), 1);
	EXPECT_TRUE(mRepository->children(child3).contains(child3_child));

	mRepository->addChild(root, child1_child);

	ASSERT_EQ(mRepository->children(root).size(), 4);
	EXPECT_TRUE(mRepository->children(root).contains(child1_child));
	EXPECT_EQ(mRepository->parent(child1_child), root);

	const Id child("child", "child", "child", "child");
	const Id childLogId("child", "child", "child", "childLogId");
	mRepository->addChild(child1_child, child, childLogId);

	ASSERT_EQ(mRepository->children(child1_child).size(), 1);
	EXPECT_TRUE(mRepository->children(child1_child).contains(child));
	EXPECT_EQ(mRepository->parent(child), child1_child);
	EXPECT_EQ(mRepository->logicalId(child), childLogId);

	mRepository->removeChild(root, child1_child);
	EXPECT_FALSE(mRepository->children(root).contains(child1_child));

	EXPECT_THROW(mRepository->children(notExistingId), Exception);
	EXPECT_THROW(mRepository->addChild(notExistingId, child1), Exception);
	EXPECT_THROW(mRepository->removeChild(notExistingId, child1), Exception);
	EXPECT_THROW(mRepository->removeChild(root, child3_child), Exception);
}

TEST_F(RepositoryTest, elementsAndExistTest) {
	IdList elems = mRepository->elements();
	ASSERT_EQ(elems.size(), 9);
	EXPECT_TRUE(elems.contains(Id::rootId()));
	EXPECT_TRUE(elems.contains(parent));
	EXPECT_TRUE(elems.contains(root));
	EXPECT_TRUE(elems.contains(child1));
	EXPECT_TRUE(elems.contains(child2));
	EXPECT_TRUE(elems.contains(child3));
	EXPECT_TRUE(elems.contains(child1_child));
	EXPECT_TRUE(elems.contains(child2_child));
	EXPECT_TRUE(elems.contains(child3_child));

	EXPECT_TRUE(mRepository->exist(parent));
	EXPECT_TRUE(mRepository->exist(root));
	EXPECT_TRUE(mRepository->exist(child1));
	EXPECT_TRUE(mRepository->exist(child2));
	EXPECT_TRUE(mRepository->exist(child3));
	EXPECT_TRUE(mRepository->exist(child1_child));
	EXPECT_TRUE(mRepository->exist(child2_child));
	EXPECT_TRUE(mRepository->exist(child3_child));

	EXPECT_FALSE(mRepository->exist(notExistingId));
}

TEST_F(RepositoryTest, logicalIdTest) {
	EXPECT_FALSE(mRepository->isLogicalId(root));
	EXPECT_FALSE(mRepository->isLogicalId(child1));
	EXPECT_FALSE(mRepository->isLogicalId(child1_child));
	EXPECT_FALSE(mRepository->isLogicalId(child2_child));

	EXPECT_TRUE(mRepository->isLogicalId(parent));
	EXPECT_TRUE(mRepository->isLogicalId(child2));
	EXPECT_TRUE(mRepository->isLogicalId(child3));
	EXPECT_TRUE(mRepository->isLogicalId(child3_child));

	EXPECT_EQ(mRepository->logicalId(root),rootLogicalId);
}

TEST_F(RepositoryTest, removeIdTest) {
	mRepository->remove(root);
	EXPECT_FALSE(mRepository->exist(root));

	EXPECT_THROW(mRepository->remove(notExistingId), Exception);
}

// Same as removeFromDisk test fro Serializer
TEST_F(RepositoryTest, removeIdListTest) {
	mRepository->serializer().decompressFile("saveFile.qrs");
	IdList toRemove;
	toRemove << child3 << child1_child << child2_child << child3_child;
	mRepository->remove(toRemove);

	EXPECT_TRUE(true);
}

TEST_F(RepositoryTest, stackBeforeTest) {
	EXPECT_THROW(mRepository->stackBefore(notExistingId, child1, child2), Exception);
	EXPECT_THROW(mRepository->stackBefore(root, notExistingId, child2), Exception);
	EXPECT_THROW(mRepository->stackBefore(root, child1, notExistingId), Exception);

	mRepository->addChild(root, child1_child);
	mRepository->addChild(root, child2_child);

	mRepository->stackBefore(root, child1_child, child2);
	ASSERT_EQ(mRepository->children(root).indexOf(child1_child) + 1, mRepository->children(root).indexOf(child2));

	mRepository->stackBefore(root, child1, child2_child);
	ASSERT_EQ(mRepository->children(root).indexOf(child1) + 1, mRepository->children(root).indexOf(child2_child));
}

TEST_F(RepositoryTest, cloneObjectTest) {
	const Id clonedId = mRepository->cloneObject(root);
	EXPECT_EQ(clonedId.type(), root.type());
	EXPECT_TRUE(clonedId != root);
}

TEST_F(RepositoryTest, exterminateTest) {
	mRepository->exterminate();

	EXPECT_EQ(mRepository->elements().size(), 1);
	EXPECT_TRUE(mRepository->elements().contains(Id::rootId()));
}

TEST_F(RepositoryTest, openTest) {
	mRepository->open("newSaveFile.qrs");
	EXPECT_EQ(mRepository->elements().size(), 3);
	EXPECT_TRUE(mRepository->elements().contains(Id::rootId()));
	EXPECT_TRUE(mRepository->elements().contains(newId1));
	EXPECT_TRUE(mRepository->elements().contains(newId2));

	ASSERT_TRUE(mRepository->hasProperty(newId1, "property1"));
	ASSERT_TRUE(mRepository->hasProperty(newId2, "property2"));

	EXPECT_EQ(mRepository->property(newId1, "property1").toString(), "value1");
	EXPECT_EQ(mRepository->property(newId2, "property2").toString(), "value2");
}

TEST_F(RepositoryTest,importFromDiskTest) {
	mRepository->importFromDisk("newSaveFile.qrs");

	IdList elems = mRepository->elements();
	ASSERT_EQ(elems.size(), 11);
	EXPECT_TRUE(elems.contains(Id::rootId()));
	EXPECT_TRUE(elems.contains(parent));
	EXPECT_TRUE(elems.contains(root));
	EXPECT_TRUE(elems.contains(child1));
	EXPECT_TRUE(elems.contains(child2));
	EXPECT_TRUE(elems.contains(child3));
	EXPECT_TRUE(elems.contains(child1_child));
	EXPECT_TRUE(elems.contains(child2_child));
	EXPECT_TRUE(elems.contains(child3_child));
	EXPECT_TRUE(elems.contains(newId1));
	EXPECT_TRUE(elems.contains(newId2));

	ASSERT_TRUE(mRepository->hasProperty(newId1, "property1"));
	ASSERT_TRUE(mRepository->hasProperty(newId2, "property2"));

	EXPECT_EQ(mRepository->property(newId1, "property1").toString(), "value1");
	EXPECT_EQ(mRepository->property(newId2, "property2").toString(), "value2");
}

TEST_F(RepositoryTest, workingFileTest) {
	EXPECT_EQ(mRepository->workingFile(), "saveFile.qrs");

	mRepository->setWorkingFile("newSaveFile");
	EXPECT_EQ(mRepository->workingFile(), "newSaveFile");
}

TEST_F(RepositoryTest, singlePropertyOperationsTest) {
	ASSERT_TRUE(mRepository->hasProperty(root, "property1"));
	ASSERT_TRUE(mRepository->hasProperty(child3_child, "property2"));
	ASSERT_TRUE(mRepository->hasProperty(child2, "property3"));
	ASSERT_TRUE(mRepository->hasProperty(child1_child, "property2"));
	EXPECT_THROW(mRepository->hasProperty(notExistingId, "property"), Exception);

	EXPECT_TRUE(mRepository->hasProperty(root, "ProPeRty1", false, false));
	EXPECT_FALSE(mRepository->hasProperty(root, "ProPeRty1", true, false));
	EXPECT_TRUE(mRepository->hasProperty(root, "Prop.*1", false, true));
	EXPECT_FALSE(mRepository->hasProperty(root, "Prop.*1", true, true));
	EXPECT_FALSE(mRepository->hasProperty(root, "property2", false, false));

	EXPECT_EQ(mRepository->property(root, "property1").toString(), "value1");
	EXPECT_EQ(mRepository->property(child3_child, "property2").toString(), "value2");
	EXPECT_EQ(mRepository->property(child2, "property3").toString(), "val3");
	EXPECT_EQ(mRepository->property(child1_child, "property2").toString(), "logicalVal");
	EXPECT_THROW(mRepository->property(notExistingId, "property"), Exception);

	mRepository->setProperty(root, "test", "test");
	mRepository->setProperty(root, "property1", "test1");
	EXPECT_THROW(mRepository->setProperty(notExistingId, "property", "value"), Exception);

	EXPECT_TRUE(mRepository->hasProperty(root, "test"));
	EXPECT_EQ(mRepository->property(root, "test").toString(), "test");
	EXPECT_EQ(mRepository->property(root, "property1").toString(), "test1");

	mRepository->removeProperty(root, "test");
	EXPECT_THROW(mRepository->removeProperty(notExistingId, "property"), Exception);

	EXPECT_FALSE(mRepository->hasProperty(root, "test"));
}

// All used methods do not check existance of object with specified id
TEST_F(RepositoryTest, multiplePropertiesOperationsTest) {
	mRepository->setProperty(root, "test", "test");

	QMap<QString, QVariant> properties = mRepository->properties(root);
	//EXPECT_THROW(mRepository->properties(notExistingId), Exception);

	ASSERT_EQ(properties.keys().size(), 3);
	ASSERT_TRUE(properties.contains("test"));
	ASSERT_TRUE(properties.contains("property1"));
	ASSERT_TRUE(properties.contains("name"));
	EXPECT_EQ(properties.value("test").toString(), "test");
	EXPECT_EQ(properties.value("property1").toString(), "value1");
	EXPECT_EQ(properties.value("name").toString(), "root");

	mRepository->setProperties(child1, properties);
	properties = mRepository->properties(child1);
	//EXPECT_THROW(mRepository->setProperties(notExistingId, properties), Exception);

	ASSERT_EQ(properties.keys().size(), 3);
	ASSERT_TRUE(properties.contains("test"));
	ASSERT_TRUE(properties.contains("property1"));
	ASSERT_TRUE(properties.contains("name"));
	EXPECT_EQ(properties.value("test").toString(), "test");
	EXPECT_EQ(properties.value("property1").toString(), "value1");
	EXPECT_EQ(properties.value("name").toString(), "root");

	mRepository->removeProperty(child2, "name");
	QMapIterator<QString, QVariant> iterator = mRepository->propertiesIterator(child2);
	//EXPECT_THROW(mRepository->propertiesIterator(notExistingId), Exception);
	iterator.next();
	EXPECT_EQ(iterator.key(), "property3");
	EXPECT_EQ(iterator.value().toString(), "val3");
}

// Method does not check existance of object with specified id
TEST_F(RepositoryTest, copyPropertiesTest) {
	//EXPECT_THROW(mRepository->copyProperties(notExistingId, root), Exception);
	//EXPECT_THROW(mRepository->copyProperties(root, notExistingId), Exception);
	mRepository->copyProperties(root, child2);

	ASSERT_TRUE(mRepository->hasProperty(root, "property3"));
	ASSERT_TRUE(mRepository->hasProperty(root, "name"));
	ASSERT_FALSE(mRepository->hasProperty(child2, "property1"));
	EXPECT_EQ(mRepository->property(root, "name").toString(), "child2");
	EXPECT_EQ(mRepository->property(root, "property3").toString(), "val3");
}

TEST_F(RepositoryTest, backReferenceTest) {
	const Id backReference1("editor", "diagram", "element", "backReference1");
	const Id backReference2("editor1", "diagram2", "element3", "child1");
	const Id backReference3("editor1", "diagram2", "element3", "child2");

	EXPECT_EQ(mRepository->property(root, "backReferences"), QVariant());
	EXPECT_THROW(mRepository->removeBackReference(root, backReference1), Exception);
	EXPECT_THROW(mRepository->removeBackReference(notExistingId, backReference1), Exception);

	mRepository->setBackReference(root, backReference2);
	EXPECT_THROW(mRepository->setBackReference(root, backReference1), Exception);
	EXPECT_THROW(mRepository->removeBackReference(root, backReference3), Exception);

	mRepository->setBackReference(root, backReference2);
	mRepository->setBackReference(root, backReference3);
	mRepository->removeBackReference(root, backReference2);

	IdList references = mRepository->property(root, "backReferences").value<IdList>();
	ASSERT_EQ(references.size(), 2);
	EXPECT_TRUE(references.contains(backReference3));
}

TEST_F(RepositoryTest, temporaryRemovedLinksTest) {
	const Id linkTo1("editor", "diagram", "element", "linkTo1");
	const Id linkTo2("editor", "diagram", "element", "linkTo2");
	const Id linkFrom1("editor", "diagram", "element", "linkFrom1");
	const Id linkFrom2("editor", "diagram", "element", "linkFrom2");
	const Id link("editor", "diagram", "element", "link");

	IdList to;
	IdList from;
	IdList empty;
	to << linkTo1 << linkTo2;
	from << linkFrom1 << linkFrom2;
	empty << link;

	ASSERT_EQ(mRepository->temporaryRemovedLinks(root).size(), 0);
	EXPECT_THROW(mRepository->temporaryRemovedLinks(notExistingId), Exception);

	mRepository->setTemporaryRemovedLinks(root, "to", to);
	mRepository->setTemporaryRemovedLinks(root, "from", from);
	mRepository->setTemporaryRemovedLinks(root, QString(), empty);
	EXPECT_THROW(mRepository->setTemporaryRemovedLinks(notExistingId, "to", to), Exception);

	mRepository->setProperty(root, "to", qReal::IdListHelper::toVariant(to));
	mRepository->setProperty(root, "from", qReal::IdListHelper::toVariant(from));
	mRepository->setProperty(root, QString(), qReal::IdListHelper::toVariant(empty));

	ASSERT_EQ(mRepository->temporaryRemovedLinks(root).size(), 5);
	EXPECT_TRUE(mRepository->temporaryRemovedLinks(root).contains(linkTo1));
	EXPECT_TRUE(mRepository->temporaryRemovedLinks(root).contains(linkTo2));
	EXPECT_TRUE(mRepository->temporaryRemovedLinks(root).contains(linkFrom1));
	EXPECT_TRUE(mRepository->temporaryRemovedLinks(root).contains(linkFrom2));
	EXPECT_TRUE(mRepository->temporaryRemovedLinks(root).contains(link));

	ASSERT_EQ(mRepository->temporaryRemovedLinksAt(root, "to").size(), 2);
	EXPECT_TRUE(mRepository->temporaryRemovedLinksAt(root, "to").contains(linkTo1));
	EXPECT_TRUE(mRepository->temporaryRemovedLinksAt(root, "to").contains(linkTo2));

	ASSERT_EQ(mRepository->temporaryRemovedLinksAt(root, "from").size(), 2);
	EXPECT_TRUE(mRepository->temporaryRemovedLinksAt(root, "from").contains(linkFrom1));
	EXPECT_TRUE(mRepository->temporaryRemovedLinksAt(root, "from").contains(linkFrom2));

	ASSERT_EQ(mRepository->temporaryRemovedLinksAt(root, QString()).size(), 1);
	EXPECT_TRUE(mRepository->temporaryRemovedLinksAt(root, QString()).contains(link));

	mRepository->removeTemporaryRemovedLinks(root);
	ASSERT_FALSE(mRepository->hasProperty(root, "to"));
	ASSERT_FALSE(mRepository->hasProperty(root, "from"));
	ASSERT_FALSE(mRepository->hasProperty(root, QString()));
}

TEST_F(RepositoryTest, saveAllTest) {
	mRepository->serializer().clearWorkingDir();
	mRepository->remove(root);
	mRepository->saveAll();
	mRepository->open("saveFile.qrs");

	EXPECT_FALSE(mRepository->exist(root));
	EXPECT_TRUE(mRepository->exist(parent));
	EXPECT_TRUE(mRepository->exist(child1));
	EXPECT_TRUE(mRepository->exist(child2));
	EXPECT_TRUE(mRepository->exist(child3));
	EXPECT_TRUE(mRepository->exist(child1_child));
	EXPECT_TRUE(mRepository->exist(child2_child));
	EXPECT_TRUE(mRepository->exist(child3_child));
}

TEST_F(RepositoryTest, saveTest) {
	mRepository->serializer().clearWorkingDir();
	IdList toSave;
	toSave << child1 << child2 << child3;
	mRepository->save(toSave);
	mRepository->open("saveFile.qrs");

	EXPECT_EQ(mRepository->elements().size(), 7);
	EXPECT_TRUE(mRepository->exist(child1));
	EXPECT_TRUE(mRepository->exist(child2));
	EXPECT_TRUE(mRepository->exist(child3));
	EXPECT_TRUE(mRepository->exist(child1_child));
	EXPECT_TRUE(mRepository->exist(child2_child));
	EXPECT_TRUE(mRepository->exist(child3_child));
}

TEST_F(RepositoryTest, saveWithLogicalIdTest) {
	mRepository->serializer().clearWorkingDir();
	IdList toSave;
	toSave << child1 << child3_child;
	mRepository->saveWithLogicalId(toSave);
	mRepository->open("saveFile.qrs");

	EXPECT_EQ(mRepository->elements().size(), 5);
	EXPECT_TRUE(mRepository->exist(child1));
	EXPECT_TRUE(mRepository->exist(child1_child));
	EXPECT_TRUE(mRepository->exist(child2_child));
	EXPECT_TRUE(mRepository->exist(child3_child));
}

TEST_F(RepositoryTest, saveDiagramsByIdTest) {
	mRepository->serializer().clearWorkingDir();
	IdList toSave;
	toSave << child1;

	QHash<QString, IdList> diagramIds;
	diagramIds.insert("diagram1", toSave);

	mRepository->saveDiagramsById(diagramIds);

	mRepository->serializer().decompressFile("diagram1.qrs");

	const QString unsavedTree = mRepository->serializer().workingDirectory() + "/tree/";
	EXPECT_TRUE(QFile::exists(unsavedTree + "graphical/editor1/diagram2/element3/child1"));
	EXPECT_TRUE(QFile::exists(unsavedTree + "graphical/editor2/diagram3/element5/child1_child"));
	EXPECT_TRUE(QFile::exists(unsavedTree + "graphical/editor2/diagram4/element6/child2_child"));
	EXPECT_FALSE(QFile::exists(unsavedTree + "graphical/editor1/diagram1/element2/root"));
	EXPECT_FALSE(QDir().exists(unsavedTree + "logical"));

	QFile::remove("diagram1.qrs");
}
