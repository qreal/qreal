#include <QStringList>
#include <QDebug>

#include "interpreter.h"
#include "gemake.h"

using namespace Geny;

Gemake::Gemake(QString gemakeFilename): makeFile(gemakeFilename), 
		filesByTasks(0), inStream(0), repoPath("") {
	init();
}

Gemake::~Gemake() {
	if (!inStream)
		delete inStream;
	if (!filesByTasks)
		delete filesByTasks;
	makeFile.close();
}

QString Gemake::getTaskFilename(const QString& taskName) {
	return filesByTasks->value(taskName);
}

bool Gemake::init() {
	if (!makeFile.isOpen() && makeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&makeFile);
	}
	
	if (!makeFile.isOpen() || inStream == 0){
		qDebug() << "cannot load make file \"" << makeFile.fileName() << "\"";
	}

	repoPath = inStream->readLine();

	QStringList taskFilenames;
	while (!inStream->atEnd()) {
		QString curFilename = inStream->readLine();
		if (curFilename.trimmed().isEmpty())
			continue;

		taskFilenames.append(curFilename.trimmed());
	}
	
	if (!filesByTasks)
		delete filesByTasks;
	filesByTasks = new QMap<QString, QString>();

	foreach (QString curFilename, taskFilenames) {
		QFile curFile(curFilename);
		QTextStream* curStream = 0;
		if (curFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			curStream = new QTextStream(&curFile);
			QString startLine = curStream->readLine();
			if (startLine.startsWith("Task ")) {
				filesByTasks->insert(startLine.mid(5).trimmed(), curFilename);
			}
			else
				qDebug() << "File" << curFilename << "doesn't start with task name";
		}
		else
			qDebug() << "File" << curFilename << "wasn't found!";
	}

	return true;
}

void Gemake::make() {
	Interpreter ipreter(repoPath, filesByTasks->value("Main"), qReal::Id(), this);
	ipreter.interpret();
}
