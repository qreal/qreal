#include <QDebug>
#include <QStringList>
#include "interpreter.h"

using namespace Geny;

//TODO: добавить выброс исключений по error'ам

Interpreter::Interpreter(const QString& repoDirectory, const QString& taskFileName, qReal::Id curObjectId): 
	taskFile(taskFileName), inStream(0), rApi(repoDirectory), curObjectId(curObjectId) {
}

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
	if (!rApi.exist(getCurObjectId())) {
		qDebug() << "Error! Trying to work with not existed element Id in current repository!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	if (!rApi.hasProperty(getCurObjectId(), propertyName)) {
		qDebug() << "Error! Trying to get not existed property of current element!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	//qDebug() << rApi.property(getCurObjectId(), propertyName).toString();

	return rApi.property(getCurObjectId(), propertyName).toString();
}

Interpreter::ControlStringType Interpreter::controlStringType(const QString& str) {
	QString workStr = str.trimmed();
	if (!workStr.startsWith("#!"))
		return notControlType;

	workStr = workStr.right(workStr.length() - 2).trimmed();//убираем #!

	if (workStr.startsWith("/"))
		return commentType;
	if (workStr.startsWith("foreach"))
		return foreachType;
	if (workStr.startsWith("{"))
		return leftBraceType;
	if (workStr.startsWith("}"))
		return rightBraceType;
	if (workStr.startsWith("toFile"))
		return toFileType;

	return notControlType;
}

bool Interpreter::isControlString(const QString& str) {
	return controlStringType(str) != notControlType ? true : false;
}

QPair<QString, QString> Interpreter::foreachStringParse(const QString& str) {
	QStringList strElements = str.split(' ');
	strElements.removeAll("");

	if ( (strElements.length() != 4) || 
			(strElements[0] != "#!foreach") || (strElements[2] != "in") ) {
		qDebug()  << "Error! Bad \'foreach\' structure!";
		return QPair<QString, QString>("", ""); //TODO: возможно лучше бросать исключение!
	}

	/*
	strElements.at(1) : elementsType;
	strElements.at(3) : elementsListName;
	*/

	return QPair<QString, QString>(strElements.at(1), strElements.at(3));
}

QString Interpreter::toFileStringFilename(const QString& str) {
	QStringList strElements = str.split(' ');
	strElements.removeAll("");

	if ( (strElements.length() != 2) || 
			(strElements[0] != "#!toFile")) {
		qDebug()  << "Error! Bad \'toFile\' structure!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	return nonControlStringParse(strElements[1]);
}

//Для обращения к методу elementsByType передается "elementsByType(__type_name__)"
qReal::IdList Interpreter::getCurObjectMethodResultList(const QString& methodName) {
	if (methodName == "children")
		return rApi.children(getCurObjectId());

	if (methodName == "parent") {
		qReal::IdList resultList;
		resultList.append(rApi.parent(getCurObjectId()));
		return resultList;
	}

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

//	if (methodName == "elements")
//		return rApi.elements(getCurObjectId());

	//Для обращения к методу elementsByType передается "elementsByType(__type_name__)"
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

	qDebug() << "Error! Uses unknown RepoApi list method!";

	return qReal::IdList();
}

QString Interpreter::nonControlStringParse(const QString& parsingStr) {
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
		resultStr += curElem;
	}

	return resultStr;
}

QString Interpreter::getBraceBlock(QTextStream& stream) {
	QString resultStr;

	QString curLine = stream.readLine();
	if (controlStringType(curLine) != leftBraceType) {
		qDebug() << "Error! After block operator not #!{ but \'" << curLine << "\' found!";
		return "";
	}

	int braceBalance = 1;
	curLine = "";

	while ( (braceBalance != 0) && !stream.atEnd() ) {
		resultStr += curLine + "\n";

		curLine  = stream.readLine();
		if (controlStringType(curLine) == leftBraceType)
			braceBalance++;
		if (controlStringType(curLine) == rightBraceType)
			braceBalance--;
	}

	if ( (braceBalance != 0) && (stream.atEnd()) ) {
		qDebug() << "Error! There is no brace balance!";
		return "";
	}

	return resultStr;
}

QString Interpreter::controlStringParse(const QString& parsingStr, QTextStream& stream) {
	switch (controlStringType(parsingStr)) {
		case commentType:
			return "";
		case foreachType:
			{
				QString braceBlock = getBraceBlock(stream);
				QTextStream foreachBlockStream(&braceBlock);

				QString resultStr;

				QPair<QString, QString> elemAndListNames = foreachStringParse(parsingStr);
				qReal::Id objectId = getCurObjectId();//TODO: change this method

				// Здесь развертка foreach
				foreach (qReal::Id element, getCurObjectMethodResultList(elemAndListNames.second)) {
					if (elemAndListNames.first == "." || element.element() == elemAndListNames.first) {
						//обновление curObjectId
						curObjectId = element;
						resultStr += interpret(foreachBlockStream);
						
						foreachBlockStream.seek(0);
					}
				}
				
				curObjectId = objectId;//TODO: change this method

				return resultStr;
			}
		case leftBraceType:			
			{
				qDebug() << "Error! In" << taskFile.fileName() << ". #!{ but not control expression (e.g. #!foreach) found!";
				return "";
			}

		case rightBraceType:
			{
				qDebug() << "Error! In" << taskFile.fileName() << ". #!} but not control expression (e.g. #!foreach) found!";
				return "";
			}
		case toFileType:
			{
				//TODO
				QFile file(toFileStringFilename(parsingStr));
				if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
					qDebug() << "cannot open \"" << file.fileName() << "\"";
					return "";
				}	
				
				QString braceBlock = getBraceBlock(stream);
				QTextStream toFileBlockStream(&braceBlock);

				QTextStream out(&file);
				out << interpret(toFileBlockStream);
				file.close();

				return "";
			}
		case notControlType:
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
			resultStr += nonControlStringParse(curStr) + "\n";
		}
		else {
			resultStr += controlStringParse(curStr, stream);//может сдвигать stream! Это нужно для for'а
		}
	}

	return resultStr;
}

QString Interpreter::interpret() {
	if (!taskFile.isOpen() && taskFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&taskFile);
	}
	
	if (!taskFile.isOpen() || inStream == 0){
		qDebug() << "cannot load file \"" << taskFile.fileName() << "\"";
		return "";
	}
	inStream->seek(0); //for second execute of interpret

	QString curStr;
	curStr = inStream->readLine();
	if (!curStr.startsWith("Task ")) {
		qDebug() << "Task file" << taskFile.fileName() << "doesn't start with \"Task __name__\"";
		return "";
	}

	QString taskName = curStr.right(curStr.length() - 5); //5 - "Task " length;

	return interpret(*inStream);
}
