#include <QDebug>
#include <QStringList>
#include "interpreter.h"
#include "gemake.h"

using namespace Geny;

//TODO: добавить выброс исключений по error'ам

Interpreter::Interpreter(const QString& repoPath, const QString& taskFilename, qReal::Id curObjectId, Gemake* geMaker): 
		taskFile(taskFilename), inStream(0), rApi(repoPath), repoPath(repoPath), 
		geMaker(geMaker), curObjectId(curObjectId) {
}

Interpreter::~Interpreter() {
	if (inStream)
		delete inStream;
	taskFile.close();
}

qReal::Id Interpreter::getCurObjId() {
	return curObjectId;
}

QString Interpreter::controlExpressionParse(const QString& expression) {
	if (expression.at(0) != '!') {
		if (!expression.contains('@'))
			return getCurObjProperty(expression);
		else {
			QStringList listOfSplitting = expression.split("@");
			if (listOfSplitting.size() == 2) {
				return getObjProperty(objectsByLabels[listOfSplitting[0].trimmed()], listOfSplitting[1].trimmed());
			}
			else
				qDebug() << "Fail in \'@@ @ @@\' expression";
		}
	}
	else if (expression.startsWith("!task ")){
		QString subTaskName = expression.mid(6).trimmed();

		Interpreter ipreter(repoPath, geMaker->getTaskFilename(subTaskName), getCurObjId(), geMaker);
		return ipreter.interpret();
	} else
		qDebug() << "Fail in @@! expression";
	
	return "";
}

QString Interpreter::getObjProperty(const qReal::Id& objectId, const QString& propertyName) {
	if (!rApi.exist(objectId)) {
		qDebug() << "Error! Trying to work with not existed element Id in current repository!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	if (!rApi.hasProperty(objectId, propertyName)) {
		qDebug() << "Error! Trying to get not existed property of current element!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	return rApi.property(objectId, propertyName).toString();
}

QString Interpreter::getCurObjProperty(const QString& propertyName) {
	return getObjProperty(getCurObjId(), propertyName);
}

Interpreter::ControlStringType Interpreter::controlStringType(const QString& str) {
	QString workStr = str.trimmed();
	if (!workStr.startsWith("#!"))
		return notControlType;

	workStr = workStr.right(workStr.length() - 2).trimmed();//убираем #!

	if (workStr.startsWith("/"))
		return commentType;
	if (workStr.startsWith("foreach "))
		return foreachType;	
	if (workStr.startsWith("for "))
		return forType;
	if (workStr.startsWith("{"))
		return leftBraceType;
	if (workStr.startsWith("}"))
		return rightBraceType;
	if (workStr.startsWith("toFile"))
		return toFileType;
	if (workStr.startsWith("saveObj"))
		return saveObjType;
	if (workStr.startsWith("switch"))
		return switchType;
	if (workStr.startsWith("case") || workStr.startsWith("default"))
		return caseType;

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

QString Interpreter::forStringParse(const QString& str) {
	QStringList strElements = str.split(' ');
	strElements.removeAll("");

	if ( (strElements.length() != 2) || 
			(strElements[0] != "#!for")) {
		qDebug()  << "Error! Bad \'for\' structure!";
		return QString(); //TODO: возможно лучше бросать исключение!
	}

	/*
	strElements.at(1) : that method to call;
	ex : #!for to - strElements.at(1) = "to"
	ex : #!for parent - strElements.at(1) = "parent"
	*/

	return strElements.at(1);
}

QString Interpreter::toFileStringFilename(const QString& str) {
	QStringList strElements = str.split(' ');
	strElements.removeAll("");

	if ( (strElements.length() != 2) || 
			(strElements[0] != "#!toFile")) {
		qDebug()  << "Error! Bad \'toFile\' structure!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	return notControlStringParse(strElements[1]);
}

QString Interpreter::switchStringParse(const QString& str) {
	QStringList strElements = str.split(' ');
	
	if ( (strElements.length() != 2) || 
			(strElements[0] != "#!switch")) {
		qDebug()  << "Error! Bad \'switch\' structure!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	return strElements[1].trimmed();
}

QString Interpreter::caseStringParse(const QString& str) {
	QStringList strElements = str.split(' ');

	if (controlStringType(strElements[0]) != caseType) {
		qDebug()  << "Error! Bad \'case\' structure!";
		return ""; //TODO: возможно лучше бросать исключение!
	}

	if (strElements[0].startsWith("#!default"))
		return "";

	QString caseValue = str.mid(7).trimmed();//отрезаем "#!case "
	int firstApostoIndex = caseValue.indexOf("\'");
	int lastApostoIndex = caseValue.lastIndexOf("\'");

	if (firstApostoIndex == -1) {
		qDebug() << "Error! Bad \'case\' structure!";
		return "";
	}

	return caseValue.mid(firstApostoIndex + 1, lastApostoIndex - firstApostoIndex - 1);
}

QString Interpreter::saveObjLabel(const QString& str) {
	return str.mid(9).trimmed();//object label
}

void Interpreter::addLabel(const QString& str) {
	objectsByLabels.insert(str, getCurObjId());
}

//Для обращения к методу elementsByType передается "elementsByType(__type_name__)"
qReal::IdList Interpreter::getCurObjectMethodResultList(const QString& methodName) {
	if (methodName == "children")
		return rApi.children(getCurObjId());

	if (methodName == "outgoingLinks")
		return rApi.outgoingLinks(getCurObjId());

	if (methodName == "incomingLinks")
		return rApi.incomingLinks(getCurObjId());

	if (methodName == "links")
		return rApi.links(getCurObjId());

	if (methodName == "outgoingConnections")
		return rApi.outgoingConnections(getCurObjId());

	if (methodName == "incomingConnections")
		return rApi.incomingConnections(getCurObjId());

	if (methodName == "outgoingUsages")
		return rApi.outgoingUsages(getCurObjId());

	if (methodName == "incomingUsages")
		return rApi.incomingUsages(getCurObjId());

	if (methodName == "connectedElements")
		return rApi.connectedElements(getCurObjId());

	if (methodName == "outgoingConnectedElements")
		return rApi.outgoingConnectedElements(getCurObjId());

	if (methodName == "incomingConnectedElements")
		return rApi.incomingConnectedElements(getCurObjId());

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

		//return rApi.elementsByType(elementsType);
		return rApi.logicalElements(qReal::Id("", "", elementsType, ""));
	}

	qDebug() << "Error! Uses unknown RepoApi list method!";

	return qReal::IdList();
}

qReal::Id Interpreter::getCurObjectMethodResult(const QString& methodName) {	
	if (methodName == "parent")
		return rApi.parent(getCurObjId());

	if (methodName == "to")
		return rApi.to(getCurObjId());

	if (methodName == "from")
		return rApi.from(getCurObjId());

	return qReal::Id();
}

QString Interpreter::notControlStringParse(const QString& parsingStr) {
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

QPair<QString, QString> Interpreter::getNextCaseBlock(QTextStream& stream) {
	if (stream.atEnd())
		return QPair<QString, QString>();

	QString caseStr = stream.readLine();

	if (controlStringType(caseStr) != caseType) {
		qDebug() << "Error! There is must be case string but \'" << caseStr << "\' found!";
		return QPair<QString, QString>();
	}

	QString braceBlock = getBraceBlock(stream);
	
	QTextStream caseBlockStream(&braceBlock);

	return QPair<QString, QString>(caseStringParse(caseStr), interpret( caseBlockStream ));
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
				qReal::Id objectId = getCurObjId();//TODO: change this method

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
		case forType:
			{
				QString braceBlock = getBraceBlock(stream);
				QTextStream forBlockStream(&braceBlock);

				QString resultStr;

				QString methodName = forStringParse(parsingStr);
				qReal::Id objectId = getCurObjId();//TODO: change this method

				curObjectId = getCurObjectMethodResult(methodName);
				resultStr += interpret(forBlockStream);
				
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
		case saveObjType:
			{
				addLabel(saveObjLabel(parsingStr));
				return "";
			}
		case switchType:
			{
				QString resultStr;
				QString switchPropertyName = switchStringParse(parsingStr);
				
				QString switchProperty;
				if (switchPropertyName == "ELEMENT_TYPE")
					switchProperty = getCurObjId().element();
				else
					switchProperty = getCurObjProperty(switchPropertyName);

				QString braceBlock = getBraceBlock(stream);

				QTextStream switchBlockStream(&braceBlock);
				switchBlockStream.readLine(); //TODO FAIL

				for (QPair<QString, QString> nextCaseBlock = getNextCaseBlock(switchBlockStream);
				    !nextCaseBlock.second.isEmpty(); 
				    nextCaseBlock = getNextCaseBlock(switchBlockStream)) {
					if (nextCaseBlock.first == switchProperty || nextCaseBlock.first == "") { 
					//nextCaseBlock.first == "" - default case
						QTextStream caseBlockStream(&(nextCaseBlock.second));
						resultStr += interpret(caseBlockStream);
						break;
					}
				}

				return resultStr;
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
			//resultStr += notControlStringParse(curStr) + "\n";
			QString str = notControlStringParse(curStr);
			resultStr += str.isEmpty() ? str : str + "\n";
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
