#include <QDebug>
#include <QStringList>
#include <QListIterator>
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
	if (i < str.length() - 1 && (str[i] == '#') && (str[i + 1] == '$'))
		return true;

	return false;
}

bool Interpreter::isCommentString(QString str) {
	int i = 0;
	while (i < str.length() && str[i].isSpace())
		i++;
	if (i < str.length() - 2 && (str[i] == '#') && (str[i + 1] == '$') && (str[i + 2] == '/'))
		return true;

	return false;
}

QString Interpreter::nonControlStringParse(QString parsingStr) {
	//TODO: сделать обработку @@_smth_@@ и #! !#
	
	//Обработка @@_smth_@@
	QStringList listOfSplitting = parsingStr.split();
	if (listOfSplitting.length() % 2 == 0) {
		qDebug() << "problem with number of @@ in task" << file.fileName();
		return "";
	}

	//теперь каждый нечетный элемент listOfSplitting - что-то между @@ @@
	int iterationNumber = 0;
	foreach (QString curElem, listOfSplitting) {
		if (iterationNumber % 2 == 1)
			//TODO: реализовать getPropertyOfCurrentObject
			listOfSplitting.replace(iterationNumber, getPropertyOfCurrentObject(curElem));
		iterationNumber++;
	}

	return parsingStr;
}

QString Interpreter::controlStringParse(QString parsingStr) {
	if (isCommentString(parsingStr))
		return "";

	//TODO: сделать обработку for и его границ
	return "";
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

	while (!inStream->atEnd()) {
		curStr = inStream->readLine();

		if (!isControlString(curStr)) {
			resultStr += nonControlStringParse(curStr);
		}
		else {
			resultStr += controlStringParse(curStr);//сдвигает inStream!
		}
	}

	//выбор парсера по паттерну State в зависимости от isControlString

	return resultStr;
}
