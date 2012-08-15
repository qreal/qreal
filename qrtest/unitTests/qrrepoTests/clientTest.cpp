#include <QtCore/QFile>
#include <QtCore/QDir>

#include "clientTest.h"
#include "../../../qrrepo/private/classes/object.h"

using namespace qrRepo;
using namespace details;
using namespace qReal;
using namespace qrTest;

Id const clientTestId1("editor1", "diagram1", "element1", "id1");
Id const clientTestId2("editor1", "diagram2", "element2", "id2");
Id const clientTestId3("editor2", "diagram3", "element3", "id3");

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

	Object obj1(clientTestId1);
	obj1.setProperty("property1", "value1");

	Object obj2(clientTestId2);
	obj2.setProperty("property2", "value2");

	Object obj3(clientTestId3);
	obj3.setProperty("property3", "val3");

	QList<Object *> list;
	list.push_back(&obj1);
	list.push_back(&obj2);
	list.push_back(&obj3);

	mSerializer->saveToDisk(list);

	mClient = new Client("saveFile.qrs");
}

void ClientTest::TearDown() {
	delete mClient;

	mSerializer->clearWorkingDir();
	delete mSerializer;

	QFile::remove("saveFile.qrs");
}

TEST_F(ClientTest, loadFromDiskTest) {
	QString const value = "value";
	IdList const list = mClient->elementsByPropertyContent(value, false, false);

	EXPECT_EQ(list.length(), 2);

	EXPECT_TRUE(list.contains(clientTestId1));
	EXPECT_TRUE(list.contains(clientTestId2));
	EXPECT_FALSE(list.contains(clientTestId3));
}

