#include "javaClassGenerator.h"

JavaClassGenerator::JavaClassGenerator(const qrRepo::RepoApi& repoApi), rApi(repoApi) {	
}

bool JavaClassGenerator::generateClass(qReal::Id classElemId) {
	if (rApi.typeName(classElemId) != "Class") {
		qDebug() << "try to generate Java class from not Class element";
		return false;
	}

	QString classFileTemplate;
	if (!loadTemplateFromFile("template.java", classFileTemplate))
		return false;
	
	insertStrToTemplate(rApi.name(classElemId), "@@ClassName@@", classFileTemplate);

	QString fieldTemplate;
	if (!loadTemplateFromFile("templateField.java", fieldTemplate))
		return false;

	QString methodTemplate;
	if (!loadTemplateFromFile("templateMethod.java", methodTemplate))
		return false;

	foreach (qReal::Id containerId, rApi.children(classElemId)) {
		if (rApi.typeName(containerId) == "MethodsContainer") {

		} else if (rApi.typeName(containerId) == "FieldsContainer") {
			foreach (qReal::Id fieldId, rApi.children(containerId)) {
			}
		}
	}

	return true;
}

bool JavaClassGenerator::loadTemplateFromFile(const QString& filename, QString& loadedTemplate) {
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

bool JavaClassGenerator::insertStrToTemplate(const QString& newStr, const QString& placeIndentificator, QString& templateString) {
	templateString.replace(placeIndentificator, newStr);
	return true;
}
