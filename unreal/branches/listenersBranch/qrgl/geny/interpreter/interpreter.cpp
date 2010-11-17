#include <QDebug>

#include "interpreter.h"

using namespace Geny;

Interpreter::Interpreter(QString taskFilename) : taskFile(taskFilename), inStream(0) {
}

Interpreter::~Interpreter() {
	if (inStream)
		delete inStream;
}

bool Interpreter::isControlString(QString str) {
	int i = 0;
	while (i < str.length() && str[i].isSpace())
		i++;
	if (i < str.length() - 1 && (str[i] == '#') && (str[i + 1] == '!'))
		return true;

	return false;
}

QString Interpreter::interpreter() {
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(file);
	}
	else {
		qDebug() << "cannot load file \"" << file.fileName() << "\"";
	}

	QString curStr;
	curStr = inStream->readLine();
	if (!curStr.startsWith("Task ")) {
		qDebug() << "Task file" << file.fileName() << "doesn't start with \"Task __name__\"";
		return "";
	}
	
	//QString taskName = curStr.end(curStr.length() - 5); //5 - "Task " length

	QString resultStr;

	//выбор парсера по паттерну State в зависимости от isControlString

	return resultStr;
}
