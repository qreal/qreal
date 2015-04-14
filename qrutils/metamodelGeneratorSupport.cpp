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

#include "metamodelGeneratorSupport.h"

#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>

#include "xmlUtils.h"
#include "outFile.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace utils;

MetamodelGeneratorSupport::MetamodelGeneratorSupport(ErrorReporterInterface *errorReporter
		, qReal::gui::MainWindowInterpretersInterface *mainWindowInterface)
	: mErrorReporter(errorReporter)
	, mMainWindowInterface(mainWindowInterface)
{
}

MetamodelGeneratorSupport::~MetamodelGeneratorSupport()
{
}

QDomDocument MetamodelGeneratorSupport::loadMetamodelFromFile(const QString &metamodelPath)
{
	return xmlUtils::loadDocument(metamodelPath);
}

void MetamodelGeneratorSupport::saveMetamodelInFile(const QDomDocument &metamodel
		, const QString &metamodelPath)
{
	const int slashIndex = metamodelPath.lastIndexOf("/");
	if (slashIndex > -1) {
		const QString dirPath = metamodelPath.mid(0, slashIndex);
		QDir().mkpath(dirPath);
	}

	OutFile out(metamodelPath);
	out() << metamodel.toString();
	out().flush();
}

void MetamodelGeneratorSupport::loadPlugin(const QString &directoryName
		, const QString &metamodelName
		, const QString &pathToQmake
		, const QString &pathToMake
		, const QString &extension
		, const QString &prefix)
{
	const int progressBarWidth = 240;
	const int progressBarHeight = 20;

	if ((pathToQmake == "") || (pathToMake == "") || (extension == "")) {
		mErrorReporter->addWarning(tr("Please, fill compiler settings"));
		return;
	}

	const QString normalizeDirName = metamodelName.at(0).toUpper() + metamodelName.mid(1);

	QProgressBar * const progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();

	QApplication::processEvents();

	const QRect screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2
			, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	if (!mMainWindowInterface->unloadPlugin(normalizeDirName)) {
		mErrorReporter->addWarning(tr("Cannot unload plugin"));
		progress->close();
		delete progress;
		return;
	}

	progress->setValue(20);

	QProcess builder;
	builder.setEnvironment(QProcess::systemEnvironment());
	builder.setWorkingDirectory(directoryName);
	builder.start(pathToQmake);

	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
		progress->setValue(60);
		builder.start(pathToMake);
		if (builder.waitForFinished()/* && (builder.exitCode() == 0)*/) {
			progress->setValue(80);
			if (mMainWindowInterface->loadPlugin(prefix + metamodelName + "."
					+ extension, normalizeDirName))
			{
				progress->setValue(100);
			} else {
				mErrorReporter->addWarning(tr("Cannot load new editor"));
			}
		}
	}

	if (progress->value() < 80) {
		mErrorReporter->addWarning(tr("Cannot build new editor"));
	}
	progress->setValue(100);
	progress->close();
	delete progress;
}

QDomElement MetamodelGeneratorSupport::diagramElement(const QDomDocument &metamodel) const
{
	return metamodel.elementsByTagName("diagram").at(0).toElement();
}

void MetamodelGeneratorSupport::insertElementsInDiagramSublevel(QDomDocument metamodel
		, const QString &sublevelName, QDomNodeList elements)
{
	QDomNodeList sublevels = metamodel.elementsByTagName(sublevelName);

	if (sublevels.length() > 0) {
		appendElements(sublevels.at(0), elements);
	} else {
		QDomElement sublevel = metamodel.createElement(sublevelName);
		appendElements(sublevel, elements);

		diagramElement(metamodel).appendChild(sublevel);
	}
}

void MetamodelGeneratorSupport::insertElementInDiagramSublevel(QDomDocument metamodel
		, const QString &sublevelName, const QDomElement &element)
{
	QDomNodeList sublevels = metamodel.elementsByTagName(sublevelName);

	if (sublevels.length() > 0) {
		sublevels.at(0).appendChild(element);
	} else {
		QDomElement sublevel = metamodel.createElement(sublevelName);
		sublevel.appendChild(element);

		diagramElement(metamodel).appendChild(sublevel);
	}
}

void MetamodelGeneratorSupport::appendElements(QDomNode parent, QDomNodeList children)
{
	const int count = children.length();
	for (int i = 0; i < count; i++) {
		parent.appendChild(children.at(0));
	}
}

QDomDocument MetamodelGeneratorSupport::loadElementsFromString(const QString &elementsXml)
{
	QDomDocument elements;
	elements.setContent(elementsXml);
	return elements;
}

QStringList MetamodelGeneratorSupport::collectAllGraphicTypesInMetamodel(const QDomDocument &metamodel
		, bool isDisplayedName) const
{
	QDomElement diagram = diagramElement(metamodel);
	QDomNodeList graphicTypes = diagram.elementsByTagName("graphicTypes");

	QString type = isDisplayedName ? "displayedName" : "name";

	QStringList result;
	if (graphicTypes.length() > 0) {
		QDomNodeList children = graphicTypes.at(0).childNodes();
		for (int i = 0; i < children.length(); i++) {
			result.push_back(children.at(i).toElement().attribute(type));
		}
	}

	return result;
}

void MetamodelGeneratorSupport::appendTypesToElement(
		QDomDocument parentDomDocument, QDomElement parent
		, const QString &childName, const QString &prefix
		, const QStringList &elementTypes)
{
	foreach (const QString &elementType, elementTypes) {
		QDomElement child = parentDomDocument.createElement(childName);
		child.setAttribute("type", prefix + "::" + elementType);
		parent.appendChild(child);
	}
}

void MetamodelGeneratorSupport::generateProFile(QDomDocument metamodel
		, const QString &baseMetamodelPath, const QString &qrealSourceFilesPath
		, const QString &newMetamodelName
		, const QString &newEditorPath, const QString &relativeNewEditorPath)
{
	QDir().mkpath(newEditorPath);

	OutFile outpro(newEditorPath + "/" + newMetamodelName + ".pro");
	outpro() << QString("QREAL_XML = %1\n").arg(newMetamodelName + ".xml");
	QDomNodeList include = metamodel.elementsByTagName("include");

	if (include.length() > 0) {
		outpro() << "QREAL_XML_DEPENDS = ";
		for (int i = 0; i < include.length(); i++) {
			const QString includePath = ".." +
					mergePaths(
							baseMetamodelPath.mid(0, baseMetamodelPath.lastIndexOf("/"))
							, include.at(i).toElement().text()
					).mid(qrealSourceFilesPath.length() + 8);

			include.at(i).toElement().childNodes().at(0).toText().setNodeValue(includePath);
			outpro() << includePath;
			outpro() << " ";
		}
	}

	outpro() << "\nQREAL_EDITOR_PATH = " + relativeNewEditorPath + "\n";
	outpro() << QString("ROOT = ../..\n");
	outpro() << QString("include (../editorsSdk/editorsCommon.pri)");

	outpro().flush();
}

QString MetamodelGeneratorSupport::mergePaths(const QString &begin, const QString &end) const
{
	QStringList beginPathList = begin.split("/", QString::SkipEmptyParts);
	QStringList endPathList = end.split("/", QString::SkipEmptyParts);

	int index = 0;
	while (endPathList[index] == "..") {
		index++;
	}

	QString res = "";
	for (int i = 0; i < beginPathList.length() - index; i++) {
		res += "/" + beginPathList[i];
	}
	for (int i = index; i < endPathList.length(); i++) {
		res += "/" + endPathList[i];
	}

	if (res.contains(":")) {
		res = res.mid(1);
	}

	return res;
}
