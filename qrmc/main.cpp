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

#include <qrkernel/platformInfo.h>

#include "metaCompiler.h"

using namespace qrmc;

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

//	qDebug() << "Running " + args.join(" ");

	if (argc != 2) {
		qDebug() << "Usage: qrmc PATH_TO_WORKING_COPY";
		return 1;
	}

	QString workingCopyDir = argv[1];
//	QString workingCopyDir = "../qrgui/save";

	qrRepo::RepoApi *repoApi = new qrRepo::RepoApi(workingCopyDir);
	MetaCompiler metaCompiler(qReal::PlatformInfo::applicationDirPath() + "/../../qrmc/", repoApi);
	if (!metaCompiler.compile()) {
		qDebug() << "compilation failed";
		return 1;
	}

	return 0;
}
