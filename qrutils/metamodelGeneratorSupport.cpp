#include "metamodelGeneratorSupport.h"

#include <QProcess>
#include <QApplication>
#include <QProgressBar>
#include <QDesktopWidget>

#include "xmlUtils.h"
#include "outFile.h"

#include "../qrkernel/settingsManager.h"

using namespace qReal;
using namespace utils;

MetamodelGeneratorSupport::MetamodelGeneratorSupport(ErrorReporterInterface *errorReporter,
		qReal::gui::MainWindowInterpretersInterface *mainWindowInterface)
		: mErrorReporter(errorReporter)
		, mMainWindowInterface(mainWindowInterface)
{
}

MetamodelGeneratorSupport::~MetamodelGeneratorSupport()
{
}

QDomDocument MetamodelGeneratorSupport::loadMetamodelFromFile(QString const &metamodelPath)
{
	return xmlUtils::loadDocument(metamodelPath);
}

void MetamodelGeneratorSupport::saveMetamodelInFile(QDomDocument const &metamodel,
		QString const &metamodelPath)
{
	QString const dirPath = metamodelPath.mid(0, metamodelPath.lastIndexOf("/"));
	QDir().mkpath(dirPath);
	
	OutFile out(metamodelPath);
	out() << metamodel.toString();
	out().flush();
}

void MetamodelGeneratorSupport::loadPlugin(QString const &directoryName
		, QString const &metamodelName
		, QString const &pathToQmake
		, QString const &pathToMake
		, QString const &extension
		, QString const &prefix)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	if ((pathToQmake == "") || (pathToMake == "") || (extension == "")) {
		mErrorReporter->addWarning(tr("Please, fill compiler settings"));
		return;
	}

	QString const normalizeDirName = metamodelName.at(0).toUpper() + metamodelName.mid(1);

	QProgressBar * const progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();

	QApplication::processEvents();

	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2,
			screenRect.height() / 2 - progressBarHeight / 2);
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
		if (builder.waitForFinished() && (builder.exitCode() == 0)) {
			progress->setValue(80);
			if (mMainWindowInterface->loadPlugin(prefix + metamodelName + "."
					+ extension, normalizeDirName))
			{
				progress->setValue(100);
			}
		}
	}

	if (progress->value() != 100) {
		mErrorReporter->addWarning(tr("Cannot load new editor"));
	}
	progress->setValue(100);
	progress->close();
	delete progress;
}

QDomElement MetamodelGeneratorSupport::getDiagramElement(QDomDocument const &metamodel) const
{
	return metamodel.elementsByTagName("diagram").at(0).toElement();
}

void MetamodelGeneratorSupport::insertElementsInDiagramSublevel(QDomDocument metamodel,
		QString const &sublevelName, QDomNodeList elements)
{
	QDomNodeList sublevels = metamodel.elementsByTagName(sublevelName);
	
	if (sublevels.length() > 0) {
		appendElements(sublevels.at(0), elements);
	} else {
		QDomElement sublevel = metamodel.createElement(sublevelName);
		appendElements(sublevel, elements);
		
		getDiagramElement(metamodel).appendChild(sublevel);
	}
}

void MetamodelGeneratorSupport::insertElementInDiagramSublevel(QDomDocument metamodel,
		const QString &sublevelName, QDomElement const &element)
{
	QDomNodeList sublevels = metamodel.elementsByTagName(sublevelName);
	
	if (sublevels.length() > 0) {
		sublevels.at(0).appendChild(element);
	} else {
		QDomElement sublevel = metamodel.createElement(sublevelName);
		sublevel.appendChild(element);
		
		getDiagramElement(metamodel).appendChild(sublevel);
	}
}

void MetamodelGeneratorSupport::appendElements(QDomNode parent, QDomNodeList children)
{
	int const count = children.length();
	for (int i = 0; i < count; i++) {
		parent.appendChild(children.at(0));
	}
}

QDomDocument MetamodelGeneratorSupport::loadElementsFromString(QString const &elementsXml)
{
	QDomDocument elements;
	elements.setContent(elementsXml);
	return elements;
}

QStringList MetamodelGeneratorSupport::collectAllGraphicTypesInMetamodel(QDomDocument const &metamodel) const
{
	QDomElement diagram = getDiagramElement(metamodel);
	QDomNodeList graphicTypes = diagram.elementsByTagName("graphicTypes");
	
	QStringList result;
	if (graphicTypes.length() > 0) {
		QDomNodeList children = graphicTypes.at(0).childNodes();
		for (int i = 0; i < children.length(); i++) {
			result.push_back(children.at(i).toElement().attribute("name"));
		}
	}

	return result;
}

void MetamodelGeneratorSupport::appendTypesToElement(
		QDomDocument parentDomDocument, QDomElement parent,
		QString const &childName, QString const &prefix,
		QStringList const &elementTypes)
{
	foreach (QString const &elementType, elementTypes) {
		QDomElement child = parentDomDocument.createElement(childName);
		child.setAttribute("type", prefix + "::" + elementType);
		parent.appendChild(child);
	}
}

void MetamodelGeneratorSupport::generateProFile(QDomDocument metamodel,
		QString const &baseMetamodelPath, QString const &qrealSourceFilesPath,
		QString const &newMetamodelName,
		QString const &newEditorPath, QString const &relativeNewEditorPath)
{
	QDir().mkpath(newEditorPath);
	
	OutFile outpro(newEditorPath + "/" + newMetamodelName + ".pro");
	outpro() << QString("QREAL_XML = %1\n").arg(newMetamodelName + ".xml");
	QDomNodeList include = metamodel.elementsByTagName("include");
	
	if (include.length() > 0) {
		outpro() << "QREAL_XML_DEPENDS = ";
		for (int i = 0; i < include.length(); i++) {
			QString const includePath = ".." +
					mergePaths(
							baseMetamodelPath.mid(0, baseMetamodelPath.lastIndexOf("/")),
							include.at(i).toElement().text()
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

QString MetamodelGeneratorSupport::mergePaths(QString const &begin, QString const &end) const
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
