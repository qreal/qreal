/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "abstractGenerator.h"
#include "nameNormalizer.h"

using namespace qReal;
using namespace utils;

AbstractGenerator::AbstractGenerator(const QString &templateDirPath
		, const QString &outputDirPath
		, const qReal::LogicalModelAssistInterface &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: mApi(logicalModel.logicalRepoApi())
		, mErrorReporter(errorReporter)
		, mOutputDirPath(outputDirPath)
		, mTemplateDirPath(templateDirPath)
{
	loadUtilsTemplates();
}

AbstractGenerator::~AbstractGenerator()
{
}

QDir AbstractGenerator::getDir(const QString &path)
{
	const QDir result(path);
	if (!result.exists()) {
		qDebug() << "cannot find directory " << path;
		return QDir();
	}

	return result;
}

bool AbstractGenerator::loadTemplateFromFile(const QString &templateFileName, QString &loadedTemplate)
{
	const QDir dir = getDir(mTemplateDirPath);
	if (dir == QDir()) {
		return false;
	}

	const QString fileName = dir.absoluteFilePath(templateFileName);
	qDebug() << fileName;
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&file);
	loadedTemplate = in.readAll();
	file.close();

	return true;
}

bool AbstractGenerator::loadUtilsFromFile()
{
	const QDir dir = getDir(mTemplateDirPath);
	if (dir == QDir()) {
		return false;
	}

	const QString fileName = dir.absoluteFilePath(utilsFileName);
	QFile utilsFile(fileName);
	if (!utilsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "cannot load file \"" << fileName << "\"";
		return false;
	}
	QTextStream in(&utilsFile);

	QString line = in.readLine();
	do {
		// first line is name, everything else before the separator is template body
		const QString name = line;
		QString body;
		line = in.readLine();
		while (!line.contains(utilsSeparator) && !line.isNull()) {
			body += (line + '\n');
			line = in.readLine();
		}
		mTemplateUtils[name] = body;
		line = in.readLine();
	} while (!line.isNull());

	utilsFile.close();

	return true;
}

bool AbstractGenerator::loadUtilsFromDir()
{
	const QDir dir = getDir(mTemplateDirPath + "/utils");
	if (dir == QDir()) {
		return false;
	}

	const QStringList files = dir.entryList(QStringList());

	foreach (const QString &fileName, files) {
		if (fileName == "." || fileName == "..") {
			continue;
		}

		// file name is template name, file contents is template body
		const QString file = dir.absoluteFilePath(fileName);
		QFile templateFile(file);

		if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qDebug() << "cannot load file \"" << file << "\"";
			return false;
		}

		QTextStream in(&templateFile);

		QString line = in.readLine();
		QString body = line;
		while (!line.isNull()) {
			body += (line + '\n');
			line = in.readLine();
		}

		const QString name = "@@" + fileName + "@@";
		if (!mTemplateUtils.contains(name)) {
			mTemplateUtils[name] = body;
		}

		templateFile.close();
	}

	return true;
}

bool AbstractGenerator::loadUtilsTemplates()
{
	mTemplateUtils.clear();

	return loadUtilsFromFile() && loadUtilsFromDir();
}

void AbstractGenerator::saveOutputFile(const QString &fileName, const QString &content)
{
	QDir dir;

	if (!dir.exists(mOutputDirPath)) {
		dir.mkdir(mOutputDirPath);
	}
	dir.cd(mOutputDirPath);

	const QString outputFileName = dir.absoluteFilePath(fileName);
	QFile file(outputFileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << outputFileName << "\"";
		return;
	}

	QTextStream out(&file);
	out << content;
	file.close();
}

QString AbstractGenerator::getDefaultValue(const QString &type)
{
	// Here to write all other needed types.
	if (type == "short" || type == "int") {
		return "0";
	}

	return "new " + type + "()";
}

QString AbstractGenerator::generatePropertiesCode(const Id &element)
{
	QString properties;
	foreach (const Id &property, mApi.children(element)) {
		if (!mApi.isLogicalElement(property) || property.element() != "Field") {
			continue;
		}

		// generate property code
		QString propertyTemplate = mTemplateUtils["@@Property@@"];
		const QString name = mApi.name(property);
		propertyTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", mApi.stringProperty(property, "Type"));

		properties += propertyTemplate;
	}
	return properties;
}
