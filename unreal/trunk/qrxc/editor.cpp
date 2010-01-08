#include "editor.h"
#include "xmlCompiler.h"
#include "diagram.h"
#include "type.h"

#include <QDebug>

Editor::Editor(QDomDocument domDocument, XmlCompiler *xmlCompiler)
	: mXmlCompiler(xmlCompiler), mXmlDomDocument(domDocument), mLoadingComplete(false)
{}

Editor::~Editor()
{
	foreach(Diagram *diagram, mDiagrams.values())
		if (diagram)
			delete diagram;
}

bool Editor::isLoaded()
{
	return mLoadingComplete;
}

bool Editor::load(QDir const &currentDir)
{
	QDomElement metamodel;

	metamodel = mXmlDomDocument.firstChildElement("metamodel");
	if (metamodel.isNull())
	{
		qDebug() << "ERROR: metamodel tag not found";
		return false;
	}

	//Load includes
	for (QDomElement includeElement = metamodel.firstChildElement("include"); !includeElement.isNull();
		includeElement = includeElement.nextSiblingElement("include"))
	{
		QString includeFileName = includeElement.text();
		QFileInfo includeFileInfo(includeFileName);
		QDir newDir = currentDir;
		newDir.cd(includeFileInfo.canonicalPath());
		Editor *includeFile = mXmlCompiler->loadXmlFile(newDir, includeFileInfo.fileName());
		if (!includeFile)
		{
			qDebug() << "ERROR: can't include file" << includeFileName;
			return false;
		}
		mIncludes.append(includeFile);
	}

	// Load diagrams part one: don't process inherited properties.
	for (QDomElement diagramElement = metamodel.firstChildElement("diagram"); !diagramElement.isNull();
		diagramElement = diagramElement.nextSiblingElement("diagram"))
	{
		QString diagramName = diagramElement.attribute("name");
		Diagram const *existingDiagram = mXmlCompiler->getDiagram(diagramName);
		if (existingDiagram)
		{
			qDebug() << "ERROR: diagram" << diagramName << "is already loaded";
			return false;
		}
		qDebug() << "parsing diagram" << diagramName;
		Diagram *diagram = new Diagram(diagramName, this);
		if (!diagram->init(diagramElement))
		{
			qDebug() << "ERROR: diagram" << diagramName << "can't be parsed";
			delete diagram;
			return false;
		}
		qDebug() << "diagram" << diagramName << "parsed";
		mDiagrams[diagramName] = diagram;
	}

	// Load diagram part two: resolve all dependencies.
	foreach (Diagram *diagram, mDiagrams.values())
		if (!diagram->resolve())
			return false;

	mLoadingComplete = true;
	return true;
}

XmlCompiler* Editor::xmlCompiler()
{
	return mXmlCompiler;
}

Type* Editor::findType(QString const &name)
{
	foreach (Diagram *diagram, mDiagrams.values()) {
		foreach (Type *type, diagram->types()) {
			if (type->qualifiedName() == name)
				return type;
		}
	}

	foreach (Editor *editor, mIncludes) {
		Type *type = editor->findType(name);
		if (type != NULL && type->qualifiedName() == name)
			return type;
	}
	return NULL;
}

Diagram* Editor::findDiagram(QString const &name)
{
	if (mDiagrams.contains(name))
		return mDiagrams[name];
	return NULL;
}

QMap<QString, Diagram*> Editor::diagrams()
{
	return mDiagrams;
}
