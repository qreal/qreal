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
#include <QtCore/QFile>

#include <qrrepo/repoApi.h>

const QString description = QObject::tr("Patcher for save files, replaces world model "
		"with contents of a given XML world model");

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("Patcher");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription(description);
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("save-file", QObject::tr("TRIK Studio save file to be patched."));
	parser.addPositionalArgument("field", QObject::tr("XML file with prepared 2D model field."));

	parser.process(app);

	const QStringList positionalArgs = parser.positionalArguments();
	if (positionalArgs.size() != 2) {
		parser.showHelp();
	}

	const QString saveFile = positionalArgs[0];
	const QString field = positionalArgs[1];

	qrRepo::RepoApi repo(saveFile);

	QFile fieldFile(field);
	if (!fieldFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return 1;
	}

	const QString fieldContents = fieldFile.readAll();

	fieldFile.close();

	repo.setMetaInformation("worldModel", fieldContents);
	repo.saveAll();

	return 0;
}
