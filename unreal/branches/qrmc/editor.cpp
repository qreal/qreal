#include "editor.h"
#include "metaCompiler.h"
#include "diagram.h"
#include "classes/type.h"
#include "classes/enumType.h"
#include "utils/outFile.h"
#include "utils/nameNormalizer.h"

#include <QDebug>

using namespace qReal;

Editor::Editor(MetaCompiler *metaCompiler, qrRepo::RepoApi *api, const qReal::Id &id)
	: mMetaCompiler(metaCompiler), mApi(api), mId(id), mLoadingComplete(false)
{
	mName = NameNormalizer::normalize(mApi->property(mId, "name of the directory").toString().section("/", -1));
}

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
		QString metamodelName = includedMetamodel.section("/", -1).section(".", 0, 0).trimmed();
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
		Diagram const *existingDiagram = mMetaCompiler->getDiagram(diagramName);
		if (existingDiagram) {
			qDebug() << "ERROR: diagram" << diagramName << "has been already loaded";
			return false;
		}
		qDebug() << "loading diagram" << diagramName;
		Diagram *diagram = new Diagram(diagramId, mApi, this);
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
//		qDebug() << "searching diagram" << editor->name() << name << "includes size" << mIncludes.size();
		Type *type = editor->findType(name);
//		if (type)
//			qDebug() << "from includes: " << type->qualifiedName() << name;
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

QString Editor::name()
{
	return mApi->name(mId);
}

void Editor::generate(const QString &headerTemplate, const QString &sourceTemplate, const QMap<QString, QString> &utils)
{
	qDebug() << "generating plugin " << mName;

	mUtilsTemplate = utils;
	mSourceTemplate = sourceTemplate;

	generatePluginHeader(headerTemplate);
	generatePluginSource();
}

bool Editor::generatePluginHeader(QString const &hdrTemplate)
{
	QString headerTemplate = hdrTemplate;
	qDebug() << "generating plugin header for " << mName;
	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	if (!dir.exists(mName))
		dir.mkdir(mName);
	dir.cd(mName);

	QString fileName = dir.absoluteFilePath(pluginHeaderName);
	QFile pluginHeaderFile(fileName);
	if (!pluginHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	headerTemplate.replace(metamodelNameTag, mName); // header requires just plugin name customization
	QTextStream out(&pluginHeaderFile);
	out << headerTemplate;
	pluginHeaderFile.close();

	return true;
}

bool Editor::generatePluginSource()
{
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
	generateDiagramNodeNamesMap();
	generateNamesMap();
	generateMouseGesturesMap();
	generatePropertiesMap();
	generatePropertyDefaultsMap();
	generateElementsFactory();
	generateContaners();
	generateConnections();
	generateUsages();

	// inserting plugin name all over the template
	mSourceTemplate.replace(metamodelNameTag, mName);

	// template is ready, writing it into a file
	QTextStream out(&pluginSourceFile);
	out << mSourceTemplate;
	pluginSourceFile.close();
	return true;

}

void Editor::generateDiagramsMap()
{
	// preparing template for diagramNameMap inits
	QString initNameMapBody = "";
	QString const line = mUtilsTemplate[initDiagramNameMapLineTag];
	foreach(Diagram *diagram, mDiagrams)	{
		QString newline = line;
		initNameMapBody += newline.replace(diagramDisplayedNameTag, diagram->displayedName())
								.replace(diagramNameTag, diagram->name()) + "\n";
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initDiagramNameMapLineTag, initNameMapBody);
}

void Editor::generateDiagramNodeNamesMap()
{
	// preparing template for diagramNodeNameMap inits
	QString initNodeNameMapBody = "";
	QString const line = mUtilsTemplate[initDiagramNodeNameMapLineTag];
	foreach(Diagram *diagram, mDiagrams)	{
		QString newline = line;
		initNodeNameMapBody += newline.replace(diagramNodeNameTag, diagram->nodeName())
								.replace(diagramNameTag, diagram->name()) + "\n";
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initDiagramNodeNameMapLineTag, initNodeNameMapBody);
}

void Editor::generateNamesMap()
{
	QString initNameMapBody = "";
	QString const line = mUtilsTemplate[initElementNameMapLineTag];
	foreach(Diagram *diagram, mDiagrams)	{
		initNameMapBody += diagram->generateNamesMap(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initElementNameMapLineTag, initNameMapBody);
}

void Editor::generateMouseGesturesMap()
{
	QString initMouseGesturesMapBody = "";
	QString const line = mUtilsTemplate[initMouseGesturesMapLineTag];
	foreach(Diagram *diagram, mDiagrams)	{
		initMouseGesturesMapBody += diagram->generateMouseGesturesMap(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initMouseGesturesMapLineTag, initMouseGesturesMapBody);
}

void Editor::generatePropertiesMap()
{
	QString initPropertiesMapBody = "";
	QString const line = mUtilsTemplate[initPropertyTypesMapLineTag];
	foreach(Diagram *diagram, mDiagrams)	{
		initPropertiesMapBody += diagram->generatePropertiesMap(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initPropertyTypesMapLineTag, initPropertiesMapBody);
}

void Editor::generatePropertyDefaultsMap()
{
	QString initPropertyDefaultMapBody = "";
	QString const line = mUtilsTemplate[initPropertyDefaultMapLineTag];
	foreach(Diagram *diagram, mDiagrams)	{
		initPropertyDefaultMapBody += diagram->generatePropertyDefaultsMap(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initPropertyDefaultMapLineTag, initPropertyDefaultMapBody);
}

void Editor::generateElementsFactory()
{
	QString factoryBody = "";
	QString line = mUtilsTemplate[getGraphicalObjectLineTag].section("\\n",0,0)
					+ "\n" + mUtilsTemplate[getGraphicalObjectLineTag].section("\\n",1,1);

	foreach(Diagram *diagram, mDiagrams)	{
		factoryBody += diagram->generateFactory(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getGraphicalObjectLineTag, factoryBody);
}

void Editor::generateContaners()
{
	QString containersBody = "";
	QString line = mUtilsTemplate[getContainersLineTag].section("\\n",0,0)
					+ "\n" + mUtilsTemplate[getContainersLineTag].section("\\n",1,1);

	foreach(Diagram *diagram, mDiagrams)	{
		containersBody += diagram->generateContainers(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getContainersLineTag, containersBody);

}

void Editor::generateConnections()
{
	QString connectionsBody = "";
	QString line = mUtilsTemplate[getConnectionsLineTag].section("\\n",0,0)
					+ "\n" + mUtilsTemplate[getConnectionsLineTag].section("\\n",1,1);

	foreach(Diagram *diagram, mDiagrams)	{
		connectionsBody += diagram->generateConnections(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getConnectionsLineTag, connectionsBody);

}

void Editor::generateUsages()
{
	QString usagesBody = "";
	QString line = mUtilsTemplate[getUsagesLineTag].section("\\n",0,0)
					+ "\n" + mUtilsTemplate[getUsagesLineTag].section("\\n",1,1);

	foreach(Diagram *diagram, mDiagrams)	{
		usagesBody += diagram->generateUsages(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getUsagesLineTag, usagesBody);

}
