#include <QtCore/QFile>
#include <QtCore/QDir>

#include "clientTest.h"
#include "../../../qrrepo/private/classes/object.h"
#include "../../../qrkernel/exception/exception.h"

using namespace qrRepo;
using namespace details;
using namespace qReal;
using namespace qrTest;

Id const parent("editor1", "diagram1", "element1", "parent");
Id const root("editor1", "diagram1", "element2", "root");

Id const child1("editor1", "diagram2", "element3", "child1");
Id const child2("editor1", "diagram2", "element3", "child2");
Id const child3("editor2", "diagram3", "element4", "child3");

Id const child1_child("editor2", "diagram3", "element5", "child1_child");
Id const child2_child("editor2", "diagram4", "element6", "child2_child");
Id const child3_child("editor2", "diagram4", "element7", "child3_child");

Id const rootLogicalId("editor2", "diagram3", "element5", "child1_child");
Id const child1LogicalId("editor2", "diagram3", "element5", "child1_child");
Id const child1_childLogicalId("editor2", "diagram4", "element6", "child2_child");
Id const child2_childLogicalId("editor2", "diagram4", "element6", "child2_child");

Id const fakeParent("fake", "fake", "fake", "fake");
Id const notExistingId("1", "1", "1", "1");

Id const newId1("editor1", "diagram1", "element1", "id1");
Id const newId2("editor2", "diagram2", "element2", "id2");


void ClientTest::removeDirectory(QString const &dirName)
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

void ClientTest::SetUp() {
	mSerializer = new Serializer("saveFile");

	Object parentObj(parent, fakeParent);
	Object rootObj(root, parent, rootLogicalId);
	Object child1Obj(child1, root, child1LogicalId);
	Object child2Obj(child2, root);
	Object child3Obj(child3, root);
	Object child1_childObj(child1_child, child1, child1_childLogicalId);
	Object child2_childObj(child2_child, child2, child2_childLogicalId);
	Object child3_childObj(child3_child, child3);

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

	mSerializer->saveToDisk(list);

	mClient = new Client("saveFile.qrs");

	mSerializer->clearWorkingDir();
	delete mSerializer;

	Object newObj1(newId1);
	Object newObj2(newId2);

	newObj1.setProperty("property1", "value1");
	newObj2.setProperty("property2", "value2");

	QList<Object *> newList;
	newList.push_back(&newObj1);
	newList.push_back(&newObj2);

	mSerializer = new Serializer("newSaveFile");
	mSerializer->saveToDisk(newList);
}

void ClientTest::TearDown() {
	delete mClient;

	mSerializer->clearWorkingDir();
	delete mSerializer;

	QFile::remove("saveFile.qrs");
	QFile::remove("newSaveFile.qrs");
}

TEST_F(ClientTest, replacePropertiesTest) {
	IdList toReplace;
	toReplace << root << child1 << child2 << child3 << child1_child << child2_child << child3_child;
	mClient->replaceProperties(toReplace, "value", "replacedValue");

	EXPECT_EQ(mClient->property(root, "property1").toString(), "replacedValue");
	EXPECT_EQ(mClient->property(child3_child, "property2").toString(), "replacedValue");
	EXPECT_EQ(mClient->property(child2, "property3").toString(), "val3");
}

TEST_F(ClientTest, findElementsByNameTest) {
	IdList list = mClient->findElementsByName("child3", false, false);
	EXPECT_EQ(list.size(), 0);

	list = mClient->findElementsByName("child2", false, false);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(child2_child));

	list = mClient->findElementsByName("child1", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(child1));
	EXPECT_TRUE(list.contains(child1_child));
	
	list = mClient->findElementsByName("ChilD1", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(child1));
	EXPECT_TRUE(list.contains(child1_child));

	list = mClient->findElementsByName("ChiLd1", true, false);
	EXPECT_EQ(list.size(), 0);

	list = mClient->findElementsByName("C.*D1.*", true, true);
	EXPECT_EQ(list.size(), 0);

	list = mClient->elementsByPropertyContent("c.*1.*", false, true);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(child1));
	EXPECT_TRUE(list.contains(child1_child));
}

TEST_F(ClientTest, elementsByPropertyTest) {
	IdList list = mClient->elementsByProperty("property1", false, false);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(root));

	list = mClient->elementsByProperty("ProPerTy1", false, false);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(root));

	list = mClient->elementsByProperty("ProPerTy1", true, false);
	EXPECT_EQ(list.size(), 0);

	list = mClient->elementsByProperty("ProP.*", true, true);
	EXPECT_EQ(list.size(), 0);

	list = mClient->elementsByProperty("prop.*[1,2]", false, true);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(root));
	EXPECT_TRUE(list.contains(child1_child));
}

TEST_F(ClientTest, elementsByPropertyContentTest) {
	IdList list = mClient->elementsByPropertyContent("value", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(root));
	EXPECT_TRUE(list.contains(child3_child));

	list = mClient->elementsByPropertyContent("VaLuE", false, false);
	EXPECT_EQ(list.size(), 2);
	EXPECT_TRUE(list.contains(root));
	EXPECT_TRUE(list.contains(child3_child));

	list = mClient->elementsByPropertyContent("VaLuE", true, false);
	EXPECT_EQ(list.size(), 0);

	list = mClient->elementsByPropertyContent("Val.*1", true, true);
	EXPECT_EQ(list.size(), 0);

	list = mClient->elementsByPropertyContent("val.*1", false, true);
	EXPECT_EQ(list.size(), 1);
	EXPECT_TRUE(list.contains(root));
}

TEST_F(ClientTest, parentOperationsTest) {
	EXPECT_EQ(mClient->parent(child1), root);
	EXPECT_EQ(mClient->parent(child2), root);
	EXPECT_EQ(mClient->parent(child3), root);

	mClient->setParent(child1, parent);

	EXPECT_EQ(mClient->parent(child1), parent);
	ASSERT_EQ(mClient->children(parent).size(), 2);
	EXPECT_TRUE(mClient->children(parent).contains(root));
	EXPECT_TRUE(mClient->children(parent).contains(child1));

	mClient->removeParent(child1);

	EXPECT_EQ(mClient->parent(child1), Id());
	ASSERT_EQ(mClient->children(parent).size(), 1);
	EXPECT_TRUE(mClient->children(parent).contains(root));

	EXPECT_THROW(mClient->parent(notExistingId), Exception);
	EXPECT_THROW(mClient->setParent(child1, notExistingId), Exception);
	EXPECT_THROW(mClient->setParent(notExistingId, child1), Exception);
	EXPECT_THROW(mClient->removeParent(notExistingId), Exception);
	EXPECT_THROW(mClient->removeParent(parent), Exception);
}

TEST_F(ClientTest, childOperationsTest) {
	ASSERT_EQ(mClient->children(parent).size(), 1);
	EXPECT_TRUE(mClient->children(parent).contains(root));

	ASSERT_EQ(mClient->children(root).size(), 3);
	EXPECT_TRUE(mClient->children(root).contains(child1));
	EXPECT_TRUE(mClient->children(root).contains(child2));
	EXPECT_TRUE(mClient->children(root).contains(child3));

	ASSERT_EQ(mClient->children(child1).size(), 1);
	EXPECT_TRUE(mClient->children(child1).contains(child1_child));
	ASSERT_EQ(mClient->children(child2).size(), 1);
	EXPECT_TRUE(mClient->children(child2).contains(child2_child));
	ASSERT_EQ(mClient->children(child3).size(), 1);
	EXPECT_TRUE(mClient->children(child3).contains(child3_child));

	mClient->addChild(root, child1_child);

	ASSERT_EQ(mClient->children(root).size(), 4);
	EXPECT_TRUE(mClient->children(root).contains(child1_child));
	EXPECT_EQ(mClient->parent(child1_child), root);

	Id const child("child", "child", "child", "child");
	Id const childLogId("child", "child", "child", "childLogId");
	mClient->addChild(child1_child, child, childLogId);

	ASSERT_EQ(mClient->children(child1_child).size(), 1);
	EXPECT_TRUE(mClient->children(child1_child).contains(child));
	EXPECT_EQ(mClient->parent(child), child1_child);
	EXPECT_EQ(mClient->logicalId(child), childLogId);

	mClient->removeChild(root, child1_child);
	EXPECT_FALSE(mClient->children(root).contains(child1_child));

	EXPECT_THROW(mClient->children(notExistingId), Exception);
	EXPECT_THROW(mClient->addChild(notExistingId, child1), Exception);
	EXPECT_THROW(mClient->removeChild(notExistingId, child1), Exception);
	EXPECT_THROW(mClient->removeChild(root, child3_child), Exception);
}

TEST_F(ClientTest, elementsAndExistTest) {
	IdList elems = mClient->elements();
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

	EXPECT_TRUE(mClient->exist(parent));
	EXPECT_TRUE(mClient->exist(root));
	EXPECT_TRUE(mClient->exist(child1));
	EXPECT_TRUE(mClient->exist(child2));
	EXPECT_TRUE(mClient->exist(child3));
	EXPECT_TRUE(mClient->exist(child1_child));
	EXPECT_TRUE(mClient->exist(child2_child));
	EXPECT_TRUE(mClient->exist(child3_child));

	EXPECT_FALSE(mClient->exist(notExistingId));
}

TEST_F(ClientTest, logicalIdTest) {
	EXPECT_FALSE(mClient->isLogicalId(root));
	EXPECT_FALSE(mClient->isLogicalId(child1));
	EXPECT_FALSE(mClient->isLogicalId(child1_child));
	EXPECT_FALSE(mClient->isLogicalId(child2_child));

	EXPECT_TRUE(mClient->isLogicalId(parent));
	EXPECT_TRUE(mClient->isLogicalId(child2));
	EXPECT_TRUE(mClient->isLogicalId(child3));
	EXPECT_TRUE(mClient->isLogicalId(child3_child));

	EXPECT_EQ(mClient->logicalId(root),rootLogicalId);
}

TEST_F(ClientTest, removeIdTest) {
	mClient->remove(root);
	EXPECT_FALSE(mClient->exist(root));

	EXPECT_THROW(mClient->remove(notExistingId), Exception);
}

// Same as removeFromDisk test fro Serializer
TEST_F(ClientTest, removeIdListTest) {
	mSerializer->decompressFile("saveFile.qrs");
	IdList toRemove;
	toRemove << child3 << child1_child << child2_child << child3_child;
	mClient->remove(toRemove);

	//EXPECT_FALSE(QFile::exists("../unsaved/tree/logical/editor2/diagram4/element7/child3_child"));
	//EXPECT_FALSE(QFile::exists("../unsaved/tree/logical/editor2/diagram3/element4/child3"));
	//EXPECT_FALSE(QDir().exists("../unsaved/tree/logical/editor2"));
	//EXPECT_FALSE(QFile::exists("../unsaved/tree/graphical/editor2/diagram3/element5/child1_child"));
	//EXPECT_FALSE(QFile::exists("../unsaved/tree/graphical/editor2/diagram4/element6/child2_child"));
	//EXPECT_FALSE(QDir().exists("../unsaved/tree/graphical/editor2"));
	EXPECT_TRUE(true);
}

TEST_F(ClientTest, stackBeforeTest) {
	EXPECT_THROW(mClient->stackBefore(notExistingId, child1, child2), Exception);
	EXPECT_THROW(mClient->stackBefore(root, notExistingId, child2), Exception);
	EXPECT_THROW(mClient->stackBefore(root, child1, notExistingId), Exception);

	mClient->addChild(root, child1_child);
	mClient->addChild(root, child2_child);

	mClient->stackBefore(root, child1_child, child2);
	ASSERT_EQ(mClient->children(root).indexOf(child1_child) + 1, mClient->children(root).indexOf(child2));

	mClient->stackBefore(root, child1, child2_child);
	ASSERT_EQ(mClient->children(root).indexOf(child1) + 1, mClient->children(root).indexOf(child2_child));
}

TEST_F(ClientTest, cloneObjectTest) {
	Id const clonedId = mClient->cloneObject(root);
	EXPECT_EQ(clonedId.type(), root.type());
	EXPECT_TRUE(clonedId != root);
}

TEST_F(ClientTest, exterminateTest) {
	mClient->exterminate();

	EXPECT_EQ(mClient->elements().size(), 1);
	EXPECT_TRUE(mClient->elements().contains(Id::rootId()));
}

TEST_F(ClientTest, openTest) {
	mClient->open("newSaveFile.qrs");
	EXPECT_EQ(mClient->elements().size(), 3);
	EXPECT_TRUE(mClient->elements().contains(Id::rootId()));
	EXPECT_TRUE(mClient->elements().contains(newId1));
	EXPECT_TRUE(mClient->elements().contains(newId2));

	ASSERT_TRUE(mClient->hasProperty(newId1, "property1"));
	ASSERT_TRUE(mClient->hasProperty(newId2, "property2"));

	EXPECT_EQ(mClient->property(newId1, "property1").toString(), "value1");
	EXPECT_EQ(mClient->property(newId2, "property2").toString(), "value2");
}

TEST_F(ClientTest,importFromDiskTest) {
	mClient->importFromDisk("newSaveFile.qrs");

	IdList elems = mClient->elements();
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

	ASSERT_TRUE(mClient->hasProperty(newId1, "property1"));
	ASSERT_TRUE(mClient->hasProperty(newId2, "property2"));

	EXPECT_EQ(mClient->property(newId1, "property1").toString(), "value1");
	EXPECT_EQ(mClient->property(newId2, "property2").toString(), "value2");
}

TEST_F(ClientTest, workingFileTest) {
	EXPECT_EQ(mClient->workingFile(), "saveFile.qrs");

	mClient->setWorkingFile("newSaveFile");
	EXPECT_EQ(mClient->workingFile(), "newSaveFile");
}

TEST_F(ClientTest, singlePropertyOperationsTest) {
	ASSERT_TRUE(mClient->hasProperty(root, "property1"));
	ASSERT_TRUE(mClient->hasProperty(child3_child, "property2"));
	ASSERT_TRUE(mClient->hasProperty(child2, "property3"));
	ASSERT_TRUE(mClient->hasProperty(child1_child, "property2"));
	EXPECT_THROW(mClient->hasProperty(notExistingId, "property"), Exception);

	EXPECT_TRUE(mClient->hasProperty(root, "ProPeRty1", false, false));
	EXPECT_FALSE(mClient->hasProperty(root, "ProPeRty1", true, false));
	EXPECT_TRUE(mClient->hasProperty(root, "Prop.*1", false, true));
	EXPECT_FALSE(mClient->hasProperty(root, "Prop.*1", true, true));
	EXPECT_FALSE(mClient->hasProperty(root, "property2", false, false));

	EXPECT_EQ(mClient->property(root, "property1").toString(), "value1");
	EXPECT_EQ(mClient->property(child3_child, "property2").toString(), "value2");
	EXPECT_EQ(mClient->property(child2, "property3").toString(), "val3");
	EXPECT_EQ(mClient->property(child1_child, "property2").toString(), "logicalVal");
	EXPECT_THROW(mClient->property(notExistingId, "property"), Exception);

	mClient->setProperty(root, "test", "test");
	mClient->setProperty(root, "property1", "test1");
	EXPECT_THROW(mClient->setProperty(notExistingId, "property", "value"), Exception);

	EXPECT_TRUE(mClient->hasProperty(root, "test"));
	EXPECT_EQ(mClient->property(root, "test").toString(), "test");
	EXPECT_EQ(mClient->property(root, "property1").toString(), "test1");

	mClient->removeProperty(root, "test");
	EXPECT_THROW(mClient->removeProperty(notExistingId, "property"), Exception);

	EXPECT_FALSE(mClient->hasProperty(root, "test"));
}

// All used methods do not check existance of object with specified id
TEST_F(ClientTest, multiplePropertiesOperationsTest) {
	mClient->setProperty(root, "test", "test");

	QMap<QString, QVariant> properties = mClient->properties(root);
	//EXPECT_THROW(mClient->properties(notExistingId), Exception);

	ASSERT_EQ(properties.keys().size(), 3);
	ASSERT_TRUE(properties.contains("test"));
	ASSERT_TRUE(properties.contains("property1"));
	ASSERT_TRUE(properties.contains("name"));
	EXPECT_EQ(properties.value("test").toString(), "test");
	EXPECT_EQ(properties.value("property1").toString(), "value1");
	EXPECT_EQ(properties.value("name").toString(), "root");

	mClient->setProperties(child1, properties);
	properties = mClient->properties(child1);
	//EXPECT_THROW(mClient->setProperties(notExistingId, properties), Exception);

	ASSERT_EQ(properties.keys().size(), 3);
	ASSERT_TRUE(properties.contains("test"));
	ASSERT_TRUE(properties.contains("property1"));
	ASSERT_TRUE(properties.contains("name"));
	EXPECT_EQ(properties.value("test").toString(), "test");
	EXPECT_EQ(properties.value("property1").toString(), "value1");
	EXPECT_EQ(properties.value("name").toString(), "root");

	mClient->removeProperty(child2, "name");
	QMapIterator<QString, QVariant> iterator = mClient->propertiesIterator(child2);
	//EXPECT_THROW(mClient->propertiesIterator(notExistingId), Exception);
	iterator.next();
	EXPECT_EQ(iterator.key(), "property3");
	EXPECT_EQ(iterator.value().toString(), "val3");
}

// Method does not check existance of object with specified id
TEST_F(ClientTest, copyPropertiesTest) {
	//EXPECT_THROW(mClient->copyProperties(notExistingId, root), Exception);
	//EXPECT_THROW(mClient->copyProperties(root, notExistingId), Exception);
	mClient->copyProperties(root, child2);
	
	ASSERT_TRUE(mClient->hasProperty(root, "property3"));
	ASSERT_TRUE(mClient->hasProperty(root, "name"));
	ASSERT_FALSE(mClient->hasProperty(child2, "property1"));
	EXPECT_EQ(mClient->property(root, "name").toString(), "child2");
	EXPECT_EQ(mClient->property(root, "property3").toString(), "val3");
}

TEST_F(ClientTest, backReferenceTest) {
	Id const backReference1("editor", "diagram", "element", "backReference1");
	Id const backReference2("editor1", "diagram2", "element3", "child1");
	Id const backReference3("editor1", "diagram2", "element3", "child2");

	EXPECT_EQ(mClient->property(root, "backReferences"), QVariant());
	EXPECT_THROW(mClient->removeBackReference(root, backReference1), Exception);
	EXPECT_THROW(mClient->removeBackReference(notExistingId, backReference1), Exception);

	mClient->setBackReference(root, backReference2);
	EXPECT_THROW(mClient->setBackReference(root, backReference1), Exception);
	EXPECT_THROW(mClient->removeBackReference(root, backReference3), Exception);

	mClient->setBackReference(root, backReference2);
	mClient->setBackReference(root, backReference3);
	mClient->removeBackReference(root, backReference2);

	IdList references = mClient->property(root, "backReferences").value<IdList>();
	ASSERT_EQ(references.size(), 1);
	EXPECT_TRUE(references.contains(backReference3));
}

TEST_F(ClientTest, temporaryRemovedLinksTest) {
	Id const linkTo1("editor", "diagram", "element", "linkTo1");
	Id const linkTo2("editor", "diagram", "element", "linkTo2");
	Id const linkFrom1("editor", "diagram", "element", "linkFrom1");
	Id const linkFrom2("editor", "diagram", "element", "linkFrom2");
	Id const link("editor", "diagram", "element", "link");

	IdList to;
	IdList from;
	IdList empty;
	to << linkTo1 << linkTo2;
	from << linkFrom1 << linkFrom2;
	empty << link;

	ASSERT_EQ(mClient->temporaryRemovedLinks(root).size(), 0);
	EXPECT_THROW(mClient->temporaryRemovedLinks(notExistingId), Exception);

	mClient->setTemporaryRemovedLinks(root, "to", to);
	mClient->setTemporaryRemovedLinks(root, "from", from);
	mClient->setTemporaryRemovedLinks(root, QString(), empty);
	EXPECT_THROW(mClient->setTemporaryRemovedLinks(notExistingId, "to", to), Exception);

	mClient->setProperty(root, "to", qReal::IdListHelper::toVariant(to));
	mClient->setProperty(root, "from", qReal::IdListHelper::toVariant(from));
	mClient->setProperty(root, QString(), qReal::IdListHelper::toVariant(empty));

	ASSERT_EQ(mClient->temporaryRemovedLinks(root).size(), 5);
	EXPECT_TRUE(mClient->temporaryRemovedLinks(root).contains(linkTo1));
	EXPECT_TRUE(mClient->temporaryRemovedLinks(root).contains(linkTo2));
	EXPECT_TRUE(mClient->temporaryRemovedLinks(root).contains(linkFrom1));
	EXPECT_TRUE(mClient->temporaryRemovedLinks(root).contains(linkFrom2));
	EXPECT_TRUE(mClient->temporaryRemovedLinks(root).contains(link));

	ASSERT_EQ(mClient->temporaryRemovedLinksAt(root, "to").size(), 2);
	EXPECT_TRUE(mClient->temporaryRemovedLinksAt(root, "to").contains(linkTo1));
	EXPECT_TRUE(mClient->temporaryRemovedLinksAt(root, "to").contains(linkTo2));

	ASSERT_EQ(mClient->temporaryRemovedLinksAt(root, "from").size(), 2);
	EXPECT_TRUE(mClient->temporaryRemovedLinksAt(root, "from").contains(linkFrom1));
	EXPECT_TRUE(mClient->temporaryRemovedLinksAt(root, "from").contains(linkFrom2));

	ASSERT_EQ(mClient->temporaryRemovedLinksAt(root, QString()).size(), 1);
	EXPECT_TRUE(mClient->temporaryRemovedLinksAt(root, QString()).contains(link));

	mClient->removeTemporaryRemovedLinks(root);
	ASSERT_FALSE(mClient->hasProperty(root, "to"));
	ASSERT_FALSE(mClient->hasProperty(root, "from"));
	ASSERT_FALSE(mClient->hasProperty(root, QString()));
}

TEST_F(ClientTest, saveAllTest) {
	mClient->remove(root);
	mClient->saveAll();
	mClient->open("saveFile.qrs");

	EXPECT_FALSE(mClient->exist(root));
	EXPECT_TRUE(mClient->exist(parent));
	EXPECT_TRUE(mClient->exist(child1));
	EXPECT_TRUE(mClient->exist(child2));
	EXPECT_TRUE(mClient->exist(child3));
	EXPECT_TRUE(mClient->exist(child1_child));
	EXPECT_TRUE(mClient->exist(child2_child));
	EXPECT_TRUE(mClient->exist(child3_child));
}

TEST_F(ClientTest, saveTest) {
	IdList toSave;
	toSave << child1 << child2 << child3;
	mClient->save(toSave);
	mClient->open("saveFile.qrs");

	EXPECT_EQ(mClient->elements().size(), 7);
	EXPECT_TRUE(mClient->exist(child1));
	EXPECT_TRUE(mClient->exist(child2));
	EXPECT_TRUE(mClient->exist(child3));
	EXPECT_TRUE(mClient->exist(child1_child));
	EXPECT_TRUE(mClient->exist(child2_child));
	EXPECT_TRUE(mClient->exist(child3_child));
}

TEST_F(ClientTest, saveWithLogicalIdTest) {
	IdList toSave;
	toSave << child1 << child3_child;
	mClient->saveWithLogicalId(toSave);
	mClient->open("saveFile.qrs");

	EXPECT_EQ(mClient->elements().size(), 5);
	EXPECT_TRUE(mClient->exist(child1));
	EXPECT_TRUE(mClient->exist(child1_child));
	EXPECT_TRUE(mClient->exist(child2_child));
	EXPECT_TRUE(mClient->exist(child3_child));
}

TEST_F(ClientTest, saveDiagramsByIdTest) {
	IdList toSave;
	toSave << child1;

	QHash<QString, IdList> diagramIds;
	diagramIds.insert("diagram1", toSave);

	mClient->saveDiagramsById(diagramIds);

	mSerializer->decompressFile("diagram1.qrs");

	EXPECT_TRUE(QFile::exists("../unsaved/tree/graphical/editor1/diagram2/element3/child1"));
	EXPECT_TRUE(QFile::exists("../unsaved/tree/graphical/editor2/diagram3/element5/child1_child"));
	EXPECT_TRUE(QFile::exists("../unsaved/tree/graphical/editor2/diagram4/element6/child2_child"));
	EXPECT_FALSE(QFile::exists("../unsaved/tree/graphical/editor1/diagram1/element2/root"));
	EXPECT_FALSE(QDir().exists("../unsaved/tree/logical"));

	QFile::remove("diagram1.qrs");
}
