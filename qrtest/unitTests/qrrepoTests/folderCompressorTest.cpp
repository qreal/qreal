#include <QtCore/QFile>
#include <QtCore/QDir>

#include "folderCompressorTest.h"

using namespace qrTest;

void FolderCompressorTest::removeDirectory(QString const &dirName)
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

void FolderCompressorTest::SetUp() {
	mFolderCompressor = new FolderCompressor();

	QDir().mkpath("temp/dir1/dir2");
	QDir().mkdir("temp/dir3");

	QFile file1("temp/file1");
	QFile file2("temp/dir1/dir2/file2");
	QFile file3("temp/dir3/file3");

	file1.open(QIODevice::ReadWrite);
	file2.open(QIODevice::ReadWrite);
	file3.open(QIODevice::ReadWrite);

	file1.write("text1");
	file2.write("text2");
	file3.write("text3");
}

void FolderCompressorTest::TearDown() {
	delete mFolderCompressor;
	removeDirectory("temp");
	removeDirectory("temp_decompessed");
	QFile::remove("compressed");
}

TEST_F(FolderCompressorTest, decompressTest) {
	mFolderCompressor->compressFolder("temp", "compressed");
	mFolderCompressor->decompressFolder("compressed", "temp_decompessed");

	ASSERT_TRUE(QFile::exists("temp_decompessed/file1"));
	ASSERT_TRUE(QFile::exists("temp_decompessed/dir1/dir2/file2"));
	ASSERT_TRUE(QFile::exists("temp_decompessed/dir3/file3"));

	QFile file1("temp_decompessed/file1");
	QFile file2("temp_decompessed/dir1/dir2/file2");
	QFile file3("temp_decompessed/dir3/file3");

	file1.open(QIODevice::ReadWrite);
	file2.open(QIODevice::ReadWrite);
	file3.open(QIODevice::ReadWrite);

	QString line1(file1.readLine());
	QString line2(file2.readLine());
	QString line3(file3.readLine());

	EXPECT_EQ(line1, "text1");
	EXPECT_EQ(line2, "text2");
	EXPECT_EQ(line3, "text3");
}
