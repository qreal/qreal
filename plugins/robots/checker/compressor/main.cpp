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

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "folderCompressor.h"

using namespace compressor;

const QString description = QObject::tr("Utility for decompression or compression of QReal save files.");

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("Compressor");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription(description);
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addPositionalArgument("source"
			, QObject::tr("Source file to be decompressed or directory to be compressed."));

	parser.process(app);

	const QStringList positionalArgs = parser.positionalArguments();
	if (positionalArgs.size() != 1) {
		parser.showHelp();
	}

	const QString source = positionalArgs[0];

	QFileInfo sourceFileInfo(source);

	bool success = false;

	if (sourceFileInfo.isDir()) {
		const QString target = sourceFileInfo.fileName() + ".qrs";
		success = FolderCompressor::compressFolder(source, target);
	} else if (sourceFileInfo.isFile()) {
		const QString target = sourceFileInfo.baseName();
		success = FolderCompressor::decompressFolder(source, target);
	} else {
		qDebug() << QObject::tr("Incorrect argument.");
	}

	return success ? 0 : 1;
}
