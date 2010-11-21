#include <QDebug>
#include <QStringList>
#include "interpreter.h"

using namespace Geny;

Interpreter::Interpreter(const QString& taskFilename) : taskFile(taskFilename), inStream(0) {
	//TODO: 1) задать rApi
	//	2) задать текущий объект (curObjectId)
}

Interpreter::~Interpreter() {
	if (inStream)
		delete inStream;
}

qReal::Id Interpreter::getCurObjectId() {
	return curObjectId;
}

QString controlExpressionParse(const QString& expression) {
	if (expression.at(0) != '!')
		return getCurrentObjectProperty(expression);
	else
		//TODO: обработка случая управляющего события, такого как ' task "__taskName__" '
		return "";
}

QString getCurrentObjectProperty(const QString& propertyName) {
	return rApi.property(getCurObjectid(), propertyName).toString();
}

ControlStringType Interpreter::controlStringType(const QString& str) {
	QString workStr = str.trimmed();
	if (!workStr.startsWith("#!"))
		return NOT_CONTROL;

	workStr = workStr.right(workStr.length() - 2).trimmed();//убираем #!

	if (workStr.startsWith("/"))
		return COMMENT;
	if (workStr.startsWith("foreach"))
		return FOREACH;
	if (workStr.startsWith("{"))
		return LEFT_BRACE;
	if (workStr.startsWith("}"))
		return RIGHT_BRACE;

	return NOT_CONTROL;
}

bool Interpreter::isControlString(const QString& str) {
	return controlStringType != NOT_CONTROL ? true : false;
}

QString Interpreter::nonControlStringParse(const QString& parsingStr, QTextStream& stream) {
	//Обработка @@_smth_@@
	QStringList listOfSplitting = parsingStr.split();
	if (listOfSplitting.length() % 2 == 0) {
		qDebug() << "problem with number of @@ in task" << taskFile.fileName();
		return "";
	}

	//теперь каждый нечетный элемент listOfSplitting - что-то между @@ @@
	int iterationNumber = 0;
	foreach (QString curElem, listOfSplitting) {
		if (iterationNumber % 2 == 1)
			listOfSplitting.replace(iterationNumber, controlExpressionParse(curElem));
		iterationNumber++;
	}

	QString resultStr;
	foreach (QString curElem, listOfSplitting) {
		resultStr = curElem + ' ';
	}

	//необходимо стереть лишний ' '
	if (resultStr.length() > 0)
		resultStr.resize(resultStr.length() - 1);

	return resultStr + '\n';
}

QString Interpreter::controlStringParse(const QString& parsingStr, QTextStream& stream) {
	switch (controlStringType(parsingStr)) {
		case COMMENT:
			return "";
		case FOREACH:
			{
				//TODO: сделать обработку foreach и его границ
				
				//TODO: парсинг parsingStr для получения атрибутов foreach
				QTextStream foreachBlockStream;

				QString curLine = stream.readLine();
				if (controlStringType(curLine) != LEFT_BRACE) {
					qDebug() << "Error! After #!foreach not #!{ but \'" << curLine << "\' found!";
					return "";
				}

				int braceBalance = 1;
				curLine = "";

				while ( (braceBalance != 0) && !stream.atEnd() ) {
					foreachBlockStream << curLine << '\n';

					curLine  = stream.readLine();
					if (controlStringType(curLine) == LEFT_BRACE)
						braceBalance++;
					if (controlStringType(curLine) == RIGHT_BRACE)
						braceBalance--;
				}
				if ( (braceBalance != 0) && (stream.atEnd()) ) {
					qDebug() << "Error! There is no brace balance!";
					return "";
				}

				QString resultStr;
				/*
				 * Здесь развертка foreach
				foreach(__, __) {
					//обновление curObjectId
					resultStr += interpret(foreachBlockStream);
					foreachBlockStream.reset();
				}
				 */

				return resultStr;
			}
		case LEFT_BRACE:			
			{
				qDebug() << "Error! In" << taskFile.fileName() << ". #!{ but not control expression (ex #!foreach) found!";
				return "";
			}

		case RIGHT_BRACE:
			{
				qDebug() << "Error! In" << taskFile.fileName() << ". #!} but not control expression (ex #!foreach) found!";
				return "";
			}
		case NOT_CONTROL:
			{
				return "";
			}
	}

	return "";
}

QString Interpreter::interpreter(QTextStream& stream) {
	QString resultStr;

	while (!stream->atEnd()) {
		curStr = stream->readLine();

		if (!isControlString(curStr)) {
			resultStr += nonControlStringParse(curStr, stream);
		}
		else {
			resultStr += controlStringParse(curStr, stream);//может сдвигать stream! Это нужно для for'а
		}
	}

	return resultStr;
}

QString Interpreter::interpreter() {
	if (taskFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(taskFile);
	}
	else {
		qDebug() << "cannot load file \"" << taskFile.fileName() << "\"";
	}

	QString curStr;
	curStr = inStream->readLine();
	if (!curStr.startsWith("Task ")) {
		qDebug() << "Task file" << taskFile.fileName() << "doesn't start with \"Task __name__\"";
		return "";
	}
	
	QString taskName = curStr.end(curStr.length() - 5); //5 - "Task " length;

	return interpreter(*inStream);
}
