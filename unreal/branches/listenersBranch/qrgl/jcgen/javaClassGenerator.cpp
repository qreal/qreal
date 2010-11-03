#include "javaClassGenerator.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

JavaClassGenerator::JavaClassGenerator(const QString& repoDirectory): rApi(repoDirectory) {	
}

bool JavaClassGenerator::generateClass(const qReal::Id& classElemId) {
	if (rApi.typeName(classElemId) != "Class") {
		qDebug() << "try to generate Java class from not Class element";
		return false;
	}

	QString classTemplate;
	if (!loadTemplateFromFile("template.java", classTemplate))
		return false;
	
	insertStrToTemplate(rApi.name(classElemId), "@@ClassName@@", classTemplate);

	QString fieldTemplate;
	if (!loadTemplateFromFile("templateField.java", fieldTemplate))
		return false;

	QString methodTemplate;
	if (!loadTemplateFromFile("templateMethod.java", methodTemplate))
		return false;

	foreach (qReal::Id containerId, rApi.children(classElemId)) {
		if (rApi.typeName(containerId) == "MethodsContainer") {
			foreach (qReal::Id methodElemId, rApi.children(containerId)) {
				QString newMethodStr;
				generateMethodString(methodElemId, methodTemplate, newMethodStr);
				appendAfterTemplatePlace(newMethodStr, "@@MethodPlace@@", classTemplate);
			}
			classTemplate.remove("@@MethodPlace@@");

		} else if (rApi.typeName(containerId) == "FieldsContainer") {
			foreach (qReal::Id fieldElemId, rApi.children(containerId)) {
				QString newFieldStr;
				generateFieldString(fieldElemId, fieldTemplate, newFieldStr);
				appendAfterTemplatePlace(newFieldStr, "@@FieldPlace@@", classTemplate);
			}
			classTemplate.remove("@@FieldPlace@@");
		}
	}

	QFile outJavaFile(rApi.name(classElemId) + ".java");
	if (!outJavaFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream outJava(&outJavaFile);
	outJava << classTemplate;
	outJavaFile.close();

	return true;
}

bool JavaClassGenerator::generateAllRepoClasses() {
	foreach (qReal::Id classId, rApi.elementsByType("Class")) {
		generateClass(classId);
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

bool JavaClassGenerator::generateFieldString(const qReal::Id& fieldElemId, const QString& fieldTemplate, QString& resultFieldString) {
	if (rApi.typeName(fieldElemId) != "Field") {
		qDebug() << "try to generate Java class field from not Field element";
		return false;
	}

	resultFieldString = fieldTemplate;

	QString fieldName = rApi.property(fieldElemId, "fieldName").toString();
	insertStrToTemplate(fieldName, "@@FieldName@@", resultFieldString);
	
	QString fieldType = rApi.property(fieldElemId, "fieldType").toString();
	insertStrToTemplate(fieldType, "@@FieldType@@", resultFieldString);
	
	QString fieldVisibility = rApi.property(fieldElemId, "fieldVisibility").toString();
	insertStrToTemplate(fieldVisibility, "@@FieldVisibility@@", resultFieldString);	

	return true;
}

bool JavaClassGenerator::generateMethodString(const qReal::Id& methodElemId, const QString& methodTemplate, QString& resultMethodString) {
	if (rApi.typeName(methodElemId) != "Method") {
		qDebug() << "try to generate Java class method from not Method element";
		return false;
	}

	resultMethodString = methodTemplate;

	QString methodName = rApi.property(methodElemId, "methodName").toString();
	insertStrToTemplate(methodName, "@@MethodName@@", resultMethodString);

	QString methodReturnType = rApi.property(methodElemId, "methodReturnType").toString();
	insertStrToTemplate(methodReturnType, "@@MethodReturnType@@", resultMethodString);
	
	QString methodVisibility = rApi.property(methodElemId, "methodVisibility").toString();
	insertStrToTemplate(methodVisibility, "@@MethodVisibility@@", resultMethodString);
	
	QString methodParameters = rApi.property(methodElemId, "methodParameters").toString();

	QString resultParamString;
	foreach (QString curParamWithType, methodParameters.split("##")) {
		QStringList paramWithTypeList = curParamWithType.split("%%");
		if (paramWithTypeList.size() > 1)
			resultParamString += paramWithTypeList.at(1) + " " + paramWithTypeList.at(0) + ", ";
	}
	resultParamString.resize(resultParamString.size() -2);//убираем лишнюю запятую и пробел
	insertStrToTemplate(resultParamString, "@@MethodParameters@@", resultMethodString);

	return true;
}

bool JavaClassGenerator::appendAfterTemplatePlace(const QString& newStr, const QString& placeIndentificator, QString& templateString) {
	int newStrPlace = templateString.indexOf(placeIndentificator);
	if (newStrPlace > -1)
		templateString.insert(newStrPlace + placeIndentificator.size(), newStr);

	return true;
}
