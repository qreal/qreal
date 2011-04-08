#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "generator.h"

bool Generator::loadTemplateFromFile(const QString& filename, QString& loadedTemplate) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << filename << "\"";
		return false;
	}
	QTextStream in(&file);
	loadedTemplate = in.readAll();

	file.close();

	return true;
}

bool Generator::loadClassContent(const QString& filename, ClassContent& classContent) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << filename << "\"";
		return false;
	}
	QTextStream in(&file);
	classContent.className = in.readLine();

	file.close();

	return true;
}

bool Generator::insertStrToTemplate(const QString& newStr, const QString& placeIndentificator, QString& templateString) {
	templateString.replace(placeIndentificator, newStr);
	return true;
}

bool Generator::compile(const QString& templateHFilename, const QString& templateCPPFilename, 
		const QString& templateMethodHFilename, const QString& templateMethodCPPFilename,
		const QString& classContentFilename,
		const QString& resultHFilename, const QString& resultCPPFilename) {
	ClassContent classContent;	
	
	if (!loadClassContent(classContentFilename, classContent))
		return false;

	classContent.methodsList.append(Method("void", "sort"));
	classContent.methodsList.append(Method("void", "swap"));
	classContent.methodsList.append(Method("int", "getNumber"));
	classContent.methodsList.append(Method("void", "setNumber"));

	QString templateHString;
	if (!loadTemplateFromFile(templateHFilename, templateHString))
		return false;
	
	QString templateCPPString;
	if (!loadTemplateFromFile(templateCPPFilename, templateCPPString))
		return false;
	
	QString templateMethodHString;
	if (!loadTemplateFromFile(templateMethodHFilename, templateMethodHString))
		return false;
	
	QString templateMethodCPPString;
	if (!loadTemplateFromFile(templateMethodCPPFilename, templateMethodCPPString))
		return false;

	foreach(Method method, classContent.methodsList) {
		insertMethodToTemplate(method, templateMethodHString, templateHString);
		insertMethodToTemplate(method, templateMethodCPPString, templateCPPString);
	}		
	
	insertStrToTemplate(classContent.className, "@@ClassName@@", templateHString);
	insertStrToTemplate(classContent.className, "@@ClassName@@", templateCPPString);
	
	templateHString.remove("@@MethodsPlace@@");
	templateCPPString.remove("@@MethodsPlace@@");

	QFile outHFile(resultHFilename);
	if (!outHFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream outH(&outHFile);
	outH << templateHString;
	outHFile.close();
	
	QFile outCPPFile(resultCPPFilename);
	if (!outCPPFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream outCPP(&outCPPFile);
	outCPP << templateCPPString;
	outCPPFile.close();

	return true;
}

bool Generator::insertMethodToTemplate(const Method& method, const QString& methodTemplate, QString& templateString) {
	QString realMethodStr = methodTemplate;
	realMethodStr.replace("@@MethodResultType@@", method.resultType);
	realMethodStr.replace("@@MethodName@@", method.name);

	int methodsPlace = -1;
	methodsPlace = templateString.indexOf("@@MethodsPlace@@");
	
	if (methodsPlace > -1)
		templateString.insert(methodsPlace + QString("@@MethodsPlace@@").size(), realMethodStr);

	return true;
}
