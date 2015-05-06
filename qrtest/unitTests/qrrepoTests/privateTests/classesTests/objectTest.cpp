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

#include "../../../qrrepo/private/classes/object.h"

#include <gtest/gtest.h>

#include "../../../qrrepo/private/classes/logicalObject.h"
#include "../../../qrrepo/private/classes/graphicalObject.h"
#include "../../../qrkernel/exception/exception.h"

using namespace qReal;
using namespace qrRepo::details;

TEST(ObjectTest, contructorsTest)
{
	Id const id("editor", "diagram", "element", "id");
	Id const logicalId("editor", "diagram", "element", "logicalId");
	Id const parentId("editor", "diagram", "element", "parentId");

	qrRepo::details::LogicalObject obj1(id);

	qrRepo::details::LogicalObject obj2(id);
	obj2.setParent(parentId);

	qrRepo::details::GraphicalObject obj3(id, parentId, logicalId);

	EXPECT_EQ(obj1.id(), id);
	EXPECT_EQ(obj2.id(), id);
	EXPECT_EQ(obj2.parent(), parentId);
	EXPECT_EQ(obj3.id(), id);
	EXPECT_EQ(obj3.parent(), parentId);
	EXPECT_EQ(obj3.logicalId(), logicalId);
}

TEST(ObjectTest, setParentTest)
{
	Id const id("editor", "diagram", "element", "id");
	Id const parent1("editor", "diagram", "element", "parent1");
	Id const parent2("editor", "diagram", "element", "parent2");

	qrRepo::details::LogicalObject obj(id);
	obj.setParent(parent2);

	EXPECT_EQ(obj.parent(), parent2);
}

TEST(ObjectTest, addAndRemoveChildAndChildrenTest)
{
	Id const id("editor", "diagram", "element", "id");
	Id const child1("editor", "diagram", "element", "child1");
	Id const child2("editor", "diagram", "element", "child2");
	Id const child3("editor", "diagram", "element", "child3");
	qrRepo::details::LogicalObject obj(id);

	obj.addChild(child1);
	obj.addChild(child2);
	obj.addChild(child3);

	ASSERT_EQ(obj.children().size(), 3);
	EXPECT_TRUE(obj.children().contains(child1));
	EXPECT_TRUE(obj.children().contains(child2));
	EXPECT_TRUE(obj.children().contains(child3));

	obj.removeChild(child2);

	ASSERT_EQ(obj.children().size(), 2);
	EXPECT_TRUE(obj.children().contains(child1));
	EXPECT_TRUE(obj.children().contains(child3));

	EXPECT_THROW(obj.removeChild(child2), Exception);

	EXPECT_THROW(obj.addChild(child1), Exception);
}

TEST(ObjectTest, propertiesHasAndSetOperationsTest)
{
	Id const id("editor", "diagram", "element", "id");
	qrRepo::details::LogicalObject obj(id);

	obj.setProperty("property1", "value1");

	ASSERT_TRUE(obj.hasProperty("property1"));
	EXPECT_TRUE(obj.hasProperty("pRoPeRty1"));
	EXPECT_FALSE(obj.hasProperty("pRoPeRty1", true));
	EXPECT_TRUE(obj.hasProperty("proper.*", false, true));
	EXPECT_FALSE(obj.hasProperty("proper.*cc", false, true));
	EXPECT_FALSE(obj.hasProperty("property2"));

	EXPECT_EQ(obj.property("property1").toString(), "value1");
	obj.setProperty("property1", "value2");
	EXPECT_EQ(obj.property("property1").toString(), "value2");

	QMap<QString, QVariant> properties;
	properties.insert("property2", "val2");
	properties.insert("property3", "val3");
	obj.setProperties(properties);

	ASSERT_FALSE(obj.hasProperty("property1"));
	ASSERT_TRUE(obj.hasProperty("property2"));
	ASSERT_TRUE(obj.hasProperty("property3"));
	EXPECT_EQ(obj.property("property2").toString(), "val2");
	EXPECT_EQ(obj.property("property3").toString(), "val3");

//	::testing::FLAGS_gtest_death_test_style = "threadsafe";
//	EXPECT_DEATH_IF_SUPPORTED(obj.setProperty("prop", QVariant());
//			, ".*Empty QVariant set as a property.*");

}

TEST(ObjectTest, propertiesGetAndRemoveTest)
{
	Id const id("editor", "diagram", "element", "id");
	qrRepo::details::LogicalObject obj(id);

	obj.setProperty("property1", "value1");
	obj.setProperty("property2", "value2");
	obj.setProperty("property3", "value3");

	EXPECT_EQ(obj.property("property1").toString(), "value1");
	EXPECT_EQ(obj.property("property2").toString(), "value2");
	EXPECT_EQ(obj.property("property3").toString(), "value3");
	EXPECT_EQ(obj.property("backReferences"), QVariant());
	// disabled throwing exceptions here until we get some kind of model migration tool
	//EXPECT_THROW(obj.property("property4"), Exception);

	obj.removeProperty("property2");

	ASSERT_FALSE(obj.hasProperty("property2"));
	// EXPECT_THROW(obj.removeProperty("property4"), Exception);

	QMap<QString, QVariant> properties = obj.properties();
	ASSERT_EQ(properties.keys().size(), 2);
	ASSERT_TRUE(properties.contains("property1"));
	ASSERT_TRUE(properties.contains("property3"));
	EXPECT_EQ(properties.value("property1").toString(), "value1");
	EXPECT_EQ(properties.value("property3").toString(), "value3");

	obj.removeProperty("property3");
	QMapIterator<QString, QVariant> iterator = obj.propertiesIterator();
	iterator.next();
	EXPECT_EQ(iterator.key(), "property1");
	EXPECT_EQ(iterator.value(), "value1");
}

TEST(ObjectTest, backReferencesTest)
{
	Id const id("editor", "diagram", "element", "id");
	Id const backReference1("editor", "diagram", "element", "backReference1");
	Id const backReference2("editor", "diagram", "element", "backReference2");
	Id const backReference3("editor", "diagram", "element", "backReference3");
	qrRepo::details::LogicalObject obj(id);

	EXPECT_EQ(obj.property("backReferences"), QVariant());
	EXPECT_THROW(obj.removeBackReference(backReference1), Exception);

	obj.setBackReference(backReference1);
	EXPECT_THROW(obj.removeBackReference(backReference2), Exception);

	obj.setBackReference(backReference2);
	obj.setBackReference(backReference2);
	obj.setBackReference(backReference3);
	obj.removeBackReference(backReference2);

	IdList references = obj.property("backReferences").value<IdList>();
	ASSERT_EQ(references.size(), 3);
	EXPECT_TRUE(references.contains(backReference1));
	EXPECT_TRUE(references.contains(backReference3));
}

TEST(ObjectTest, temporaryRemovedLinksTest)
{
	Id const id("editor", "diagram", "element", "id");
	qrRepo::details::LogicalObject obj(id);

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

	ASSERT_EQ(obj.temporaryRemovedLinks().size(), 0);

	obj.setTemporaryRemovedLinks("to", to);
	obj.setTemporaryRemovedLinks("from", from);
	obj.setTemporaryRemovedLinks(QString(), empty);

	obj.setProperty("to", qReal::IdListHelper::toVariant(to));
	obj.setProperty("from", qReal::IdListHelper::toVariant(from));
	obj.setProperty(QString(), qReal::IdListHelper::toVariant(empty));

	ASSERT_EQ(obj.temporaryRemovedLinks().size(), 5);
	EXPECT_TRUE(obj.temporaryRemovedLinks().contains(linkTo1));
	EXPECT_TRUE(obj.temporaryRemovedLinks().contains(linkTo2));
	EXPECT_TRUE(obj.temporaryRemovedLinks().contains(linkFrom1));
	EXPECT_TRUE(obj.temporaryRemovedLinks().contains(linkFrom2));
	EXPECT_TRUE(obj.temporaryRemovedLinks().contains(link));

	ASSERT_EQ(obj.temporaryRemovedLinksAt("to").size(), 2);
	EXPECT_TRUE(obj.temporaryRemovedLinksAt("to").contains(linkTo1));
	EXPECT_TRUE(obj.temporaryRemovedLinksAt("to").contains(linkTo2));

	ASSERT_EQ(obj.temporaryRemovedLinksAt("from").size(), 2);
	EXPECT_TRUE(obj.temporaryRemovedLinksAt("from").contains(linkFrom1));
	EXPECT_TRUE(obj.temporaryRemovedLinksAt("from").contains(linkFrom2));

	ASSERT_EQ(obj.temporaryRemovedLinksAt(QString()).size(), 1);
	EXPECT_TRUE(obj.temporaryRemovedLinksAt(QString()).contains(link));

	obj.removeTemporaryRemovedLinksAt("to");
	ASSERT_FALSE(obj.hasProperty("to"));
	ASSERT_TRUE(obj.hasProperty("from"));
	ASSERT_TRUE(obj.hasProperty(QString()));

	obj.removeTemporaryRemovedLinks();
	ASSERT_FALSE(obj.hasProperty("to"));
	ASSERT_FALSE(obj.hasProperty("from"));
	ASSERT_FALSE(obj.hasProperty(QString()));
}

TEST(ObjectTest, stackBeforeTest)
{
	Id const id("editor", "diagram", "element", "id");
	Id const child1("editor", "diagram", "element", "child1");
	Id const child2("editor", "diagram", "element", "child2");
	Id const child3("editor", "diagram", "element", "child3");
	Id const child4("editor", "diagram", "element", "child4");
	Id const child5("editor", "diagram", "element", "child5");
	Id const child6("editor", "diagram", "element", "child6");
	qrRepo::details::LogicalObject obj(id);

	obj.addChild(child1);
	obj.addChild(child2);
	obj.addChild(child3);
	obj.addChild(child4);
	obj.addChild(child5);

	EXPECT_THROW(obj.stackBefore(child6, child1), Exception);
	EXPECT_THROW(obj.stackBefore(child1, child6), Exception);

	obj.stackBefore(child4, child2);
	ASSERT_EQ(obj.children().indexOf(child4) + 1, obj.children().indexOf(child2));

	obj.stackBefore(child1, child5);
	ASSERT_EQ(obj.children().indexOf(child1) + 1, obj.children().indexOf(child5));
}

TEST(ObjectTest, cloneTest)
{
	Id const parent("editor", "diagram", "element", "parent");
	Id const root("editor", "diagram", "element", "root");
	Id const child1("editor", "diagram", "element", "child1");
	Id const child2("editor", "diagram", "element", "child2");
	Id const child3("editor", "diagram", "element", "child3");
	Id const child1_child("editor", "diagram", "element", "child1_child");
	Id const child2_child("editor", "diagram", "element", "child2_child");
	Id const child3_child("editor", "diagram", "element", "child3_child");

	qrRepo::details::LogicalObject parentObj(parent);
	qrRepo::details::LogicalObject rootObj(root);
	qrRepo::details::LogicalObject child1Obj(child1);
	qrRepo::details::LogicalObject child2Obj(child2);
	qrRepo::details::LogicalObject child3Obj(child3);
	qrRepo::details::LogicalObject child1_childObj(child1_child);
	qrRepo::details::LogicalObject child2_childObj(child2_child);
	qrRepo::details::LogicalObject child3_childObj(child3_child);

	rootObj.setProperty("testProperty1", "value1");
	child3_childObj.setProperty("testProperty2", "value2");

	QHash<Id, qrRepo::details::Object *> objHash;
	objHash.insert(parent, &parentObj);
	objHash.insert(root, &rootObj);
	objHash.insert(child1, &child1Obj);
	objHash.insert(child2, &child2Obj);
	objHash.insert(child3, &child3Obj);
	objHash.insert(child1_child, &child1_childObj);
	objHash.insert(child2_child, &child2_childObj);
	objHash.insert(child3_child, &child3_childObj);

	parentObj.addChild(root);
	rootObj.addChild(child1);
	rootObj.addChild(child2);
	rootObj.addChild(child3);
	child1Obj.addChild(child1_child);
	child2Obj.addChild(child2_child);
	child3Obj.addChild(child3_child);

	qrRepo::details::Object * const cloned = rootObj.clone(objHash);
	cloned->setParent(parent);

	ASSERT_EQ(objHash.keys().size(), 15);

	ASSERT_EQ(cloned->parent(), parent);
	ASSERT_TRUE(cloned != &rootObj);
	ASSERT_TRUE(cloned->id() != root);
	ASSERT_EQ(cloned->id().type(), root.type());

	ASSERT_EQ(cloned->children().size(), 3);
	Id clonedChildId1 = cloned->children().at(0);
	Id clonedChildId2 = cloned->children().at(1);
	Id clonedChildId3 = cloned->children().at(2);

	EXPECT_EQ(clonedChildId1.type(), child1.type());
	EXPECT_EQ(clonedChildId2.type(), child2.type());
	EXPECT_EQ(clonedChildId3.type(), child3.type());

	EXPECT_TRUE(clonedChildId1 != child1);
	EXPECT_TRUE(clonedChildId2 != child2);
	EXPECT_TRUE(clonedChildId3 != child3);

	ASSERT_TRUE(objHash.contains(clonedChildId1));
	ASSERT_TRUE(objHash.contains(clonedChildId2));
	ASSERT_TRUE(objHash.contains(clonedChildId3));

	EXPECT_TRUE(objHash.value(clonedChildId1) != &child1Obj);
	EXPECT_TRUE(objHash.value(clonedChildId2) != &child2Obj);
	EXPECT_TRUE(objHash.value(clonedChildId3) != &child3Obj);

	ASSERT_EQ(objHash.value(clonedChildId1)->children().size(), 1);
	ASSERT_EQ(objHash.value(clonedChildId2)->children().size(), 1);
	ASSERT_EQ(objHash.value(clonedChildId3)->children().size(), 1);

	Id clonedChildChildId1 = objHash[clonedChildId1]->children().at(0);
	Id clonedChildChildId2 = objHash[clonedChildId2]->children().at(0);
	Id clonedChildChildId3 = objHash[clonedChildId3]->children().at(0);

	EXPECT_EQ(clonedChildChildId1.type(), child1_child.type());
	EXPECT_EQ(clonedChildChildId2.type(), child2_child.type());
	EXPECT_EQ(clonedChildChildId3.type(), child3_child.type());

	EXPECT_TRUE(clonedChildChildId1 != child1_child);
	EXPECT_TRUE(clonedChildChildId2 != child2_child);
	EXPECT_TRUE(clonedChildChildId3 != child3_child);
	EXPECT_TRUE(objHash.value(clonedChildChildId1) != &child1_childObj);
	EXPECT_TRUE(objHash.value(clonedChildChildId2) != &child2_childObj);
	EXPECT_TRUE(objHash.value(clonedChildChildId3) != &child3_childObj);

	ASSERT_TRUE(cloned->hasProperty("testProperty1"));
	ASSERT_TRUE(objHash.value(clonedChildChildId3)->hasProperty("testProperty2"));

	EXPECT_EQ(cloned->property("testProperty1").toString(), "value1");
	EXPECT_EQ(objHash.value(clonedChildChildId3)->property("testProperty2").toString(), "value2");
}

TEST(ObjectTest, replacePropertiesTest)
{
	Id const id("editor", "diagram", "element", "id");
	qrRepo::details::LogicalObject obj(id);

	obj.setProperty("testProperty1", "value1");
	obj.setProperty("property", "val");
	obj.setProperty("property_test2", "value2");
	obj.replaceProperties("value", "replace_value");

	EXPECT_EQ(obj.property("testProperty1").toString(), "replace_value");
	EXPECT_EQ(obj.property("property_test2").toString(), "replace_value");
	EXPECT_EQ(obj.property("property").toString(), "val");
}
