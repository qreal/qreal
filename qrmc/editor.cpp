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

#include "editor.h"
#include "metaCompiler.h"
#include "diagram.h"
#include "classes/type.h"
#include "classes/enumType.h"
#include "utils/nameNormalizer.h"

#include <QDebug>

using namespace qReal;
using namespace qrmc;

Editor::Editor(MetaCompiler *metaCompiler, qrRepo::LogicalRepoApi *api, const qReal::Id &id)
	: mMetaCompiler(metaCompiler), mApi(api), mId(id), mLoadingComplete(false)
{
	mName = mApi->property(mId, nameOfTheDirectory).toString().section("/", -1);
	mNameOfMetamodel = mApi->stringProperty(mId, "name");
	//mName = mName.section("_", 0, 0) + "Plugin";
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

qReal::Id Editor::id()
{
	return mId;
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

		Editor *includedEditor = nullptr;
		IdList metamodels = mApi->elementsByType(metamodelDiagram);
		foreach(Id metamodel, metamodels) {
			if (!mApi->isLogicalElement(metamodel))
				continue;
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
		if (mApi->isLogicalElement(child) && child.element() == metaEditorDiagramNode)
			diagrams << child;

	foreach(Id diagramId, diagrams)
	{
		if (!mApi->isLogicalElement(diagramId))
			continue;

		qDebug() << "\tchildren:" << mApi->children(diagramId).size();
		QString diagramName = mApi->name(diagramId);
		const Diagram *existingDiagram = mMetaCompiler->getDiagram(diagramName);
		if (existingDiagram) {
			qDebug() << "ERROR: diagram" << diagramName << "has been already loaded";
			return false;
		}
		qDebug() << "\tloading diagram" << diagramName;
		Diagram *diagram = new Diagram(diagramId, mApi, this);
		if (!diagram->init())
		{
			qDebug() << "ERROR: error loading diagram" << diagramName;
			delete diagram;
			return false;
		}
		qDebug() << "\tdiagram" << diagramName << "loaded";
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

Type* Editor::findType(const QString &name)
{
	foreach (Diagram *diagram, mDiagrams.values()) {
		foreach (Type *type, diagram->types()) {
			if (type->qualifiedName() == name)
				return type;
		}
	}

	foreach (Editor *editor, mIncludes) {
		Type *type = editor->findType(name);
		if (type != nullptr && type->qualifiedName() == name)
			return type;
	}
	return nullptr;
}

QSet<EnumType*> Editor::getAllEnumTypes()
{
	EnumType *current = nullptr;
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

Diagram* Editor::findDiagram(const QString &name)
{
	if (mDiagrams.contains(name))
		return mDiagrams[name];
	return nullptr;
}

QStringList Editor::getAllPortNames() const
{
	QStringList result;

	for (const Diagram * const diagram : mDiagrams.values()) {
		for (const Type * const type : diagram->types()) {
			if (dynamic_cast<const Port * const>(type)) {
				result << type->name();
			}
		}
	}

	for (const Editor * const editor : mIncludes) {
		result += editor->getAllPortNames();
	}

	result.removeDuplicates();
	return result;
}

QMap<QString, Diagram*> Editor::diagrams()
{
	return mDiagrams;
}

QString Editor::name()
{
	return mName;
}

void Editor::generate(const QString &headerTemplate, const QString &sourceTemplate,
					const QString &nodeTemplate, const QString &edgeTemplate,
					const QString &elementsHeaderTemplate, const QString &resourceTemplate,
					const QString &projectTemplate, QMap<QString, QString> const &utils)
{
	qDebug() << "generating plugin " << mName;

	mUtilsTemplate = utils;
	mSourceTemplate = sourceTemplate;
	mNodeTemplate = nodeTemplate;
	mEdgeTemplate = edgeTemplate;
	mElementsHeaderTemplate = elementsHeaderTemplate;

	generatePluginHeader(headerTemplate);
	generateElementsClasses();
	generatePluginSource();
	generateResourceFile(resourceTemplate);
	generateProjectFile(projectTemplate);
}

bool Editor::generatePluginHeader(const QString &hdrTemplate)
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

	headerTemplate.replace(metamodelNameTag, NameNormalizer::normalize(mNameOfMetamodel));
	QTextStream out(&pluginHeaderFile);
	out.setCodec("UTF-8");
	out << headerTemplate;
	pluginHeaderFile.close();

	return true;
}

bool Editor::generatePluginSource()
{
	qDebug() << "generating plugin source for " << mName;
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
	generatePropertyDisplayedNamesMap();
	generateElementDescriptionMap();
	generateMouseGesturesMap();
	generatePropertiesMap();
	generatePropertyDefaultsMap();
	generateElementsFactory();
	generateContainers();
	generatePropertyNames();
	generateReferenceProperties();
	generatePortTypes();
	generateConnections();
	generateUsages();
	generateIsNodeOrEdge();
	generateEnums();
	generatePossibleEdges();
	generateParentsMap();

	// inserting plugin name all over the template
	mSourceTemplate.replace(metamodelNameTag,  NameNormalizer::normalize(mNameOfMetamodel));

	// template is ready, writing it into a file
	QTextStream out(&pluginSourceFile);
	out.setCodec("UTF-8");
	out << mSourceTemplate;
	pluginSourceFile.close();
	return true;

}

bool Editor::generateElementsClasses()
{
	qDebug() << "generating elements classes for " << mName;
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

	QString generatedNodes;
	QString generatedEdges;

	foreach(Diagram *diagram, mDiagrams) {
		generatedNodes += diagram->generateNodeClasses(mNodeTemplate);
		generatedEdges += diagram->generateEdgeClasses(mEdgeTemplate);
	}

	mElementsHeaderTemplate.replace(nodesListTag, generatedNodes)
						.replace(edgesListTag, generatedEdges);
	// template is ready, writing it into a file
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << mElementsHeaderTemplate;
	file.close();
	return true;

}

bool Editor::generateResourceFile(const QString &resourceTemplate)
{
	qDebug() << "generating resource file for " << mName;
	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	if (!dir.exists(mName))
		dir.mkdir(mName);
	dir.cd(mName);

	QString fileName = dir.absoluteFilePath(resourceFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	QString resourceBody = "";
	const QString line = mUtilsTemplate[sdfFileTag];
	foreach(Diagram *diagram, mDiagrams) {
		resourceBody += diagram->generateResourceFile(line);
	}

	QString resourceGenerated = resourceTemplate;
	resourceGenerated.replace(sdfFileTag, resourceBody);

	// template is ready, writing it into a file
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << resourceGenerated;
	file.close();
	return true;

}

bool Editor::generateProjectFile(const QString &proTemplate)
{
	QString projectTemplate = proTemplate;
	qDebug() << "generating project file for " << mName;
	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	if (!dir.exists(mName))
		dir.mkdir(mName);
	dir.cd(mName);
	QString nameOfMetamodel = mApi->stringProperty(mId, "name");
	QString fileName = dir.absoluteFilePath(nameOfMetamodel + ".pro");
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	projectTemplate.replace(metamodelNameTag, mNameOfMetamodel);
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << projectTemplate;
	file.close();

	return true;
}

void Editor::generateDiagramsMap()
{
	// preparing template for diagramNameMap inits
	QString initNameMapBody = "";
	const QString line = mUtilsTemplate[initDiagramNameMapLineTag];
	foreach(Diagram *diagram, mDiagrams) {
		QString newline = line;
		initNameMapBody += newline.replace(diagramDisplayedNameTag, diagram->displayedName())
				.replace(diagramNameTag, diagram->name()) + endline;
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initDiagramNameMapLineTag, initNameMapBody);
}

void Editor::generateDiagramNodeNamesMap()
{
	// preparing template for diagramNodeNameMap inits
	QString initNodeNameMapBody = "";
	const QString line = mUtilsTemplate[initDiagramNodeNameMapLineTag];
	foreach(Diagram *diagram, mDiagrams) {
		QString newline = line;
		initNodeNameMapBody += newline.replace(diagramNodeNameTag, diagram->nodeName())
								.replace(diagramNameTag, diagram->name()) + endline;
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(initDiagramNodeNameMapLineTag, initNodeNameMapBody);
}


class Editor::MethodGenerator {
public:
	virtual ~MethodGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const = 0;
};

class Editor::NamesGenerator: public Editor::MethodGenerator {
public:
	~NamesGenerator() override {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateNamesMap(lineTemplate);
	}
};

class Editor::MouseGesturesGenerator: public Editor::MethodGenerator {
public:
	~MouseGesturesGenerator() override {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateMouseGesturesMap(lineTemplate);
	}
};

class Editor::PropertiesGenerator: public Editor::MethodGenerator {
public:
	virtual ~PropertiesGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generatePropertiesMap(lineTemplate);
	}
};

class Editor::PropertyDefaultsGenerator: public Editor::MethodGenerator {
public:
	virtual ~PropertyDefaultsGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generatePropertyDefaultsMap(lineTemplate);
	}
};

class Editor::PropertyDisplayedNamesGenerator: public Editor::MethodGenerator {
public:
	virtual ~PropertyDisplayedNamesGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generatePropertyDisplayedNamesMap(lineTemplate);
	}
};

class Editor::ElementDescriptionGenerator: public Editor::MethodGenerator {
public:
	virtual ~ElementDescriptionGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateElementDescriptionMap(lineTemplate);
	}
};

class Editor::ParentsMapGenerator: public Editor::MethodGenerator {
public:
	virtual ~ParentsMapGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateParentsMap(lineTemplate);
	}
};

class Editor::ContainersGenerator: public Editor::MethodGenerator {
public:
	virtual ~ContainersGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateContainers(lineTemplate);
	}
};

class Editor::ReferencePropertiesGenerator: public Editor::MethodGenerator {
public:
	virtual ~ReferencePropertiesGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateReferenceProperties(lineTemplate);
	}
};

class Editor::PortTypesGenerator: public Editor::MethodGenerator {
public:
	virtual ~PortTypesGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generatePortTypes(lineTemplate);
	}
};

class Editor::PropertyNameGenerator: public Editor::MethodGenerator {
public:
	virtual ~PropertyNameGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generatePropertyName(lineTemplate);
	}
};

class Editor::ConnectionsGenerator: public Editor::MethodGenerator {
public:
	virtual ~ConnectionsGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateConnections(lineTemplate);
	}
};

class Editor::UsagesGenerator: public Editor::MethodGenerator {
public:
	virtual ~UsagesGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateUsages(lineTemplate);
	}
};

class Editor::FactoryGenerator: public Editor::MethodGenerator {
public:
	virtual ~FactoryGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateFactory(lineTemplate);
	}
};

class Editor::IsNodeOrEdgeGenerator: public Editor::MethodGenerator {
public:
	virtual ~IsNodeOrEdgeGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generateIsNodeOrEdge(lineTemplate);
	}
};

class Editor::PossibleEdgesGenerator: public Editor::MethodGenerator {
public:
	virtual ~PossibleEdgesGenerator() {}
	virtual QString generate(Diagram *diagram, const QString &lineTemplate) const {
		return diagram->generatePossibleEdges(lineTemplate);
	}
};

void Editor::generatePluginMethod(const QString &tag, const MethodGenerator &generator)
{
	QString body = "";
	const QString line = mUtilsTemplate[tag].replace("\\n", "\n");
	foreach(Diagram *diagram, mDiagrams) {
		body += generator.generate(diagram, line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(tag, body);

}

void Editor::generateNamesMap()
{
	generatePluginMethod(initElementNameMapLineTag, NamesGenerator());
}

void Editor::generatePropertyDisplayedNamesMap()
{
	generatePluginMethod(initPropertyDisplayedNamesTag, PropertyDisplayedNamesGenerator());
}

void Editor::generateElementDescriptionMap()
{
	generatePluginMethod(elementDescriptionMapTag, ElementDescriptionGenerator());
}

void Editor::generateParentsMap()
{
	generatePluginMethod(initParentsMapLineTag, ParentsMapGenerator());
}

void Editor::generateMouseGesturesMap()
{
	generatePluginMethod(initMouseGesturesMapLineTag, MouseGesturesGenerator());
}

void Editor::generatePropertiesMap()
{
	generatePluginMethod(initPropertyTypesMapLineTag, PropertiesGenerator());
}

void Editor::generatePropertyDefaultsMap()
{
	generatePluginMethod(initPropertyDefaultMapLineTag, PropertyDefaultsGenerator());
}

void Editor::generateContainers()
{
	generatePluginMethod(getContainersLineTag, ContainersGenerator());
}

void Editor::generateReferenceProperties()
{
	generatePluginMethod(getReferencePropertiesLineTag, ReferencePropertiesGenerator());
}

void Editor::generatePortTypes()
{
	generatePluginMethod(getPortTypesLineTag, PortTypesGenerator());
}

void Editor::generatePropertyNames()
{
	generatePluginMethod(getPropertyNameTag, PropertyNameGenerator());
}

void Editor::generateConnections()
{
	generatePluginMethod(getConnectionsLineTag, ConnectionsGenerator());
}

void Editor::generateUsages()
{
	generatePluginMethod(getUsagesLineTag, UsagesGenerator());
}

void Editor::generateElementsFactory()
{
	generatePluginMethod(getGraphicalObjectLineTag, FactoryGenerator());
}

void Editor::generateIsNodeOrEdge()
{
	generatePluginMethod(getIsNodeOrEdgeLineTag, IsNodeOrEdgeGenerator());
}

void Editor::generatePossibleEdges()
{
	generatePluginMethod(getPossibleEdgesLineTag, PossibleEdgesGenerator());
}

void Editor::generateEnums()
{
	QString body = "";
	QString line = mUtilsTemplate[getEnumsLineTag].replace("\\n", "\n");

	foreach(Diagram *diagram, mDiagrams) {
		body += diagram->generateEnums(line);
	}
	// inserting generated lines into main template
	mSourceTemplate.replace(getEnumsLineTag, body);
}

