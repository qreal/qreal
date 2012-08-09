#include <QtCore/QFile>
#include <QtCore/QDir>

#include "serializerTest.h"
#include "../../qrrepo/private/classes/object.h"

using namespace qrRepo;
using namespace details;
using namespace qReal;
using namespace qrTest;

void SerializerTest::removeDirectory(QString const &dirName)
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

void SerializerTest::SetUp() {
	mSerializer = new Serializer("saveFile");
}

void SerializerTest::TearDown() {
	mSerializer->clearWorkingDir();
	delete mSerializer;

	QFile::remove("saveFile.qrs");
}

TEST_F(SerializerTest, loadFromDiskTest) {
	Id const id1("editor1", "diagram1", "element1", "id1");
	Object obj1(id1);
	obj1.setProperty("property1", "value1");

	Id const id2("editor1", "diagram2", "element2", "id2");
	Object obj2(id2);
	obj2.setProperty("property2", "value2");

	QList<Object *> list;
	list.push_back(&obj1);
	list.push_back(&obj2);
	
	mSerializer->saveToDisk(list);

	QHash<Id, Object *> map;
	mSerializer->setWorkingFile("saveFile.qrs");
	mSerializer->loadFromDisk(map);

	ASSERT_TRUE(map.contains(id1));
	ASSERT_TRUE(map.contains(id2));
	
	ASSERT_TRUE(map.value(id1)->hasProperty("property1"));
	ASSERT_TRUE(map.value(id2)->hasProperty("property2"));

	EXPECT_EQ(map.value(id1)->property("property1").toString(), "value1");
	EXPECT_EQ(map.value(id2)->property("property2").toString(), "value2");
}
