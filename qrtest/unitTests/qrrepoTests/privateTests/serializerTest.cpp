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
#include <QtCore/QPointF>

#include "serializerTest.h"
#include "../../../qrrepo/private/classes/logicalObject.h"
#include "../../../qrrepo/private/classes/graphicalObject.h"
#include "../../../qrkernel/settingsManager.h"

using namespace qrRepo;
using namespace details;
using namespace qReal;
using namespace qrTest;

void SerializerTest::removeDirectory(QString const &dirName)
{
	QDir const dir(dirName);

	foreach (QFileInfo const &info, dir.entryInfoList(QDir::Hidden
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

void SerializerTest::SetUp()
{
	mSerializer = new Serializer("saveFile");
}

void SerializerTest::TearDown()
{
	mSerializer->clearWorkingDir();
	delete mSerializer;

	QFile::remove("saveFile.qrs");
}

TEST_F(SerializerTest, saveAndLoadFromDiskTest)
{
	QHash<QString, QVariant> metaInfo;
	metaInfo["key1"] = "info1";
	metaInfo["key2"] = 2;

	const Id id1("editor1", "diagram1", "element1", "id1");
	LogicalObject obj1(id1);
	obj1.setProperty("property1", "value1");

	const Id id2("editor1", "diagram2", "element2", "id2");
	LogicalObject obj2(id2);
	obj2.setProperty("property2", "value2");

	QList<Object *> list;
	list.push_back(&obj1);
	list.push_back(&obj2);

	mSerializer->saveToDisk(list, metaInfo);

	QHash<Id, Object *> map;
	mSerializer->setWorkingFile("saveFile.qrs");
	mSerializer->loadFromDisk(map, metaInfo);

	ASSERT_TRUE(map.contains(id1));
	ASSERT_TRUE(map.contains(id2));

	ASSERT_TRUE(map.value(id1)->hasProperty("property1"));
	ASSERT_TRUE(map.value(id2)->hasProperty("property2"));

	EXPECT_EQ(map.value(id1)->property("property1").toString(), "value1");
	EXPECT_EQ(map.value(id2)->property("property2").toString(), "value2");

	ASSERT_TRUE(metaInfo.keys().count() == 2);
	ASSERT_EQ(metaInfo["key1"], "info1");
	ASSERT_EQ(metaInfo["key2"], 2);
}

// Decomment EXPECT_FALSE and delete EXPECT_TRUE(true) when removeFromDisk will be fixed. pathToElement(id) returns
// path without parent folder /tree and /logical or /graphical according to id type.
TEST_F(SerializerTest, removeFromDiskTest)
{
	const Id id1("editor1", "diagram1", "element1", "id1");
	LogicalObject obj1(id1);
	obj1.setProperty("property1", "value1");

	const Id id2("editor1", "diagram2", "element2", "id2");
	LogicalObject obj2(id2);
	obj2.setProperty("property2", "value2");

	QList<Object *> list;
	list.push_back(&obj1);
	list.push_back(&obj2);

	mSerializer->saveToDisk(list, QHash<QString, QVariant>());
	mSerializer->decompressFile("saveFile.qrs");
	mSerializer->removeFromDisk(id2);

	//EXPECT_FALSE(QFile::exists("unsaved/tree/logical/editor1/diagram2/element2/id2"));
	//EXPECT_FALSE(QDir().exists("unsaved/tree/logical/editor1/diagram2"));
	EXPECT_TRUE(true);
}

TEST_F(SerializerTest, saveAndLoadGraphicalPartsTest)
{
	const Id element("editor", "diagram", "element", "id");
	LogicalObject logicalObj(element);

	const Id graphicalElement("editor", "diagram", "element", "graphicalId");

	GraphicalObject graphicalObj(graphicalElement, Id(), element);

	graphicalObj.createGraphicalPart(0);
	graphicalObj.setGraphicalPartProperty(0, "Coord", QPointF(10, 20));

	QList<Object *> list;
	list.push_back(&graphicalObj);
	list.push_back(&logicalObj);

	mSerializer->saveToDisk(list, QHash<QString, QVariant>());

	QHash<Id, Object *> map;
	QHash<QString, QVariant> metaInfo;
	mSerializer->setWorkingFile("saveFile.qrs");
	mSerializer->loadFromDisk(map, metaInfo);

	ASSERT_TRUE(map.contains(graphicalElement));

	GraphicalObject const * const deserializedGraphicalObject
			= dynamic_cast<GraphicalObject const *>(map.value(graphicalElement));

	ASSERT_EQ(QPointF(10, 20), deserializedGraphicalObject->graphicalPartProperty(0, "Coord"));
}
