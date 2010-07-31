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
	mName = NameNormalizer::normalize(mApi->property(mId, nameOfTheDirectory).toString().section("/", -1));
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
		IdList metamodels = mApi->elementsByType(metamodelDiagram);
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
		if (child.element() == metaEditorDiagramNode)
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

QString Editor::name()
{
	return mApi->name(mId);
}

void Editor::generate(const QString &headerTemplate, const QString &sourceTemplate, const QString &nodeTemplate,const QMap<QString, QString> &utils)
{
	qDebug() << "generating plugin " << mName;

	mUtilsTemplate = utils;
	mSourceTemplate = sourceTemplate;
	mNodeTemplate = nodeTemplate;

	generatePluginHeader(headerTemplate);
	generatePluginSource();
	generateNodeClasses();
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
	generateContainers();
	generateConnections();
	generateUsages();
	generateIsNodeOrEdge();
	generateEnums();
	generatePossibleEdges();

	// inserting plugin name all over the template
	mSourceTemplate.replace(metamodelNameTag, mName);

	// template is ready, writing it into a file
	QTextStream out(&pluginSourceFile);
	out << mSourceTemplate;
	pluginSourceFile.close();
	return true;

}

bool Editor::generateNodeClasses()
{
	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	if (!dir.exists(mName))
		dir.mkdir(mName);
	dir.cd(mName);

	QString fileName = dir.absoluteFilePath(elementsFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	QString generatedSource = "";

	foreach(Diagram *diagram, mDiagrams) {
		generatedSource += diagram->generateNodeClasses(mNodeTemplate);
	}

	// template is ready, writing it into a file
	QTextStream out(&file);
	out << generatedSource;
	file.close();
	return true;

}

void Editor::generateDiagramsMap()
{
	// preparing template for diagramNameMap inits
	QString initNameMapBody = "";
	QString const line = mUtilsTemplate[initDiagramNameMapLineTag];
	foreach(Diagram *diagram, mDiagrams) {
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


class Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const = 0;
};

class Editor::NamesGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateNamesMap(lineTemplate);
	}
};

class Editor::MouseGesturesGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateMouseGesturesMap(lineTemplate);
	}
};

class Editor::PropertiesGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generatePropertiesMap(lineTemplate);
	}
};

class Editor::PropertyDefaultsGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generatePropertyDefaultsMap(lineTemplate);
	}
};

class Editor::ContainersGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateContainers(lineTemplate);
	}
};

class Editor::ConnectionsGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateConnections(lineTemplate);
	}
};

class Editor::UsagesGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateUsages(lineTemplate);
	}
};

class Editor::FactoryGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateFactory(lineTemplate);
	}
};

class Editor::IsNodeOrEdgeGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generateIsNodeOrEdge(lineTemplate);
	}
};

class Editor::PossibleEdgesGenerator: public Editor::MethodGenerator {
public:
	virtual QString generate(Diagram *diagram, QString const &lineTemplate) const {
		return diagram->generatePossibleEdges(lineTemplate);
	}
};

void Editor::generatePluginMethod(const QString &tag, const MethodGenerator &generator, bool isSingleLineMethod)
{
	QString body = "";
	QString const line = isSingleLineMethod
						? mUtilsTemplate[tag]
						: mUtilsTemplate[tag].section("\\n",0,0) + "\n" + mUtilsTemplate[tag].section("\\n",1,1);
	foreach(Diagram *diagram, mDiagrams) {
		body += generator.generate(diagram, line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(tag, body);

}

void Editor::generateNamesMap()
{
	generatePluginMethod(initElementNameMapLineTag, NamesGenerator(), true);
}

void Editor::generateMouseGesturesMap()
{
	generatePluginMethod(initMouseGesturesMapLineTag, MouseGesturesGenerator(), true);
}

void Editor::generatePropertiesMap()
{
	generatePluginMethod(initPropertyTypesMapLineTag, PropertiesGenerator(), true);
}

void Editor::generatePropertyDefaultsMap()
{
	generatePluginMethod(initPropertyDefaultMapLineTag, PropertyDefaultsGenerator(), true);
}

void Editor::generateContainers()
{
	generatePluginMethod(getContainersLineTag, ContainersGenerator(), false);
}

void Editor::generateConnections()
{
	generatePluginMethod(getConnectionsLineTag, ConnectionsGenerator(), false);
}

void Editor::generateUsages()
{
	generatePluginMethod(getUsagesLineTag, UsagesGenerator(), false);
}

void Editor::generateElementsFactory()
{
	generatePluginMethod(getGraphicalObjectLineTag, FactoryGenerator(), false);
}

void Editor::generateIsNodeOrEdge()
{
	generatePluginMethod(getIsNodeOrEdgeLineTag, IsNodeOrEdgeGenerator(), false);
}

void Editor::generatePossibleEdges()
{
	generatePluginMethod(getPossibleEdgesLineTag, PossibleEdgesGenerator(), false);
}

void Editor::generateEnums()
{
	QString body = "";
	QString line = mUtilsTemplate[getEnumsLineTag].section("\\n",0,0)
					+ "\n" + mUtilsTemplate[getEnumsLineTag].section("\\n",1,1);

	foreach(Diagram *diagram, mDiagrams) {
		body += diagram->generateEnums(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getEnumsLineTag, body);
}

