/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <qrrepo/logicalRepoApi.h>

#include "metaCompiler.h"
#include "diagram.h"
#include "classes/type.h"
#include "classes/port.h"
#include "classes/enumType.h"
#include "utils/nameNormalizer.h"

using namespace qReal;
using namespace qrmc;

Editor::Editor(MetaCompiler &metaCompiler, const qrRepo::LogicalRepoApi &api, const qReal::Id &id
		, const QString &targetDirectory)
	: mMetaCompiler(metaCompiler)
	, mApi(api)
	, mId(id)
	, mTargetDirectory(targetDirectory)
{
	mName = mApi.property(mId, nameOfTheDirectory).toString().section("/", -1);
	mNameOfMetamodel = mApi.stringProperty(mId, "name");
}

Editor::~Editor()
{
	qDeleteAll(mDiagrams);
}

bool Editor::isLoaded() const
{
	return mLoadingComplete;
}

qReal::Id Editor::id() const
{
	return mId;
}

bool Editor::load()
{
	if (!loadIncludes()) {
		return false;
	}

	if (!loadDiagrams()) {
		return false;
	}

	if (!resolve()) {
		return false;
	}

	mLoadingComplete = true;
	return true;
}

MetaCompiler &Editor::metaCompiler()
{
	return mMetaCompiler;
}

Type* Editor::findType(const QString &name) const
{
	for (const Diagram * const diagram : mDiagrams.values()) {
		for (Type * const type : diagram->types()) {
			if (type->qualifiedName() == name) {
				return type;
			}
		}
	}

	for (const Editor * const editor : mIncludes) {
		Type * const type = editor->findType(name);
		if (type != nullptr && type->qualifiedName() == name) {
			return type;
		}
	}

	return nullptr;
}

QSet<EnumType*> Editor::getAllEnumTypes() const
{
	QSet<EnumType*> result;

	for (const Diagram * const diagram : mDiagrams.values()) {
		for (Type * const type : diagram->types()) {
			const auto current = dynamic_cast<EnumType*>(type);
			if (current) {
				result << current;
			}
		}
	}

	for (const Editor * const editor : mIncludes) {
		result += editor->getAllEnumTypes();
	}

	return result;
}

Diagram* Editor::findDiagram(const QString &name) const
{
	if (mDiagrams.contains(name)) {
		return mDiagrams[name];
	}

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

QMap<QString, Diagram*> Editor::diagrams() const
{
	return mDiagrams;
}

QString Editor::name() const
{
	return mName;
}

void Editor::generate(const QString &headerTemplate, const QString &sourceTemplate,
					const QString &nodeTemplate, const QString &edgeTemplate,
					const QString &elementsHeaderTemplate, const QString &resourceTemplate,
					const QString &projectTemplate, const QMap<QString, QString> &utils)
{
	if (!mLoadingComplete) {
		qDebug() << "Trying to generate editor that is not loaded yet";
		return;
	}

	qDebug() << "generating plugin " << mName << "into" << QFileInfo(mTargetDirectory).canonicalPath();

	mUtilsTemplate = utils;
	mSourceTemplate = sourceTemplate;
	mNodeTemplate = nodeTemplate;
	mEdgeTemplate = edgeTemplate;
	mElementsHeaderTemplate = elementsHeaderTemplate;

	QDir dir;
	if (QFileInfo::exists(mTargetDirectory)) {
		dir.mkdir(mTargetDirectory);
	}

	if (!dir.exists(mName)) {
		dir.mkdir(mName);
	}

	generatePluginHeader(headerTemplate);
	generatePluginSource();
	generateElementsClasses();
	generateResourceFile(resourceTemplate);
	generateProjectFile(projectTemplate);
}

bool Editor::generatePluginHeader(const QString &hdrTemplate)
{
	QString headerTemplate = hdrTemplate;
	qDebug() << "generating plugin header for " << mName;

	QDir dir;
	dir.cd(mTargetDirectory);
	dir.cd(mName);

	const QString fileName = dir.absoluteFilePath(pluginHeaderName);
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
	dir.cd(mTargetDirectory);
	dir.cd(mName);

	const QString fileName = dir.absoluteFilePath(pluginSourceName);
	QFile pluginSourceFile(fileName);
	if (!pluginSourceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	generateDiagramsMap();
	generateDiagramNodeNamesMap();
	generatePluginMethod(initElementNameMapLineTag, &Diagram::generateNamesMap);
	generatePluginMethod(initPropertyDisplayedNamesTag, &Diagram::generatePropertyDisplayedNamesMap);
	generatePluginMethod(elementDescriptionMapTag, &Diagram::generateElementDescriptionMap);
	generatePluginMethod(initMouseGesturesMapLineTag, &Diagram::generateMouseGesturesMap);
	generatePluginMethod(initPropertyTypesMapLineTag, &Diagram::generatePropertiesMap);
	generatePluginMethod(initPropertyDefaultMapLineTag, &Diagram::generatePropertyDefaultsMap);
	generatePluginMethod(getGraphicalObjectLineTag, &Diagram::generateFactory);
	generatePluginMethod(getContainersLineTag, &Diagram::generateContainers);
	generatePluginMethod(getPropertyNameTag, &Diagram::generatePropertyName);
	generatePluginMethod(getReferencePropertiesLineTag, &Diagram::generateReferenceProperties);
	generatePluginMethod(getPortTypesLineTag, &Diagram::generatePortTypes);
	generatePluginMethod(getConnectionsLineTag, &Diagram::generateConnections);
	generatePluginMethod(getUsagesLineTag, &Diagram::generateUsages);
	generatePluginMethod(getIsNodeOrEdgeLineTag, &Diagram::generateIsNodeOrEdge);
	generatePluginMethod(getPossibleEdgesLineTag, &Diagram::generatePossibleEdges);
	generatePluginMethod(initParentsMapLineTag, &Diagram::generateParentsMap);
	generateEnums();

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
	dir.cd(mTargetDirectory);
	dir.cd(mName);

	const QString fileName = dir.absoluteFilePath(elementsFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	QString generatedNodes;
	QString generatedEdges;

	for (const Diagram * const diagram : mDiagrams) {
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
	dir.cd(mTargetDirectory);
	dir.cd(mName);

	const QString fileName = dir.absoluteFilePath(resourceFileName);
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return false;
	}

	QString resourceBody = "";
	const QString line = mUtilsTemplate[sdfFileTag];
	for (const Diagram * const diagram : mDiagrams) {
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
	dir.cd(mTargetDirectory);
	dir.cd(mName);

	const QString nameOfMetamodel = mApi.stringProperty(mId, "name");
	const QString fileName = dir.absoluteFilePath(nameOfMetamodel + ".pro");
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
	generatePluginMethod(initDiagramNameMapLineTag
			, [](Diagram *diagram, const QString &line) {
				QString newline = line;
				return newline.replace(diagramDisplayedNameTag, diagram->displayedName())
						.replace(diagramNameTag, diagram->name()) + endline;
			}
	);
}

void Editor::generateDiagramNodeNamesMap()
{
	generatePluginMethod(initDiagramNodeNameMapLineTag
			, [](Diagram *diagram, const QString &line) {
				QString newline = line;
				return newline.replace(diagramNodeNameTag, diagram->nodeName())
						.replace(diagramNameTag, diagram->name()) + endline;
			}
	);
}

void Editor::generatePluginMethod(const QString &tag
		, const std::function<QString(Diagram *, const QString &)> &generator)
{
	QString body;
	const QString line = mUtilsTemplate[tag].replace("\\n", "\n");
	for (Diagram * const diagram : mDiagrams) {
		body += generator(diagram, line);
	}

	// inserting generated lines into main template
	mSourceTemplate.replace(tag, body);
}

void Editor::generateEnums()
{
	QString body = "";
	QString line = mUtilsTemplate[getEnumsLineTag].replace("\\n", "\n");

	for (const Diagram * const diagram : mDiagrams) {
		body += diagram->generateEnums(line);
	}

	// inserting generated lines into main template
	mSourceTemplate.replace(getEnumsLineTag, body);
}

bool Editor::loadIncludes()
{
	const QStringList includes = mApi.stringProperty(mId, "include").split(",");
	for (const QString &includedMetamodel : includes)
	{
		const QString metamodelName = includedMetamodel.section("/", -1).section(".", 0, 0).trimmed();
		if (metamodelName.isEmpty()) {
			continue;
		}

		Editor *includedEditor = nullptr;
		const IdList metamodels = mApi.elementsByType(metamodelDiagram);
		for (const Id &metamodel : metamodels) {
			if (!mApi.isLogicalElement(metamodel)) {
				continue;
			}

			if (mApi.name(metamodel) == metamodelName) {
				includedEditor = mMetaCompiler.loadMetaModel(metamodel);
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

	return true;
}

bool Editor::loadDiagrams()
{
	const IdList children = mApi.children(mId);
	IdList diagrams;
	for (const Id &child : children) {
		if (mApi.isLogicalElement(child) && child.element() == metaEditorDiagramNode) {
			diagrams << child;
		}
	}

	for (const Id &diagramId : diagrams) {
		qDebug() << "\tchildren:" << mApi.children(diagramId).size();
		const QString diagramName = mApi.name(diagramId);
		const Diagram *existingDiagram = mMetaCompiler.getDiagram(diagramName);
		if (existingDiagram) {
			qDebug() << "ERROR: diagram" << diagramName << "has been already loaded";
			return false;
		}

		qDebug() << "\tloading diagram" << diagramName;
		Diagram * const diagram = new Diagram(diagramId, mApi, *this, mTargetDirectory);
		if (!diagram->init()) {
			qDebug() << "ERROR: error loading diagram" << diagramName;
			delete diagram;
			return false;
		}

		qDebug() << "\tdiagram" << diagramName << "loaded";
		mDiagrams[diagramName] = diagram;
	}

	return true;
}

bool Editor::resolve()
{
	for (Diagram * const diagram : mDiagrams.values()) {
		if (!diagram->resolve()) {
			return false;
		}
	}

	return true;
}
