#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "pluginSource.h"
#include "defs.h"

PluginSource::PluginSource(QString const &name) : mName(name)
{
}

PluginSource::~PluginSource()
{
}

void PluginSource::init(qrRepo::RepoApi &repo, qReal::Id metamodelId)
{
	mApi = &repo; // will need it on generation stage
	qDebug() << "init id" << metamodelId.toString();
	foreach(qReal::Id diagramId, repo.children(metamodelId)) {
		initDiagram(repo, diagramId);
	}
	qDebug() << "diagrams: " << mDiagrams.size();
}

void PluginSource::initDiagram(qrRepo::RepoApi &repo, qReal::Id diagramId)
{
	Diagram diagram;
	diagram.name = repo.name(diagramId);
	diagram.displayedName = repo.property(diagramId, "displayedName").toString();
	diagram.id = diagramId;

	foreach(qReal::Id elementId, repo.children(diagramId)) {
		Element element;
		element.name = repo.name(elementId);
		element.displayedName = repo.stringProperty(elementId, "displayedName");
		element.id = elementId;
		if (element.displayedName.isEmpty()) // in case of incorrect metamodels
			element.displayedName = element.name;
		qDebug() << element.name << elementId.toString() << elementId.element();
		diagram.elements << element;
	}

	mDiagrams << diagram;

}

bool PluginSource::generate(QString const &sourceTemplate, QMap<QString, QString> const &utils)
{
	mUtilsTemplate = utils;
	mSourceTemplate = sourceTemplate;

	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	if (!dir.exists(mName))
		dir.mkdir(mName);
	dir.cd(mName);

	QString fileName = dir.absoluteFilePath(pluginSourceName);
	QFile pluginSourceFile(fileName);
	if (!pluginSourceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	generateDiagramsMap();
	generateElementsMap();
	generateMouseGesturesMap();
	generatePropertyTypesMap();
	generateGetGraphicalObject();

	// inserting plugin name all over the template
	mSourceTemplate.replace(metamodelNameTag, mName);

	// template is ready, writing it into a file
	QTextStream out(&pluginSourceFile);
	out << mSourceTemplate;
	pluginSourceFile.close();
	return true;
}

void PluginSource::generateDiagramsMap()
{
	// preparing template for diagramNameMap inits
	QString initNameMapBody = "";
	QString const line = mUtilsTemplate[initDiagramNameMapLineTag];
	foreach(Diagram diagram, mDiagrams)	{
		QString newline = line;
		initNameMapBody += newline.replace(diagramDisplayedNameTag, diagram.displayedName).replace(diagramNameTag, diagram.name) + "\n";
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initDiagramNameMapLineTag, initNameMapBody);
}

void PluginSource::generateElementsMap()
{
	// filling template for elementsNameMap inits
	QString initElementsMapBody = "";
	QString const line = mUtilsTemplate[initElementNameMapLineTag];
	foreach(Diagram diagram, mDiagrams)	{
		foreach(Element el, diagram.elements) {
			// here we need only edges and nodes that have graphical representation

			if (((el.id.element() != "MetaEntityNode") && (el.id.element() != "MetaEntityEdge") /*&& (el.id.element() != "Importation")*/))
				// we acutally do need some of these "Importation"s
				// TODO: request needed diagrams and resolve all imports as qrxc does
				continue;

//			qDebug() << el.id.element() << "has set shape prop: " << mApi->hasProperty(el.id, "set Shape");

//			if (el.id.element() == "MetaEntityNode") {
//				if (mApi->stringProperty(el.id, "set Shape").isEmpty())
//					continue;
//				else
//					qDebug() << el.displayedName << el.name << !mApi->stringProperty(el.id, "set Shape").isEmpty();
//			}

			QString newline = line;
			initElementsMapBody += newline.replace(elementNameTag, el.name)
								.replace(elementDisplayedNameTag, el.displayedName)
								.replace(diagramNameTag, diagram.name)
								 + "\n";
		}
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initElementNameMapLineTag, initElementsMapBody);
}

void PluginSource::generateMouseGesturesMap()
{
	// preparing template for elementMouseGesturesMap map inits
	QString initMouseGesturesMapBody = "";
	QString const line = mUtilsTemplate[initMouseGesturesMapLineTag];
	foreach(Diagram diagram, mDiagrams)	{
		foreach(Element el, diagram.elements) {
			if (!mApi->hasProperty(el.id, "Path")) // skipping elements with no such property (links and others)
				continue;
			QString path = mApi->stringProperty(el.id, "Path");
			if (path.isEmpty()) // empty paths are not needed
				continue;
			QString newline = line;
			initMouseGesturesMapBody += newline.replace(elementNameTag, el.name)
								.replace(gesturePathTag, path)
								.replace(diagramNameTag, diagram.name)
								 + "\n";
		}
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initMouseGesturesMapLineTag, initMouseGesturesMapBody);
}

void PluginSource::generatePropertyTypesMap()
{
	// preparing template for propertyTypes map inits
	QString initPropTypesMapBody = "";
	QString const line = mUtilsTemplate[initPropertyTypesMapLineTag];
	foreach(Diagram diagram, mDiagrams)	{
		QString newline = line;
		// TODO: resolve properties
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initPropertyTypesMapLineTag, initPropTypesMapBody);
}

void PluginSource::generateGetGraphicalObject()
{
	// preparing template for getGraphicalObject factory
	QString getGraphicalObjectBody = "";
	QString line = mUtilsTemplate[getGraphicalObjectLineTag].section("\\n",0,0) + "\n" + mUtilsTemplate[getGraphicalObjectLineTag].section("\\n",1,1);
	bool isFirstLine = true;
	foreach(Diagram diagram, mDiagrams)	{
		foreach(Element el, diagram.elements) {

			if (((el.id.element() != "MetaEntityNode") && (el.id.element() != "MetaEntityEdge") /*&& (el.id.element() != "Importation")*/))
				continue;

			QString newline = line;
			if (isFirstLine)
				line.replace("if", "else if");
			getGraphicalObjectBody += newline.replace(elementNameTag, el.name) + "\n";
			isFirstLine = false;
		}
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getGraphicalObjectLineTag, getGraphicalObjectBody);
}
