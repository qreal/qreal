#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "pluginSource.h"
#include "nameNormalizer.h"
#include "defs.h"

using namespace qReal;

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
	foreach(qReal::Id diagramId, mApi->children(metamodelId)) {
		initDiagram(diagramId);
	}
	qDebug() << "  diagrams: " << mDiagrams.size();
	resolveImports();
	resolveElements();
	updateIsGraphicalProperty();
}

void PluginSource::initDiagram(qReal::Id &diagramId)
{
	Diagram diagram;
	diagram.name = mApi->name(diagramId);
	diagram.displayedName = mApi->property(diagramId, "displayedName").toString();
	diagram.id = diagramId;

	foreach(qReal::Id elementId, mApi->children(diagramId)) {
		Element element;
		if (elementId.element() == importType)
			element.name = mApi->stringProperty(elementId, "as");
		else
			element.name = mApi->name(elementId);
		element.displayedName = mApi->stringProperty(elementId, "displayedName");
		element.id = elementId;
		element.isGraphicalObject = false;
		if (element.displayedName.isEmpty()) // in case of incorrect metamodels
			element.displayedName = element.name;
		initElement(element, elementId);
//		qDebug() << element.name << elementId.toString() << elementId.element();
		diagram.elements << element;
	}

	mDiagrams << diagram;

}

void PluginSource::initElement(Element &el, Id &id)
{
	IdList children = mApi->children(id);
	foreach(Id child, children) {
		if (child.element() == "MetaEntityParent")
			el.parents << child;
		else if (child.element() == "MetaEntity_Attribute") {
			Property prop;
			prop.name = mApi->stringProperty(child, "name");
			prop.type = mApi->stringProperty(child, "attributeType");
			prop.defaultValue = mApi->stringProperty(child, "defaultValue");
			el.properties << prop;
		}
	}
}


void PluginSource::resolveImports()
{
	for (int i = 0; i < mDiagrams.size(); ++i)	{
		for (int j = 0; j < mDiagrams[i].elements.size(); ++j) {
			Element el = mDiagrams[i].elements[j];
			if (el.id.element() != importType)
				continue;

			Id sourceElement = findElement(mApi->stringProperty(el.id, "Imported from"), mApi->name(el.id));
			mDiagrams[i].elements[j].id = sourceElement;
			el.id = sourceElement;
		}
	}

}

void PluginSource::resolveElements()
{
	for (int i = 0; i < mDiagrams.size(); ++i)	{
		for (int j = 0; j < mDiagrams[i].elements.size(); ++j) {
			Element el = mDiagrams[i].elements[j];
			qDebug() << el.name;
			IdList props = mApi->children(el.id);
			foreach(Id propId, props) {

			}
		}
	}
}

qReal::Id PluginSource::findElement(QString diagram, QString name)
{
	IdList diagramNodes = mApi->elementsByType("MetaEditorDiagramNode");
	foreach(Id id, diagramNodes) {
		if (mApi->name(id) == diagram) {// found source diagram
			IdList children = mApi->children(id);
			foreach(Id child, children) {
				if (mApi->name(child) == name)
					return child;
			}
		}
	}

	qDebug() << "could not resolve imported element" << name << "from " << diagram;
	return ROOT_ID;
}

void PluginSource::updateIsGraphicalProperty()
{
	for (int i = 0; i < mDiagrams.size(); ++i)	{
		for (int j = 0; j < mDiagrams[i].elements.size(); ++j) {
			Element el = mDiagrams[i].elements[j];
			mDiagrams[i].elements[j].isGraphicalObject = ((el.id.element() == "MetaEntityEdge")
						 || (mApi->hasProperty(el.id, setShapeProperty)
							 && !(mApi->stringProperty(el.id, setShapeProperty).isEmpty())));

		}
	}
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
		initNameMapBody += newline.replace(diagramDisplayedNameTag, diagram.displayedName)
								.replace(diagramNameTag, diagram.name) + "\n";
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

			// imports have already been resolved into nodes and edges
			if (((el.id.element() != "MetaEntityNode") && (el.id.element() != "MetaEntityEdge")))
				continue;

			if (!el.isGraphicalObject)
				continue;

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
			if (!mApi->hasProperty(el.id, "Path"))
				// skipping elements with no such property (links and others)
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
		foreach(Element element, diagram.elements) {
			if (((element.id.element() != "MetaEntityNode") && (element.id.element() != "MetaEntityEdge")))
				continue;

			if (!element.isGraphicalObject)
					continue;

			foreach(Property property, element.properties) {
				QString newline = line;
				initPropTypesMapBody += newline.replace(elementNameTag, NameNormalizer::normalize(element.name))
								.replace(propertyNameTag, property.name)
								.replace(propertyTypeTag, property.type)
								 + "\n";
			}
		}

	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initPropertyTypesMapLineTag, initPropTypesMapBody);
}

void PluginSource::generateGetGraphicalObject()
{
	// preparing template for getGraphicalObject factory
	QString getGraphicalObjectBody = "";
	QString line = mUtilsTemplate[getGraphicalObjectLineTag]
					.section("\\n",0,0) + "\n" + mUtilsTemplate[getGraphicalObjectLineTag]
					.section("\\n",1,1);
	bool isFirstLine = true;
	foreach(Diagram diagram, mDiagrams)	{
		foreach(Element el, diagram.elements) {

			// imports have already been resolved into nodes and edges
			if (((el.id.element() != "MetaEntityNode") && (el.id.element() != "MetaEntityEdge")))
				continue;

			if (!el.isGraphicalObject)
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
