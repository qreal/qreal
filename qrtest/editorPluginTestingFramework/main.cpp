/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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
#include <QtCore/QCommandLineParser>

#include "mainClass.h"

using namespace editorPluginTestingFramework;

static const QString description = QObject::tr(
		"Compares results of editor plugin generation from QRXC and QRMC, checks that the same metamodel "
		"loaded into metamodel interpreter produces the same results. Requires metamodel file and configuration"
		"file. Example:\n") +
		"    editorPluginTestingFramework fileName.qrs configurationFile.xml";

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
	Q_UNUSED(context)
	QByteArray localMsg = message.toLocal8Bit();
	switch (type) {
		case QtDebugMsg:
			fprintf(stdout, "Debug: %s\n", localMsg.constData());
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n", localMsg.constData());
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", localMsg.constData());
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", localMsg.constData());
			abort();
		default:
			fprintf(stderr, "Info: %s\n", localMsg.constData());
			break;
	}
}

int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageOutput);
	QCoreApplication app(argc, argv);

	QCoreApplication::setApplicationName("QReal Editor Plugin Testing Framework");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription(description);
	parser.addHelpOption();
	parser.addVersionOption();

	QTranslator appTranslator;
	if (!app.arguments().contains("--no-locale")) {
		appTranslator.load(":/editorPluginTestingFramework_" + QLocale::system().name());
		app.installTranslator(&appTranslator);
	}

	parser.addPositionalArgument("metamodel", QObject::tr("Metamodel file to be processed."));
	parser.addPositionalArgument("config", QObject::tr("XML file with configuration of the test."));

	parser.process(app);

	const QStringList positionalArgs = parser.positionalArguments();
	if (positionalArgs.size() != 2) {
		parser.showHelp();
	}

	const QString metamodel = positionalArgs.first();
	const QString configurationFileName = positionalArgs.at(1);

	/// @todo: is path to QRMC shall be configured?
	MainClass mainClass(metamodel, qApp->applicationDirPath() + "/../../qrmc/"
			, qApp->applicationDirPath(), configurationFileName);

	return mainClass.testResult();
}
