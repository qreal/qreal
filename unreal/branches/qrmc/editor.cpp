#include "editor.h"
#include "metaCompiler.h"
#include "diagram.h"
#include "classes/type.h"
#include "classes/enumType.h"
#include "utils/outFile.h"

#include <QDebug>

using namespace qReal;

Editor::Editor(MetaCompiler *metaCompiler, qrRepo::RepoApi *api, const qReal::Id &id)
	: mMetaCompiler(metaCompiler), mApi(api), mId(id), mLoadingComplete(false)
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

bool Editor::load()
{
	// load includes
	QStringList includes = mApi->stringProperty(mId, "include").split(",");
	foreach(QString includedMetamodel, includes)
	{
		QString metamodelName = includedMetamodel.section("/", -1).section(".", 0, 0);
		if (metamodelName.isEmpty())
			continue;

		Editor *includedEditor = NULL;
		IdList metamodels = mApi->elementsByType("MetamodelDiagram");
		foreach(Id metamodel, metamodels) {
			if (mApi->name(metamodel) == metamodelName) {
				includedEditor = mMetaCompiler->loadMetaModel(metamodel);
				break;
			}
		}
		if (!includedEditor)
		{
			qDebug() << "ERROR: can't load included metamodel" << metamodelName;
			return false;
		}
		mIncludes.append(includedEditor);
	}

	// TODO: load listeners

	// load diagrams (no resolving yet)
	IdList children = mApi->children(mId);
	IdList diagrams;
	foreach(Id child, children)
		if (child.element() == "MetaEditorDiagramNode")
			diagrams << child;

	foreach(Id diagramId, diagrams)
	{
		qDebug() << "children:" << mApi->children(diagramId).size();
		QString diagramName = mApi->name(diagramId);
		// TODO: nodeName property
		QString diagramDisplayedName = mApi->stringProperty(mId, "displayedName");

		Diagram const *existingDiagram = mMetaCompiler->getDiagram(diagramName);
		if (existingDiagram) {
			qDebug() << "ERROR: diagram" << diagramName << "has been already loaded";
			return false;
		}
		qDebug() << "loading diagram" << diagramName;
		Diagram *diagram = new Diagram(diagramId, mApi, diagramName, diagramDisplayedName, this);
		if (!diagram->init())
		{
			qDebug() << "ERROR: error loading diagram" << diagramName;
			delete diagram;
			return false;
		}
		qDebug() << "diagram" << diagramName << "loaded";
		mDiagrams[diagramName] = diagram;
	}

	// resolve everything
	foreach (Diagram *diagram, mDiagrams.values())
		if (!diagram->resolve())
			return false;

	mLoadingComplete = true;
	return true;
}

MetaCompiler* Editor::metaCompiler()
{
	return mMetaCompiler;
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

QSet<EnumType*> Editor::getAllEnumTypes()
{
	EnumType *current = NULL;
	QSet<EnumType*> result;

	foreach (Diagram *diagram, mDiagrams.values()) {
		foreach (Type *type, diagram->types()) {
			current = dynamic_cast<EnumType*>(type);
			if (current)
				result << current;
		}
	}

	foreach (Editor *editor, mIncludes) {
		result += editor->getAllEnumTypes();
	}

	return result;
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

