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

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QTranslator>

#include "mainClass.h"

using namespace editorPluginTestingFramework;

void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			fprintf(stdout, "Debug: %s\n", msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n", msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", msg);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", msg);
			abort();
		case QtInfoMsg:
			fprintf(stderr, "Warning: %s\n", msg);
			break;
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QTranslator appTranslator;
	if (!app.arguments().contains("--no-locale")) {
		appTranslator.load(":/editorPluginTestingFramework_" + QLocale::system().name());
		app.installTranslator(&appTranslator);
	}

	if (argc != 3) {
		qDebug() << "Usage: editorPluginTestTool fileName.qrs configurationFile.xml";
		return 1;
	}

	const QString workingCopyDir = argv[1];
	const QString &configurationFileName = argv[2];
	MainClass newMainClass(workingCopyDir, qApp->applicationDirPath() + "/../../qrmc/"
			, qApp->applicationDirPath(), configurationFileName);
	return newMainClass.travisTestResult();
}

