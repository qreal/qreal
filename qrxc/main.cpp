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

#include "xmlCompiler.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
	Q_UNUSED(context)
	const char *msg = message.toLatin1().data();
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
	}
}

int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageOutput);
	QCoreApplication app(argc, argv);
	QStringList args = app.arguments();

	qDebug() << "Running " + args.join(" ");

	if (args.count() != 3) {
		qDebug() << "Usage: qrxc inputFile.xml <path to root directory of qreal sources>";
		return 1;
	}

	const QString inputXmlFileName = args[1];
	const QString root = args[2];

	XmlCompiler xmlCompiler;
	if (!xmlCompiler.compile(inputXmlFileName, root))
		return 1;

	return 0;
}
