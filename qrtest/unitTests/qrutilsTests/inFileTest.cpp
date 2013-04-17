#include <QtCore/QFile>

#include "../../../qrutils/inFile.h"

#include "gtest/gtest.h"

TEST(InFileTest, readAllTest) {
	QFile file("testFile");
	file.open(QIODevice::ReadWrite);

	file.write("text1\n");
	file.write("text2\n");
	file.write("text3\n");
	file.close();

	QString dataRead = utils::InFile::readAll("testFile");

	EXPECT_EQ(dataRead, "text1\ntext2\ntext3\n");

	QFile::remove("testFile");
}
