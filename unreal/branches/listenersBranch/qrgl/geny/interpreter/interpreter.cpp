#include <QDebug>
#include <QStringList>
#include "interpreter.h"

using namespace Geny;

Interpreter::Interpreter(const QString& repoDirectory, const QString& taskFileName, qReal::Id curObjectId): 
	taskFile(taskFileName), inStream(0), rApi(repoDirectory), curObjectId(curObjectId) {
}

/*
Interpreter::Interpreter(const QString& taskFilename) : taskFile(taskFilename), inStream(0) {
	//TODO: 1) задать rApi
	//	2) задать текущий объект (curObjectId)
}
*/

Interpreter::~Interpreter() {
	if (inStream)
		delete inStream;
	taskFile.close();
}

qReal::Id Interpreter::getCurObjectId() {
	return curObjectId;
}

QString Interpreter::controlExpressionParse(const QString& expression) {
	if (expression.at(0) != '!')
		return getCurrentObjectProperty(expression);
	else
		//TODO: обработка случая управляющего события, такого как ' task "__taskName__" '
		return "";
}

QString Interpreter::getCurrentObjectProperty(const QString& propertyName) {
	//может падать, если у getCurObjectId() нет property propertyName
	//TODO: не закрывать ошибку!!!!
	if (!rApi.exist(getCurObjectId()) || !rApi.hasProperty(getCurObjectId(), propertyName))
		return "";

	qDebug() << rApi.property(getCurObjectId(), propertyName).toString();

	return rApi.property(getCurObjectId(), propertyName).toString();
}

Interpreter::ControlStringType Interpreter::controlStringType(const QString& str) {
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
	return controlStringType(str) != NOT_CONTROL ? true : false;
}

QPair<QString, QString> Interpreter::foreachStringParse(const QString& str) {
	QStringList strElements = str.split(' ');
	strElements.removeAll("");

	qDebug() << "foreach structure:" << strElements;

	if ( (strElements.length() != 4) || 
			(strElements[0] != "#!foreach") || (strElements[2] != "in") ) {
		qDebug()  << "Error! Bad \'foreach\' structure!";
		return QPair<QString, QString>("", "");
	}

	return QPair<QString, QString>(strElements.at(1), strElements.at(3));
	/*
	QString elementsType = strElements[1];
	QString elementsListName = strElements[3];
	*/
}

qReal::IdList Interpreter::getCurObjectMethodResultList(const QString& methodName) {
	if (methodName == "children")
		return rApi.children(getCurObjectId());

	if (methodName == "parents")
		return rApi.parents(getCurObjectId());

	if (methodName == "outgoingLinks")
		return rApi.outgoingLinks(getCurObjectId());

	if (methodName == "incomingLinks")
		return rApi.incomingLinks(getCurObjectId());

	if (methodName == "links")
		return rApi.links(getCurObjectId());

	if (methodName == "outgoingConnections")
		return rApi.outgoingConnections(getCurObjectId());

	if (methodName == "incomingConnections")
		return rApi.incomingConnections(getCurObjectId());

	if (methodName == "outgoingUsages")
		return rApi.outgoingUsages(getCurObjectId());

	if (methodName == "incomingUsages")
		return rApi.incomingUsages(getCurObjectId());
	
	if (methodName == "elements")
		return rApi.elements(getCurObjectId());

	/*
	if (methodName == "elementsByType")
		return rApi.elementsByType(getCurObjectId().element());
	*/
	if (methodName.startsWith("elementsByType")) {
		QString elementsType;
		int leftParenthesisPos = methodName.indexOf('(');
		int rightParenthesisPos = methodName.indexOf(')');

		if ( (leftParenthesisPos > -1) && (rightParenthesisPos > leftParenthesisPos) )
			elementsType = methodName.mid(leftParenthesisPos + 1,
					rightParenthesisPos - leftParenthesisPos - 1);
		else
			return qReal::IdList();

		return rApi.elementsByType(elementsType);
	}

	if (methodName == "getOpenedDiagrams")
		return rApi.getOpenedDiagrams();

	if (methodName == "getChangedDiagrams")
		return rApi.getChangedDiagrams();

	qDebug() << "Error! Uses unknown RepoApi list method!";

	return qReal::IdList();
}

QString Interpreter::nonControlStringParse(const QString& parsingStr, QTextStream& stream) {
	Q_UNUSED(stream);

	//Обработка @@_smth_@@
	QStringList listOfSplitting = parsingStr.split("@@");
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
		resultStr += curElem + ' ';
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
				QString *foreachBlockString = new QString(); //for foreachBlockStream only
				QTextStream foreachBlockStream(foreachBlockString);

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

				QPair<QString, QString> elemAndListNames = foreachStringParse(parsingStr);
				qReal::Id objectId = getCurObjectId();//TODO: change this method

				// Здесь развертка foreach
				foreach (qReal::Id element, getCurObjectMethodResultList(elemAndListNames.second)) {
					if (element.element() == elemAndListNames.first) {
						//обновление curObjectId
						curObjectId = element;
						resultStr += interpret(foreachBlockStream);
						
						foreachBlockStream.seek(0);
					}
				}
				
				curObjectId = objectId;//TODO: change this method

				delete foreachBlockString;

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

QString Interpreter::interpret(QTextStream& stream) {
	QString resultStr;

	while (!stream.atEnd()) {
		QString curStr = stream.readLine();

		if (!isControlString(curStr)) {
			resultStr += nonControlStringParse(curStr, stream);
		}
		else {
			//TODO: ошибка в controlStringParse
			resultStr += controlStringParse(curStr, stream);//может сдвигать stream! Это нужно для for'а
		}
	}

	return resultStr;
}

QString Interpreter::interpret() {
	//TODO: исправить ошибку, связанную со вторым запуском
	if (taskFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&taskFile);
	}
	else {
		qDebug() << "cannot load file \"" << taskFile.fileName() << "\"";
		return "";
	}

	QString curStr;
	curStr = inStream->readLine();
	if (!curStr.startsWith("Task ")) {
		qDebug() << "Task file" << taskFile.fileName() << "doesn't start with \"Task __name__\"";
		return "";
	}

	QString taskName = curStr.right(curStr.length() - 5); //5 - "Task " length;

	return interpret(*inStream);
}
