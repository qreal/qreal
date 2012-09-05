#include <QtCore/QFile>

#include "../../../qrutils/outFile.h"

#include "gtest/gtest.h"

TEST(OutFileTest, writeTest) {
	utils::OutFile outFile("testFile");
	
	outFile() << "test text";
	outFile().flush();

	QFile file("testFile");
	file.open(QIODevice::ReadOnly);
	QString dataWritten = QString(file.readAll());

	EXPECT_EQ(dataWritten, "test text");

	QFile::remove("testFile");
}

