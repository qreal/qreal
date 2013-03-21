#include "xmlCompiler.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "../qrutils/outFile.h"
#include "../qrutils/xmlUtils.h"
#include "diagram.h"
#include "type.h"

#include "edgeType.h"
#include "nodeType.h"
#include "enumType.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

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
	foreach(Editor *editor, mEditors.values()) {
		if (editor) {
			delete editor;
		}
	}
}

bool XmlCompiler::compile(QString const &inputXmlFileName, QString const &sourcesRootFolder)
{
	QFileInfo const inputXmlFileInfo(inputXmlFileName);
	mPluginName = NameNormalizer::normalize(inputXmlFileInfo.baseName());
	mCurrentEditor = inputXmlFileInfo.absoluteFilePath();
	mSourcesRootFolder = sourcesRootFolder;
	QDir const startingDir = inputXmlFileInfo.dir();
	if (!loadXmlFile(startingDir, inputXmlFileInfo.fileName())) {
		return false;
	}
	generateCode();
	return true;
}

Editor* XmlCompiler::loadXmlFile(QDir const &currentDir, QString const &inputXmlFileName)
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
			return NULL;
		}
	} else {
		QDomDocument inputXmlDomDocument = xmlUtils::loadDocument(fullFileName);
		Editor *editor = new Editor(inputXmlDomDocument, this);
		if (!editor->load(currentDir)) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return NULL;
		}
		mEditors[fullFileName] = editor;
		return editor;
	}
}

Diagram * XmlCompiler::getDiagram(QString const &diagramName)
{
	foreach (Editor *editor, mEditors) {
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram) {
			return diagram;
		}
	}
	return NULL;
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

void XmlCompiler::addResource(QString const &resourceName)
{
	if (!mResources.contains(resourceName))
		mResources += resourceName;
}

void XmlCompiler::generateElementClasses()
{
	OutFile out("generated/elements.h");
	out() << "#pragma once\n\n"
		<< "#include <QBrush>\n"
		<< "#include <QPainter>\n\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/editorPluginInterface/elementImpl.h\"\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/editorPluginInterface/elementRepoInterface.h\"\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/editorPluginInterface/elementTitleHelpers.h\"\n\n"
		;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		foreach (Type *type, diagram->types().values()) {
			type->generateCode(out);
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
		<< "#include <QPair>"
		<< "\n"
		<< "#include \"../" << mSourcesRootFolder << "/qrgui/editorPluginInterface/editorInterface.h\"\n"
		<< "\n"
		<< "class " << mPluginName << "Plugin : public QObject, public qReal::EditorInterface\n"
		<< "{\n\tQ_OBJECT\n\tQ_INTERFACES(qReal::EditorInterface)\n"
		<< "\n"
		<< "public:\n"
		<< "\n"
		<< "\t" << mPluginName << "Plugin();\n"
		<< "\n"
		<< "\tvirtual void initPlugin();\n"
		<< "\tvirtual void initMouseGestureMap();\n"
		<< "\tvirtual void initNameMap();\n"
		<< "\tvirtual void initPropertyMap();\n"
		<< "\tvirtual void initPropertyDefaultsMap();\n"
		<< "\tvirtual void initDescriptionMap();\n"
		<< "\tvirtual void initParentsMap();\n"
		<< "\tvirtual void initPaletteGroupsMap();\n"
		<< "\tvirtual void initPaletteGroupsDescriptionMap();\n"
		<< "\n"
		<< "\tvirtual QString id() const { return \"" << mPluginName << "\"; }\n"
		<< "\n"
		<< "\tvirtual QStringList diagrams() const;\n"
		<< "\tvirtual QStringList elements(QString const &diagram) const;\n"
		<< "\tvirtual QStringList getPropertiesWithDefaultValues(QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual QStringList getTypesContainedBy(QString const &element) const;\n"
		<< "\tvirtual QStringList getConnectedTypes(QString const &element) const;\n"
		<< "\tvirtual QStringList getUsedTypes(QString const &element) const;\n"
		<< "\tvirtual QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > getPossibleEdges(QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual int isNodeOrEdge(QString const &element) const; \n"
		<< "\n"
		<< "\tvirtual QIcon getIcon(SdfIconEngineV2Interface *engine) const;\n"
		<< "\tvirtual ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString getPropertyType(QString const &element, QString const &property) const;\n"
		<< "\tvirtual QString getPropertyDefaultValue(QString const &element, QString const &property) const;\n"
		<< "\tvirtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QStringList getReferenceProperties(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QStringList getEnumValues(QString name) const;\n"
		<< "\tvirtual QString getGroupsXML() const;\n"
		<< "\tvirtual QList<QPair<QString, QString> > getParentsOf(QString const &diagram, QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual QString editorName() const;\n"
		<< "\tvirtual QString diagramName(QString const &diagram) const;\n"
		<< "\tvirtual QString diagramNodeName(QString const &diagram) const;\n"
		<< "\tvirtual QString elementName(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString elementDescription(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString propertyDescription(QString const &diagram, QString const &element, QString const &property) const;\n"
		<< "\tvirtual QString propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const;\n"
		<< "\tvirtual QString elementMouseGesture(QString const &digram, QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual QList<qReal::ListenerInterface*> listeners() const;\n"
		<< "\n"
		<< "\tvirtual bool isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const;\n"
		<< "\n"
		<< "\tvirtual QStringList diagramPaletteGroups(QString const &diagram) const;\n"
		<< "\tvirtual QStringList diagramPaletteGroupList(QString const &diagram, QString const &group) const;\n"
		<< "\tvirtual QString diagramPaletteGroupDescription(QString const &diagram, QString const &group) const;\n"
		<< "\n"
		<< "private:\n"
		<< "\tQMap<QString, QIcon> iconMap;\n"
		<< "\tQMap<QString, QString> diagramNameMap;\n"
		<< "\tQMap<QString, QString> diagramNodeNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > propertyTypes;\n"
		<< "\tQMap<QString, QMap<QString, QString> > propertyDefault;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementsNameMap;\n"

		<< "\tQMap<QString, QMap<QString, QString> > elementsDescriptionMap;\n"
		<< "\tQMap<QString, QMap<QString, QMap<QString, QString> > > propertiesDescriptionMap;\n"
		<< "\tQMap<QString, QMap<QString, QMap<QString, QString> > > propertiesDisplayedNamesMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementMouseGesturesMap;\n"
		<< "\tQMap<QString, QMap<QString, QList<QPair<QString, QString> > > > parentsMap;  // Maps diagram and element to a list of diagram-element pairs of parents (generalization relation).\n"
		<< "\tQMap<QString, QMap<QString, QStringList > > paletteGroupsMap;  // Maps element`s lists of all palette groups.\n"
		<< "\tQMap<QString, QMap<QString, QString > > paletteGroupsDescriptionMap; \n"
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
	generateReferenceProperties(out);
	generateContainedTypes(out);
	generateConnections(out);
	generateUsages(out);
	generatePossibleEdges(out);
	generateNodesAndEdges(out);
	generateGroupsXML(out);
	generateEnumValues(out);
	generatePropertyTypesRequests(out);
	generatePropertyDefaultsRequests(out);

	mEditors[mCurrentEditor]->generateListenerFactory(out, mPluginName);
}

void XmlCompiler::generateIncludes(OutFile &out)
{
	out() << "#include \"" << "pluginInterface.h\"\n" //mPluginName
		<< "\n";

	out() << "#include \"" << "elements.h" << "\"\n";

	out() << "\n";

	mEditors[mCurrentEditor]->generateListenerIncludes(out);

	out() << "Q_EXPORT_PLUGIN2(qreal_editors, " << mPluginName << "Plugin)\n\n"
		<< mPluginName << "Plugin::" << mPluginName << "Plugin()\n{\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n";
}

void XmlCompiler::generateInitPlugin(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPlugin()\n{\n"
		<< "\tinitNameMap();\n"
		<< "\tinitMouseGestureMap();\n"
		<< "\tinitPropertyMap();\n"
		<< "\tinitPropertyDefaultsMap();\n"
		<< "\tinitDescriptionMap();\n"
		<< "\tinitParentsMap();\n"
		<< "\tinitPaletteGroupsMap();\n"
		<< "\tinitPaletteGroupsDescriptionMap();\n"
		<< "}\n\n";

	generateNameMappings(out);
	generatePaletteGroupsLists(out);
	generatePaletteGroupsDescriptions(out);
	generateMouseGestureMap(out);
	generatePropertyMap(out);
	generatePropertyDefaultsMap(out);
	generateDescriptionMappings(out);
	generateParentsMappings(out);
}

void XmlCompiler::generateNameMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initNameMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tdiagramNameMap[\"" << diagramName << "\"] = QString::fromUtf8(\"" << diagram->displayedName() << "\");\n";
		out() << "\tdiagramNodeNameMap[\"" << diagramName << "\"] = \"" << diagram->nodeName() << "\"" << ";\n";
		out() << "\n";
	}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			type->generateNameMapping(out);

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj)
				obj->generatePropertyDisplayedNamesMapping(out);
		}

	out() << "}\n\n";
}

void XmlCompiler::generatePaletteGroupsLists(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPaletteGroupsMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		QMap<QString, QStringList > paletteGroups = diagram->paletteGroups();
		foreach (QList<QString> list , paletteGroups) {
			QString groupName = paletteGroups.key(list);
			foreach (QString name, list) {
				out() << "\tpaletteGroupsMap[QString::fromUtf8(\""
					<< diagramName << "\")][QString::fromUtf8(\""
					<< groupName << "\")].append(QString::fromUtf8(\""
					<< NameNormalizer::normalize(name) << "\"));\n";
			}
		}
	}
	out() << "}\n\n";
}

void XmlCompiler::generatePaletteGroupsDescriptions(utils::OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPaletteGroupsDescriptionMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		QMap<QString, QString > paletteGroupsDescriptions = diagram->paletteGroupsDescriptions();
		foreach (QString groupName, paletteGroupsDescriptions.keys()) {
			QString descriptionName = paletteGroupsDescriptions[groupName];
			if (!descriptionName.isEmpty()) {
				out() << "\tpaletteGroupsDescriptionMap[QString::fromUtf8(\""
					<< diagramName << "\")][QString::fromUtf8(\""
					<< groupName << "\")] = QString::fromUtf8(\""
					<< descriptionName << "\");\n";
			}
		}
	}
	out() << "}\n\n";
}


void XmlCompiler::generateDescriptionMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initDescriptionMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj)
				obj->generateDescriptionMapping(out);
		}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj)
				obj->generatePropertyDescriptionMapping(out);
		}

	out() << "}\n\n";
}

void XmlCompiler::generateParentsMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initParentsMap()\n"
		<< "{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()) {
			GraphicType *obj = dynamic_cast<GraphicType *>(type);
			if (obj)
				obj->generateParentsMapping(out);
		}

	out() << "}\n\n";
}

void XmlCompiler::generateMouseGestureMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initMouseGestureMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			type->generateMouseGesturesMap(out);
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPropertyMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			type->generatePropertyTypes(out);
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyDefaultsMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPropertyDefaultsMap()\n{\n";
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			type->generatePropertyDefaults(out);
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyTypesRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyType(QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertyTypes[element][property];\n" // TODO: merge with getPropertyNames()!!11
		<< "}\n\n";
}

void XmlCompiler::generatePropertyDefaultsRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyDefaultValue(QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertyDefault[element][property];\n" // TODO: merge with getPropertyNames()!!11
		<< "}\n\n";
}

void XmlCompiler::generateNameMappingsRequests(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::diagrams() const\n{\n"
		<< "\treturn diagramNameMap.keys();\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::diagramPaletteGroups(QString const &diagram) const\n{\n"
		<< "\treturn paletteGroupsMap[diagram].keys();\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const\n{\n"
		<< "\treturn paletteGroupsMap[diagram][group];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramPaletteGroupDescription(QString const &diagram, QString const &group) const\n{\n"
		<< "\treturn paletteGroupsDescriptionMap[diagram][group];\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::elements(QString const &diagram) const\n{\n"
		<< "\treturn elementsNameMap[diagram].keys();\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::getPropertiesWithDefaultValues(QString const &element) const\n{\n"
		<< "\treturn propertyDefault[element].keys();\n"
		<< "}\n\n"

		<< "QIcon " << mPluginName << "Plugin::getIcon(SdfIconEngineV2Interface *engine) const\n{\n"
		<< "\treturn QIcon(engine);\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::editorName() const\n{\n"
		<< "\t return \"" << mPluginName << " Editor\";\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramName(QString const &diagram) const\n{\n"
		<< "\treturn diagramNameMap[diagram];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramNodeName(QString const &diagram) const\n{\n"
		<< "\treturn diagramNodeNameMap[diagram];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementName(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementsNameMap[diagram][element];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementDescription(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementsDescriptionMap[diagram][element];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertiesDescriptionMap[diagram][element][property];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertiesDisplayedNamesMap[diagram][element][property];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementMouseGesture(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementMouseGesturesMap[diagram][element];\n"
		<< "}\n\n";
}

void XmlCompiler::generateGraphicalObjectRequest(OutFile &out)
{
	out() << "ElementImpl* " << mPluginName
		<< "Plugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const\n{\n";

	bool isNotFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			isNotFirst |= type->generateObjectRequestString(out, isNotFirst);

	if (isNotFirst) {
		out() << "	else {\n"
			<< "		Q_ASSERT(!\"Request for creation of an element with unknown name\");\n"
			<< "		return NULL;\n"
			<< "	}\n";
	} else {
		out() << "	Q_ASSERT(!\"Request for creation of an element with unknown name\");\n"
			<< "	return NULL;\n";
	}
	out() << "}\n\n";
}

void XmlCompiler::generateIsParentOfRequest(OutFile &out)
{
	out() << "bool " << mPluginName << "Plugin::isParentOf(QString const &parentDiagram"
			 << ", QString const &parentElement, QString const &childDiagram, QString const &childElement) const\n"
		<< "{\n"
		<< "\tif (childDiagram == parentDiagram && childElement == parentElement)\n"
		<< "\t\treturn true;\n"
		<< "\tif (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))\n"
		<< "\t\treturn true;\n"
		<< "\ttypedef QPair<QString, QString> StringPair;\n"
		<< "\tforeach (StringPair const &pair, parentsMap[childDiagram][childElement])\n"
		<< "\t\tif (isParentOf(parentDiagram, parentElement, pair.first, pair.second))\n"
		<< "\t\t\treturn true;\n"
		<< "\treturn false;\n"
		<< "}\n"
	;
}

void XmlCompiler::generateGetParentsOfRequest(OutFile &out)
{
	out() << "QList<QPair<QString, QString> > " << mPluginName << "Plugin::getParentsOf(QString const &diagram" << ", QString const &element) const\n"
		<< "{\n"
		<< "\treturn parentsMap[diagram][element];\n"
		<< "}\n"
	;
}

// Набор классов ниже представляет собой эмуляцию средствами C++ лямбда-выражений.
// Цель: разделить действие, выполняемое при хитром обходе хитрой структуры данных
// и саму логику хитрого обхода. В нормальных языках хитрый обход можно сделать методом,
// принимающим функцию, вызываемую при посещении каждого элемента. В C++ так тоже
// можно, но невежливо, поэтому делается так: тоже есть метод, реализующий
// хитрый обход, ему вместо функции передаётся объект-действие, реализующий некий
// интерфейс. �?нтерфейс содержит только один метод, который принимает в качестве параметров
// элемент структуры, который мы хотим посетить, некоторые дополнительные параметры,
// говорящии о состоянии обхода, и некоторые параметры из внешнего контекста
// (для которых в нормальных языках вообще есть замыкания).
// Здесь: обход (не очень хитрый) - это generateListMethod, интерфейс -
// ListMethodGenerator, объекты-действия - PropertiesGenerator и т.д.
// Примечание: на С++ это выглядит уродски, на C# вообще лишнего кода бы не было.
// Даже в Java с анонимными классами это бы выглядело лучше.
class XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const = 0;
};

class XmlCompiler::PropertiesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateProperties(out, isNotFirst, false);
	}
};

class XmlCompiler::ReferencePropertiesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateProperties(out, isNotFirst, true);
	}
};

class XmlCompiler::ContainedTypesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateContainedTypes(out, isNotFirst);
	}
};

class XmlCompiler::ConnectionsGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateConnections(out, isNotFirst);
	}
};

class XmlCompiler::UsagesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateUsages(out, isNotFirst);
	}
};

class XmlCompiler::PossibleEdgesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generatePossibleEdges(out, isNotFirst);
	}
};

class XmlCompiler::EnumValuesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateEnumValues(out, isNotFirst);
	}
};

void XmlCompiler::generateListMethod(OutFile &out, QString const &signature, ListMethodGenerator const &generator)
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

void XmlCompiler::generatePossibleEdges(utils::OutFile &out)
{
	PossibleEdgesGenerator generator;
		out() << "QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > " << mPluginName << "Plugin::getPossibleEdges(QString const &element) const\n"
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
	out() << "int " << mPluginName << "Plugin::isNodeOrEdge(QString const &element) const\n"
		<< "{\n";
	bool isFirst = true;
	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type* type, diagram->types().values()) {
			int result = 0;
			EdgeType* edge = dynamic_cast<EdgeType*>(type);
			NodeType* node = dynamic_cast<NodeType*>(type);
			if (edge)
				result = (-1);
			else if (node)
				result = 1;

			if (!isFirst)
				out() << "\telse ";
			else {
				isFirst = false;
				out() << "\t";
			}
			out() << "if (element == \""
				<< NameNormalizer::normalize(type->qualifiedName())
				<< "\")\n"
				<< "\t\treturn " << result << ";\n";
		}
	out() << "\treturn 0;\n}\n";
}

void XmlCompiler::generateProperties(OutFile &out)
{
	generateListMethod(out, "getPropertyNames(QString const &/*diagram*/, QString const &element)", PropertiesGenerator());
}

void XmlCompiler::generateReferenceProperties(OutFile &out)
{
	generateListMethod(out, "getReferenceProperties(QString const &/*diagram*/, QString const &element)", ReferencePropertiesGenerator());
}

void XmlCompiler::generateContainedTypes(OutFile &out)
{
	generateListMethod(out, "getTypesContainedBy(QString const &element)", ContainedTypesGenerator());
}

void XmlCompiler::generateConnections(OutFile &out)
{
	generateListMethod(out, "getConnectedTypes(QString const &element)", ConnectionsGenerator());
}

void XmlCompiler::generateUsages(utils::OutFile &out)
{
	generateListMethod(out, "getUsedTypes(QString const &element)", UsagesGenerator());
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
		if (diagram->getGroupsXML() != "")
			result = result + diagram->getGroupsXML();
	}
	out() << "\treturn QString::fromUtf8(\"" << result << "\");\n"
		<< "}\n\n";
}

void XmlCompiler::generateEnumValues(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::getEnumValues(QString name) const \n{\n"
		<< "\tQStringList result;\n";

	EnumValuesGenerator generator;
	bool isNotFirst = false;

	foreach (EnumType *type, mEditors[mCurrentEditor]->getAllEnumTypes())
		isNotFirst |= generator.generate(type, out, isNotFirst);

	if (!isNotFirst)
		out() << "\tQ_UNUSED(name);\n";
	out() << "\treturn result;\n"
		<< "}\n\n";
}
