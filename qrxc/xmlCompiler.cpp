/* Copyright 2007-2016 QReal Research Group
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

#include "xmlCompiler.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/stringUtils.h>

#include "editor.h"
#include "nameNormalizer.h"
#include "diagram.h"
#include "type.h"
#include "edgeType.h"
#include "nodeType.h"
#include "portType.h"
#include "enumType.h"

using namespace utils;

XmlCompiler::XmlCompiler()
{
	mResources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";

	QDir dir;
	if (!dir.exists("generated")) {
		dir.mkdir("generated");
	}
	dir.cd("generated");
	if (!dir.exists("shapes")) {
		dir.mkdir("shapes");
	}
	dir.cd("..");
}

XmlCompiler::~XmlCompiler()
{
	qDeleteAll(mEditors);
}

bool XmlCompiler::compile(const QString &inputXmlFileName, const QString &sourcesRootFolder)
{
	const QFileInfo inputXmlFileInfo(inputXmlFileName);
	mPluginName = NameNormalizer::normalize(inputXmlFileInfo.completeBaseName());
	mCurrentEditor = inputXmlFileInfo.absoluteFilePath();
	mSourcesRootFolder = sourcesRootFolder;
	const QDir startingDir = inputXmlFileInfo.dir();
	if (!loadXmlFile(startingDir, inputXmlFileInfo.fileName())) {
		return false;
	}

	mPluginVersion = mEditors[mCurrentEditor]->version();

	generateCode();
	return true;
}

Editor* XmlCompiler::loadXmlFile(const QDir &currentDir, const QString &inputXmlFileName)
{
	QFileInfo fileInfo(inputXmlFileName);
	Q_ASSERT(fileInfo.fileName() == inputXmlFileName);

	QString fullFileName = currentDir.absolutePath() + "/" + inputXmlFileName;
	qDebug() << "Loading file started: " << fullFileName;

	if (mEditors.contains(fullFileName)) {
		Editor *editor = mEditors[fullFileName];
		if (editor->isLoaded()) {
			qDebug() << "File already loaded";
			return editor;
		} else {
			qDebug() << "ERROR: cycle detected";
			return nullptr;
		}
	} else {
		QString errorMessage;
		int errorLine = 0;
		int errorColumn = 0;
		QDomDocument inputXmlDomDocument = xmlUtils::loadDocument(fullFileName
			, &errorMessage, &errorLine, &errorColumn);
		if (!errorMessage.isEmpty()) {
			qCritical() << QString("(%1, %2):").arg(errorLine).arg(errorColumn)
					<< "Could not parse XML. Error:" << errorMessage;
		}

		Editor *editor = new Editor(inputXmlDomDocument, this);
		if (!editor->load(currentDir)) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return nullptr;
		}
		mEditors[fullFileName] = editor;
		return editor;
	}
}

Diagram * XmlCompiler::getDiagram(const QString &diagramName)
{
	foreach (Editor *editor, mEditors) {
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram) {
			return diagram;
		}
	}
	return nullptr;
}

void XmlCompiler::generateCode()
{
	if (!mEditors.contains(mCurrentEditor)) {
		qDebug() << "ERROR: Main editor xml was not loaded, generation aborted";
		return;
	}

	generateElementClasses();
	generatePluginHeader();
	generatePluginSource();
	generateResourceFile();
}

void XmlCompiler::addResource(const QString &resourceName)
{
	if (!mResources.contains(resourceName))
		mResources += resourceName;
}

void XmlCompiler::generateElementClasses()
{
	OutFile outElements("generated/elements.h");
	outElements() << "#pragma once\n\n"
		<< "#include <QBrush>\n"
		<< "#include <QPainter>\n\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/plugins/metaMetaModel/elementType.h\"\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/plugins/metaMetaModel/elementRepoInterface.h\"\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/plugins/metaMetaModel/labelFactoryInterface.h\"\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/plugins/metaMetaModel/labelInterface.h\"\n"
		<< "#include \"ports.h\"\n\n"
		;

	OutFile outPorts("generated/ports.h");
	outPorts() << "#pragma once\n\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/plugins/metaMetaModel/portHelpers.h\"\n\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			if (dynamic_cast<PortType *>(type)) {
				type->generateCode(outPorts);
			} else {
				type->generateCode(outElements);
			}
		}
	}
}

void XmlCompiler::generatePluginHeader()
{
	QString fileName = "generated/pluginInterface.h";// mPluginName

	OutFile out(fileName);

	out() << "#pragma once\n"
		<< "\n"
		<< "#include <QtCore/QStringList>\n"
		<< "#include <QtCore/QMap>\n"
		<< "#include <QtGui/QIcon>\n"
		<< "#include <QtCore/QPair>\n"
		<< "\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/plugins/metaMetaModel/metamodel.h\"\n"
		<< "\n"
		<< "class " << mPluginName << "Plugin : public QObject, public qReal::Metamodel\n"
		<< "{\n\tQ_OBJECT\n\tQ_INTERFACES(qReal::Metamodel)\n"
		<< "\tQ_PLUGIN_METADATA(IID \"" << mPluginName << "\")\n"
		<< "\n"
		<< "public:\n"
		<< "\n"
		<< "\t" << mPluginName << "Plugin();\n"
		<< "\n"
		<< "\tQString id() const { return \"" << mPluginName << "\"; }\n"
		<< "\tQString version() const { return \"" << mPluginVersion << "\"; }\n"
		<< "\n"
		<< "\tQStringList diagrams() const override;\n"
		<< "\tQStringList elements(const QString &diagram) const override;\n"
		<< "\tQStringList getPropertiesWithDefaultValues(const QString &element) const override;\n"
		<< "\n"
		<< "\tQStringList getTypesContainedBy(const QString &element) const override;\n"
		<< "\tQList<QPair<QPair<QString, QString>, QPair<bool, QString>>> getPossibleEdges(QString "
				"const &element) const override;\n"
		<< "\tQList<qReal::Metamodel::ExplosionData> explosions(const QString &diagram, QString "
				"const &element) const override;\n"
		<< "\n"
		<< "\tint isNodeOrEdge(const QString &element) const override;\n"
		<< "\n"
		<< "\tqReal::ElementType *getGraphicalObject(const QString &diagram, const QString &element) const override;\n"
		<< "\tQString getPropertyType(const QString &element, const QString &property) const override;\n"
		<< "\tQString getPropertyDefaultValue(const QString &element, const QString &property) const override;\n"
		<< "\tQStringList getPropertyNames(const QString &diagram, const QString &element) const override;\n"
		<< "\tQStringList getPortTypes(const QString &diagram, const QString &element) const override;\n"
		<< "\tQStringList getReferenceProperties(const QString &diagram, const QString &element) const override;\n"
		<< "\tQList<QPair<QString, QString>> getEnumValues(const QString &name) const override;\n"
		<< "\tbool isEnumEditable(const QString &name) const override;\n"
		<< "\tQString getGroupsXML() const override;\n"
		<< "\tQList<QPair<QString, QString>> getParentsOf(const QString &diagram, const QString &element) "
				"const override;\n"
		<< "\n"
		<< "\tQString editorName() const override;\n"
		<< "\tQString diagramName(const QString &diagram) const override;\n"
		<< "\tQString diagramNodeName(const QString &diagram) const override;\n"
		<< "\tQString elementName(const QString &diagram, const QString &element) const override;\n"
		<< "\tQString elementDescription(const QString &diagram, const QString &element) const override;\n"
		<< "\tQString propertyDescription(const QString &diagram, const QString &element, const QString "
				"&property) const override;\n"
		<< "\tQString propertyDisplayedName(const QString &diagram, const QString &element, const QString "
				"&property) const override;\n"
		<< "\tQString elementMouseGesture(const QString &digram, const QString &element) const override;\n"
		<< "\n"
		<< "\tbool isParentOf(const QString &parentDiagram, const QString &parentElement, const QString "
				"&childDiagram, const QString &childElement) const override;\n"
		<< "\n"
		<< "\tQStringList diagramPaletteGroups(const QString &diagram) const override;\n"
		<< "\tQStringList diagramPaletteGroupList(const QString &diagram, const QString &group) const override;\n"
		<< "\tQString diagramPaletteGroupDescription(const QString &diagram, const QString &group) const override;\n"
		<< "\tbool shallPaletteBeSorted(const QString &diagram) const override;\n"
		<< "\n"
		<< "private:\n"
		<< "\tvirtual void initPlugin();\n"
		<< "\tvirtual void initMouseGestureMap();\n"
		<< "\tvirtual void initNameMap();\n"
		<< "\tvirtual void initNodesAndEdgesSets();\n"
		<< "\tvirtual void initPropertyMap();\n"
		<< "\tvirtual void initPropertyDefaultsMap();\n"
		<< "\tvirtual void initDescriptionMap();\n"
		<< "\tvirtual void initPortTypes();\n"
		<< "\tvirtual void initParentsMap();\n"
		<< "\tvirtual void initPaletteGroupsMap();\n"
		<< "\tvirtual void initPaletteGroupsDescriptionMap();\n"
		<< "\tvirtual void initShallPaletteBeSortedMap();\n"
		<< "\tvirtual void initExplosionsMap();\n"
		<< "\n"
		<< "\tQMap<QString, QIcon> mIconMap;\n"
		<< "\tQMap<QString, QString> mDiagramNameMap;\n"
		<< "\tQMap<QString, QString> mDiagramNodeNameMap;\n"
		<< "\tQSet<QString> mNodes;  // A set of nodes in this editor. Useful for deciding is element node or edge.\n"
		<< "\tQSet<QString> mEdges;  // A set of edges in this editor. Useful for deciding is element node or edge.\n"
		<< "\tQMap<QString, QMap<QString, QString>> mPropertyTypes;\n"
		<< "\tQMap<QString, QStringList> mPortTypes;\n"
		<< "\tQMap<QString, QMap<QString, QString>> mPropertyDefault;\n"
		<< "\tQMap<QString, QMap<QString, QString>> mElementsNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString>> mElementsDescriptionMap;\n"
		<< "\tQMap<QString, QMap<QString, QMap<QString, QString>>> mPropertiesDescriptionMap;\n"
		<< "\tQMap<QString, QMap<QString, QMap<QString, QString>>> mPropertiesDisplayedNamesMap;\n"
		<< "\tQMap<QString, QMap<QString, QString>> mElementMouseGesturesMap;\n"
		<< "\tQMap<QString, QMap<QString, QList<QPair<QString, QString>>>> mParentsMap;  // Maps diagram and element"
				" to a list of diagram-element pairs of parents (generalization relation).\n"
		<< "\tQMap<QString, QList<QPair<QString, QStringList>>> mPaletteGroupsMap;  // Maps element`s lists of all "
				"palette groups.\n"
		<< "\tQMap<QString, QMap<QString, QString>> mPaletteGroupsDescriptionMap;\n"
		<< "\tQMap<QString, bool> mShallPaletteBeSortedMap;\n"
		<< "\tQMap<QString, QMap<QString, QList<qReal::Metamodel::ExplosionData>>> mExplosionsMap;\n"
		<< "};\n"
		<< "\n";
}

void XmlCompiler::generatePluginSource()
{
	QString fileName = "generated/pluginInterface.cpp"; //mPluginName

	OutFile out(fileName);

	generateIncludes(out);
	generateInitPlugin(out);
	generateNameMappingsRequests(out);
	generateGraphicalObjectRequest(out);
	generateIsParentOfRequest(out);
	generateGetParentsOfRequest(out);
	generateProperties(out);
	generatePortTypes(out);
	generateReferenceProperties(out);
	generateContainedTypes(out);
	generatePossibleEdges(out);
	generateNodesAndEdges(out);
	generateGroupsXML(out);
	generateEnumValues(out);
	generateEditableEnums(out);
	generatePropertyTypesRequests(out);
	generatePropertyDefaultsRequests(out);
}

void XmlCompiler::generateIncludes(OutFile &out)
{
	out() << "#include \"" << "pluginInterface.h\"\n" //mPluginName
		<< "\n";

	out() << "#include \"" << "elements.h" << "\"\n";

	out() << "\n";

	out()
		//<< "Q_EXPORT_PLUGIN2(qreal_editors, " << mPluginName << "Plugin)\n\n"
		<< mPluginName << "Plugin::" << mPluginName << "Plugin()\n{\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n";
}

void XmlCompiler::generateInitPlugin(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPlugin()\n{\n"
		<< "\tinitNameMap();\n"
		<< "\tinitNodesAndEdgesSets();\n"
		<< "\tinitMouseGestureMap();\n"
		<< "\tinitPropertyMap();\n"
		<< "\tinitPropertyDefaultsMap();\n"
		<< "\tinitDescriptionMap();\n"
		<< "\tinitPortTypes();\n"
		<< "\tinitParentsMap();\n"
		<< "\tinitPaletteGroupsMap();\n"
		<< "\tinitPaletteGroupsDescriptionMap();\n"
		<< "\tinitShallPaletteBeSortedMap();\n"
		<< "\tinitExplosionsMap();\n"
		<< "}\n\n";

	generateNameMappings(out);
	generateNodesAndEdgesSets(out);
	generatePaletteGroupsLists(out);
	generatePaletteGroupsDescriptions(out);
	generateMouseGestureMap(out);
	generatePropertyMap(out);
	generatePropertyDefaultsMap(out);
	generateDescriptionMappings(out);
	generatePortTypeMappings(out);
	generateParentsMappings(out);
	generateShallPaletteBeSorted(out);
	generateExplosionsMappings(out);
}

void XmlCompiler::generateNameMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initNameMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tmDiagramNameMap[\"" << diagramName << "\"] = tr(\""
				<< diagram->displayedName() << "\");\n";
		out() << "\tmDiagramNodeNameMap[\"" << diagramName << "\"] = \"" << diagram->nodeName() << "\"" << ";\n";
		out() << "\n";
	}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			type->generateNameMapping(out);
		}
	}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj) {
				obj->generatePropertyDisplayedNamesMapping(out);
			}
		}
	}

	out() << "}\n\n";
}

void XmlCompiler::generateNodesAndEdgesSets(OutFile &out)
{
	QSet<QString> nodes;
	QSet<QString> edges;
	for (Diagram *diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		for (Type *type : diagram->types().values()) {
			if (dynamic_cast<NodeType*>(type)) {
				nodes.insert(NameNormalizer::normalize(type->qualifiedName()));
			} else if (dynamic_cast<EdgeType*>(type)) {
				edges.insert(NameNormalizer::normalize(type->qualifiedName()));
			}
		}
	}

	out() << "void " << mPluginName << "Plugin::initNodesAndEdgesSets()\n{\n";
	out() << "\tmNodes = ";
	generateStringSet(out, nodes);
	out() << "\tmEdges = ";
	generateStringSet(out, edges);
	out() << "}\n\n";
}

void XmlCompiler::generatePaletteGroupsLists(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPaletteGroupsMap()\n{\n";

	for (Diagram *diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		QList<QPair<QString, QStringList>> paletteGroups = diagram->paletteGroups();
		for (QPair<QString, QStringList> const &group: paletteGroups) {
			const QString groupName = group.first;

			out() << "\t{\n";
			out() << "\t\tQStringList groupElements;\n";

			for (const QString &name : group.second) {
				out() << "\t\tgroupElements << QString::fromUtf8(\"" << NameNormalizer::normalize(name) << "\");\n";
			}

			out() << "\t\tmPaletteGroupsMap[QString::fromUtf8(\""
				<< diagramName << "\")].append(qMakePair(tr(\""
				<< groupName << "\"), groupElements));\n";

			out() << "\t}\n";
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generatePaletteGroupsDescriptions(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPaletteGroupsDescriptionMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		QMap<QString, QString> paletteGroupsDescriptions = diagram->paletteGroupsDescriptions();
		foreach (const QString &groupName, paletteGroupsDescriptions.keys()) {
			const QString descriptionName = paletteGroupsDescriptions[groupName];
			if (!descriptionName.isEmpty()) {
				out() << "\tmPaletteGroupsDescriptionMap[QString::fromUtf8(\""
					<< diagramName << "\")][tr(\""
					<< groupName << "\")] = tr(\""
					<< descriptionName << "\");\n";
			}
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generateShallPaletteBeSorted(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initShallPaletteBeSortedMap()\n{\n";

	for (const Diagram * const diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		const QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tmShallPaletteBeSortedMap[QString::fromUtf8(\""
			<< diagramName << "\")] = " << (diagram->shallPaletteBeSorted() ? "true" : "false") << ";\n";
	}
	out() << "}\n\n";
}

void XmlCompiler::generateDescriptionMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initDescriptionMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj) {
				obj->generateDescriptionMapping(out);
			}
		}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj) {
				obj->generatePropertyDescriptionMapping(out);
			}
		}

	out() << "}\n\n";
}

void XmlCompiler::generatePortTypeMappings(OutFile &out)
{
	QMap<QString, QStringList> result;
	for (const Diagram *diagram : mEditors[mCurrentEditor]->diagrams().values()) {
		for (const Type *type : diagram->types().values()) {
			const QString elementName = NameNormalizer::normalize(type->qualifiedName());
			if (const GraphicType *graphicsType = dynamic_cast<const GraphicType *>(type)) {
				QSet<QString> portTypes;
				for (const Port *port : graphicsType->ports()) {
					portTypes.insert(port->type());
				}

				result[elementName] = portTypes.toList();
			}
		}
	}

	out() << "void " << mPluginName << "Plugin::initPortTypes()\n{\n";
	out() << "\tmPortTypes = ";
	generateStringListMap(out, result);
	out() << ";\n}\n\n";
}

void XmlCompiler::generateParentsMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initParentsMap()\n"
		<< "{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj) {
				obj->generateParentsMapping(out);
			}
		}

	out() << "}\n\n";
}

void XmlCompiler::generateMouseGestureMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initMouseGestureMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			type->generateMouseGesturesMap(out);
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPropertyMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			type->generatePropertyTypes(out);
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyDefaultsMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPropertyDefaultsMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			type->generatePropertyDefaults(out);
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generateExplosionsMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initExplosionsMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			type->generateExplosionsMap(out);
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyTypesRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyType(const QString &element, "
			<< "const QString &property) const\n{\n"
			<< "\treturn mPropertyTypes[element][property];\n" // TODO: merge with getPropertyNames()!!11
			<< "}\n\n";
}

void XmlCompiler::generatePropertyDefaultsRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyDefaultValue(const QString &element, "
			<< "const QString &property) const\n{\n"
			<< "\treturn mPropertyDefault[element][property];\n" // TODO: merge with getPropertyNames()!!11
			<< "}\n\n";
}

void XmlCompiler::generateNameMappingsRequests(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::diagrams() const\n{\n"
		<< "\treturn mDiagramNameMap.keys();\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::diagramPaletteGroups(const QString &diagram) const\n{\n"

		<< "\tQStringList result;\n"
		<< "\tfor (QPair<QString, QStringList> const &group : mPaletteGroupsMap[diagram]) {\n"
		<< "\t\tresult << group.first;\n"
		<< "\t}\n"
		<< "\n"
		<< "\treturn result;\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::diagramPaletteGroupList(const QString &diagram, "
		<< "const QString &group) const\n{\n"
		<< "\tfor (QPair<QString, QStringList> const &ourGroup : mPaletteGroupsMap[diagram]) {\n"
		<< "\t\tif (ourGroup.first == group) {\n"
		<< "\t\t\treturn ourGroup.second;\n"
		<< "\t\t}\n"
		<< "\t}\n"
		<< "\n"
		<< "\treturn QStringList();\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramPaletteGroupDescription(const QString &diagram, "
		<< "const QString &group) const\n{\n"
		<< "\treturn mPaletteGroupsDescriptionMap[diagram][group];\n"
		<< "}\n\n"

		<< "bool " << mPluginName << "Plugin::shallPaletteBeSorted(const QString &diagram) const\n{\n"
		<< "\treturn mShallPaletteBeSortedMap[diagram];\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::elements(const QString &diagram) const\n{\n"
		<< "\treturn mElementsNameMap[diagram].keys();\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::getPropertiesWithDefaultValues(const QString &element) const\n{\n"
		<< "\treturn mPropertyDefault[element].keys();\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::editorName() const\n{\n"
		<< "\t return \"" << mPluginName << " Editor\";\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramName(const QString &diagram) const\n{\n"
		<< "\treturn mDiagramNameMap[diagram];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramNodeName(const QString &diagram) const\n{\n"
		<< "\treturn mDiagramNodeNameMap[diagram];\n"
		<< "}\n\n"

		<< "QString " << mPluginName
				<< "Plugin::elementName(const QString &diagram, const QString &element) const\n{\n"
		<< "\treturn mElementsNameMap[diagram][element];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementDescription(const QString &diagram, const QString &element)"
		<< "const\n{\n"
		<< "\treturn mElementsDescriptionMap[diagram][element];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::propertyDescription(const QString &diagram, const QString &element, "
		<< "const QString &property) const\n{\n"
		<< "\treturn mPropertiesDescriptionMap[diagram][element][property];\n"
		<< "}\n\n"

		<< "QString " << mPluginName
				<< "Plugin::propertyDisplayedName(const QString &diagram, const QString &element, "
		<< "const QString &property) const\n{\n"
		<< "\treturn mPropertiesDisplayedNamesMap[diagram][element][property];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementMouseGesture(const QString &diagram, const QString &element) "
		<< "const\n{\n"
		<< "\treturn mElementMouseGesturesMap[diagram][element];\n"
		<< "}\n\n"

		<< "QList<qReal::Metamodel::ExplosionData>" << mPluginName
				<< "Plugin::explosions(const QString &diagram, "
		<< "const QString &element) const \n{\n"
		<< "\treturn mExplosionsMap[diagram][element];\n"
		<< "}\n\n";
}

void XmlCompiler::generateGraphicalObjectRequest(OutFile &out)
{
	out() << "qReal::ElementType *" << mPluginName
		<< "Plugin::getGraphicalObject(const QString &/*diagram*/, const QString &element) const\n{\n";

	bool isNotFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			isNotFirst |= type->generateObjectRequestString(out, isNotFirst);

	if (isNotFirst) {
		out() << "	else {\n"
			<< "		Q_ASSERT(!\"Request for creation of an element with unknown name\");\n"
			<< "		return nullptr;\n"
			<< "	}\n";
	} else {
		out() << "	Q_ASSERT(!\"Request for creation of an element with unknown name\");\n"
			<< "	return nullptr;\n";
	}
	out() << "}\n\n";
}

void XmlCompiler::generateIsParentOfRequest(OutFile &out)
{
	out() << "bool " << mPluginName << "Plugin::isParentOf(const QString &parentDiagram"
			 << ", const QString &parentElement, const QString &childDiagram, const QString &childElement) const\n"
		<< "{\n"
		<< "\tif (childDiagram == parentDiagram && childElement == parentElement)\n"
		<< "\t\treturn true;\n"
		<< "\tif (mParentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))\n"
		<< "\t\treturn true;\n"
		<< "\ttypedef QPair<QString, QString> StringPair;\n"
		<< "\tfor (const StringPair &pair : mParentsMap[childDiagram][childElement])\n"
		<< "\t\tif (isParentOf(parentDiagram, parentElement, pair.first, pair.second))\n"
		<< "\t\t\treturn true;\n"
		<< "\treturn false;\n"
		<< "}\n"
	;
}

void XmlCompiler::generateGetParentsOfRequest(OutFile &out)
{
	out() << "QList<QPair<QString, QString> > " << mPluginName << "Plugin::getParentsOf(const QString &diagram"
		<< ", const QString &element) const\n"
		<< "{\n"
		<< "\treturn mParentsMap[diagram][element];\n"
		<< "}\n"
	;
}

// Набор классов ниже представляет собой эмуляцию средствами C++ лямбда-выражений.
// Цель: разделить действие, выполняемое при хитром обходе хитрой структуры данных
// и саму логику хитрого обхода. В нормальных языках хитрый обход можно сделать методом,
// принимающим функцию, вызываемую при посещении каждого элемента. В C++ так тоже
// можно, но невежливо, поэтому делается так: тоже есть метод, реализующий
// хитрый обход, ему вместо функции передаётся объект-действие, реализующий некий
// интерфейс. Интерфейс содержит только один метод, который принимает в качестве параметров
// элемент структуры, который мы хотим посетить, некоторые дополнительные параметры,
// говорящии о состоянии обхода, и некоторые параметры из внешнего контекста
// (для которых в нормальных языках вообще есть замыкания).
// Здесь: обход (не очень хитрый) - это generateListMethod, интерфейс -
// ListMethodGenerator, объекты-действия - PropertiesGenerator и т.д.
// Примечание: на С++ это выглядит уродски, на C# вообще лишнего кода бы не было.
// Даже в Java с анонимными классами это бы выглядело лучше.
class XmlCompiler::ListMethodGenerator
{
public:
	virtual ~ListMethodGenerator() {}
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const = 0;
};

class XmlCompiler::PropertiesGenerator: public XmlCompiler::ListMethodGenerator
{
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateProperties(out, isNotFirst, false);
	}
};

class XmlCompiler::PortsGenerator: public XmlCompiler::ListMethodGenerator
{
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generatePorts(out, isNotFirst);
	}
};

class XmlCompiler::ReferencePropertiesGenerator: public XmlCompiler::ListMethodGenerator
{
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateProperties(out, isNotFirst, true);
	}
};

class XmlCompiler::ContainedTypesGenerator: public XmlCompiler::ListMethodGenerator
{
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateContainedTypes(out, isNotFirst);
	}
};

class XmlCompiler::PossibleEdgesGenerator: public XmlCompiler::ListMethodGenerator
{
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generatePossibleEdges(out, isNotFirst);
	}
};

class XmlCompiler::EnumValuesGenerator: public XmlCompiler::ListMethodGenerator
{
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateEnumValues(out, isNotFirst);
	}
};

void XmlCompiler::generateListMethod(OutFile &out, const QString &signature, const ListMethodGenerator &generator)
{
	out() << "QStringList " << mPluginName << "Plugin::" << signature << " const\n"
		<< "{\n"
		<< "\tQStringList result;\n";

	bool isNotFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			isNotFirst |= generator.generate(type, out, isNotFirst);

	if (!isNotFirst)
		out() << "\tQ_UNUSED(element);\n";
	out() << "\treturn result;\n"
		  << "}\n\n";
}

void XmlCompiler::generateStringSet(OutFile &out, const QSet<QString> &set) const
{
	out() << "QSet<QString>({\n";
	for (const QString &element : set) {
		out() << "\t\t\"" << element << "\",\n";
	}

	out() << "\t});\n";
}

void XmlCompiler::generateStringList(OutFile &out, const QStringList &list) const
{
	out() << "{";
	for (const QString &string : list) {
		out() << "\"" << string << "\", ";
	}

	out() << "}";
}

void XmlCompiler::generateStringListMap(OutFile &out, const QMap<QString, QStringList> &map) const
{
	out() << "QMap<QString, QStringList>({\n";
	for (const QString &key : map.keys()) {
		out() << "\t\t{" << StringUtils::wrap(key) << ", ";
		generateStringList(out, map[key]);
		out() << "},\n";
	}

	out() << "\t})";
}

void XmlCompiler::generatePossibleEdges(utils::OutFile &out)
{
	PossibleEdgesGenerator generator;
		out() << "QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > " << mPluginName
			<< "Plugin::getPossibleEdges(const QString &element) const\n"
			<< "{\n"
			<< "\tQList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;\n";
	bool isNotFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			isNotFirst |= generator.generate(type, out, isNotFirst);

	if (!isNotFirst)
		out() << "\tQ_UNUSED(element);\n";
		out() << "\treturn result;\n"
		<< "}\n\n";
}

void XmlCompiler::generateNodesAndEdges(utils::OutFile &out)
{
	out() << "//(-1) means \"edge\", (+1) means \"node\"\n";
	out() << "int " << mPluginName << "Plugin::isNodeOrEdge(const QString &element) const\n{\n";
	out() << "\treturn mNodes.contains(element) ? 1 : (mEdges.contains(element) ? -1 : 0);\n}\n";
}

void XmlCompiler::generateProperties(OutFile &out)
{
	generateListMethod(out, "getPropertyNames(const QString &/*diagram*/, const QString &element)"
			, PropertiesGenerator());
}

void XmlCompiler::generatePortTypes(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::getPortTypes(const QString &/*diagram*/, const QString &element)"
		<< " const\n{\n";
	out() << "\treturn mPortTypes[element];\n";
	out() << "}\n\n";
}

void XmlCompiler::generateReferenceProperties(OutFile &out)
{
	generateListMethod(out, "getReferenceProperties(const QString &/*diagram*/, const QString &element)"
			, ReferencePropertiesGenerator());
}

void XmlCompiler::generateContainedTypes(OutFile &out)
{
	generateListMethod(out, "getTypesContainedBy(const QString &element)", ContainedTypesGenerator());
}

void XmlCompiler::generateResourceFile()
{
	OutFile out("plugin.qrc");
	out() << mResources
		<< "</qresource>\n"
		<< "</RCC>\n";
}

void XmlCompiler::generateGroupsXML(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getGroupsXML() const \n{\n";
	QString result = "";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams()){
		if (!diagram->getGroupsXML().isEmpty())
			result = result + diagram->getGroupsXML();
	}
	out() << "\treturn QString::fromUtf8(\"" << result << "\");\n"
		<< "}\n\n";
}

void XmlCompiler::generateEnumValues(OutFile &out)
{
	out() << "QList<QPair<QString, QString>> " << mPluginName
			<< "Plugin::getEnumValues(const QString &name) const \n{\n";

	EnumValuesGenerator generator;
	bool isNotFirst = false;

	foreach (EnumType *type, mEditors[mCurrentEditor]->getAllEnumTypes())
		isNotFirst |= generator.generate(type, out, isNotFirst);

	if (!isNotFirst) {
		out() << "\tQ_UNUSED(name);\n";
	}

	out() << "\treturn {};\n"
		<< "}\n\n";
}

void XmlCompiler::generateEditableEnums(OutFile &out)
{
	out() << "bool " << mPluginName << "Plugin::isEnumEditable(const QString &name) const\n{\n";

	QStringList editableEnums;
	for (const EnumType *type : mEditors[mCurrentEditor]->getAllEnumTypes()) {
		if (type->isEditable()) {
			editableEnums << StringUtils::wrap(type->name());
		}
	}

	out() << QString("\tconst QStringList editableEnums = { %1 };\n").arg(editableEnums.join(", "));

	out() << "\treturn editableEnums.contains(name);\n"
		<< "}\n\n";
}
