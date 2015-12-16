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

#include "folderCompressorTest.h"

using namespace qrTest;
using namespace qrRepo::details;

void FolderCompressorTest::removeDirectory(QString const &dirName)
{
	const QDir dir(dirName);

	for (QFileInfo info : dir.entryInfoList(
			 QDir::Hidden | QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::DirsFirst))
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
	removeDirectory("temp");
	removeDirectory("temp_decompessed");
	QFile::remove("compressed");
}

TEST_F(FolderCompressorTest, decompressTest) {
	FolderCompressor::compressFolder("temp", "compressed");
	FolderCompressor::decompressFolder("compressed", "temp_decompessed");

	ASSERT_TRUE(QFile::exists("temp_decompessed/file1"));
	ASSERT_TRUE(QFile::exists("temp_decompessed/dir1/dir2/file2"));
	ASSERT_TRUE(QFile::exists("temp_decompessed/dir3/file3"));

	QFile file1("temp_decompessed/file1");
	QFile file2("temp_decompessed/dir1/dir2/file2");
	QFile file3("temp_decompessed/dir3/file3");

	file1.open(QIODevice::ReadWrite);
	file2.open(QIODevice::ReadWrite);
	file3.open(QIODevice::ReadWrite);

	const QString line1(file1.readLine());
	const QString line2(file2.readLine());
	const QString line3(file3.readLine());

	EXPECT_EQ(line1, "text1");
	EXPECT_EQ(line2, "text2");
	EXPECT_EQ(line3, "text3");
}
