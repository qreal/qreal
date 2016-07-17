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

#include <QtCore/QTranslator>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QCommandLineParser>

#include <qrrepo/repoApi.h>

#include "metaCompiler.h"

using namespace qrmc;

static const QString description = QObject::tr(
		"QReal Metamodel Compiler. It takes .qrs file with QReal metamodel created by metaeditor"
		" and generates Qt project with editor plugin for that metamodel. Project is ready to be built"
		" with qmake, but it actively uses QReal sources and build system, so it can not be built alone."
		" Example of command line:\n") +
		"    qrmc metamodel.qrs ../../plugins/qrmcGeneratedEditor";

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

	QCoreApplication::setApplicationName("QRMC");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription(description);
	parser.addHelpOption();
	parser.addVersionOption();

	QTranslator appTranslator;
	if (!app.arguments().contains("--no-locale")) {
		appTranslator.load(":/qrmc_" + QLocale::system().name());
		app.installTranslator(&appTranslator);
	}

	parser.addPositionalArgument("metamodel", QObject::tr("Metamodel file to be processed."));
	parser.addPositionalArgument("target-directory"
			, QObject::tr("Directory to which source code of the editor plugin shall be generated."));

	parser.process(app);

	const QStringList positionalArgs = parser.positionalArguments();
	if (positionalArgs.size() != 2) {
		parser.showHelp();
	}

	const QString metamodel = positionalArgs.first();
	const QString targetDir = positionalArgs.at(1);

	qrRepo::RepoApi repoApi(metamodel);
	MetaCompiler metaCompiler(repoApi, targetDir);
	if (!metaCompiler.compile()) {
		qDebug() << "Compilation failed.";
		return 1;
	}

	qDebug() << "Compilation completed.";
	return 0;
}
