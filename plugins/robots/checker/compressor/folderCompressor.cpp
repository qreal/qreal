/* Copyright 2015 CyberTech Labs Ltd.
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

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>

using namespace compressor;

bool FolderCompressor::compressFolder(const QString &sourceFolder, const QString &destinationFile)
{
	const QDir src(sourceFolder);
	if (!src.exists()) {
		qDebug() << QObject::tr("Source folder does not exist, aborting.");
		return false;
	}

	QFile target(destinationFile);
	if (!target.open(QIODevice::WriteOnly)) {
		qDebug() << QObject::tr("Failed to open target file, aborting.");
		return false;
	}

	QDataStream dataStream(&target);

	const bool success = compress(sourceFolder, "", dataStream);
	target.close();

	return success;
}

bool FolderCompressor::decompressFolder(const QString &sourceFile, const QString &destinationFolder)
{
	QFile src(sourceFile);
	if (!src.exists()) {
		qDebug() << QObject::tr("Source file does not exist, aborting.");
		return false;
	}

	QDir dir;
	if (!dir.mkpath(destinationFolder)) {
		qDebug() << QObject::tr("Failed to create target folder, aborting.");
		return false;
	}

	if (!src.open(QIODevice::ReadOnly)) {
		qDebug() << QObject::tr("Failed to open source file, aborting.");
		return false;
	}

	QDataStream dataStream(&src);

	while (!dataStream.atEnd()) {
		QString fileName;
		QByteArray data;

		// extract mFile name and data in order
		dataStream >> fileName >> data;

		// create any needed folder
		QString subfolder;
		for (int i = fileName.length() - 1; i > 0; --i) {
			if (fileName.at(i) == '\\' || fileName.at(i) == '/') {
				subfolder = fileName.left(i);
				dir.mkpath(destinationFolder + "/" + subfolder);
				break;
			}
		}

		QFile outFile(destinationFolder + "/" + fileName);
		if (!outFile.open(QIODevice::WriteOnly)) {
			qDebug() << QObject::tr("Failed to create one of target files, aborting.");
			src.close();
			return false;
		}

		outFile.write(qUncompress(data));
		outFile.close();
	}

	src.close();
	return true;
}

bool FolderCompressor::compress(const QString &sourceFolder, const QString &prefix, QDataStream &dataStream)
{
	QDir dir(sourceFolder);
	if (!dir.exists()) {
		return false;
	}

	// 1 - list all folders inside the current folder
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
	const QFileInfoList foldersList = dir.entryInfoList();

	// 2 - For each folder in list: call the same function with folders' paths
	for (const QFileInfo file : foldersList) {
		const QString folderName = file.fileName();
		const QString folderPath = dir.absolutePath() + "/" + folderName;
		const QString newPrefix = prefix + "/" + folderName;

		compress(folderPath, newPrefix, dataStream);
	}

	// 3 - List all files inside the current folder
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	const QFileInfoList filesList = dir.entryInfoList();

	// 4- For each file in list: add file path and compressed binary data
	for (const QFileInfo fileInfo : filesList) {
		QFile file(dir.absolutePath() + "/" + fileInfo.fileName());
		if (!file.open(QIODevice::ReadOnly)) {
			return false;
		}

		dataStream << QString(prefix + "/" + fileInfo.fileName());
		dataStream << qCompress(file.readAll());

		file.close();
	}

	return true;
}
