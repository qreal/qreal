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

#include "folderCompressor.h"

bool FolderCompressor::compressFolder(const QString &sourceFolder, const QString &destinationFile)
{
	if (!QDir(sourceFolder).exists()) {
		return false;
	}

	QFile file(destinationFile);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QDataStream dataStream(&file);

	const bool result = compress(sourceFolder, "", dataStream);
	file.close();

	return result;
}

bool FolderCompressor::compress(const QString &sourceFolder, const QString &prefix, QDataStream &dataStream)
{
	QDir dir(sourceFolder);
	if (!dir.exists()) {
		return false;
	}

	// 1 - list all folders inside the current folder
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
	QFileInfoList foldersList = dir.entryInfoList();

	// 2 - For each folder in list: call the same function with folders' paths
	foreach (const QFileInfo &folder, foldersList) {
		QString folderName = folder.fileName();
		QString folderPath = dir.absolutePath() + "/" + folderName;
		QString newPrefix = prefix + "/" + folderName;
		compress(folderPath, newPrefix, dataStream);
	}

	// 3 - List all files inside the current folder
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	QFileInfoList filesList = dir.entryInfoList();

	// 4- For each mFile in list: add mFile path and compressed binary data
	foreach (const QFileInfo &fileInfo, filesList) {
		QFile file(dir.absolutePath() + "/" + fileInfo.fileName());
		if (!file.open(QIODevice::ReadOnly)) { // couldn't open file
			return false;
		}

		dataStream << QString(prefix + "/" + fileInfo.fileName());
		dataStream << qCompress(file.readAll());

		file.close();
	}

	return true;
}

bool FolderCompressor::decompressFolder(const QString &sourceFile, const QString &destinationFolder)
{
	if (!QFile(sourceFile).exists()) { // mFile not found, to handle later
		return false;
	}

	QDir dir;
	if (!dir.mkpath(destinationFolder)) { // could not create folder
		return false;
	}

	QFile file(sourceFile);
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}

	QDataStream dataStream(&file);

	while (!dataStream.atEnd()) {
		QString fileName;
		QByteArray data;

		dataStream >> fileName >> data; // extract file name and data in order

		QString subfolder; // create any needed folder
		for (int i = fileName.length() - 1; i > 0; i--) {
			if((QString(fileName.at(i)) == QString("\\"))
					|| (QString(fileName.at(i)) == QString("/")))
			{
				subfolder = fileName.left(i);
				dir.mkpath(destinationFolder+"/"+subfolder);
				break;
			}
		}

		QFile outFile(destinationFolder + "/" + fileName);
		if (!outFile.open(QIODevice::WriteOnly)) {
			file.close();
			return false;
		}
		outFile.write(qUncompress(data));
		outFile.close();
	}

	file.close();
	return true;
}

