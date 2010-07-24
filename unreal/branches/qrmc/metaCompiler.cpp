#include "metaCompiler.h"
#include "editor.h"
#include "utils/nameNormalizer.h"
#include "utils/outFile.h"
#include "diagram.h"

#include "classes/type.h"
#include "classes/edgeType.h"
#include "classes/nodeType.h"
#include "classes/enumType.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

using namespace utils;
using namespace qReal;

MetaCompiler::MetaCompiler(QString const &workingCopyDir) : mApi(workingCopyDir)
{
	mApi.setName(ROOT_ID, ROOT_ID.toString());
}

MetaCompiler::~MetaCompiler()
{
	foreach(Editor *editor, mEditors.values())
		if (editor)
			delete editor;
}

bool MetaCompiler::compile()
{
	IdList rootItems = mApi.children(ROOT_ID);
	qDebug() << "root diagrams:" << rootItems.size();
	if (rootItems.isEmpty())
		qDebug() << "couldn't load any root diagrams";
	foreach(qReal::Id editorId, rootItems) {
		if (editorId.element() == "MetamodelDiagram") {
			mPluginName = NameNormalizer::normalize(mApi.property(editorId, "name of the directory")
											.toString().section("/", -1));
			if (!loadMetaModel(editorId))
				return false;
			generateCode();
		}
	}
	return true;
}

Editor* MetaCompiler::loadMetaModel(Id const &metamodelId)
{
	qDebug() << "Loading metamodel started: " << mApi.name(metamodelId);
	QString metamodelName = mApi.name(metamodelId);

	if (mEditors.contains(metamodelName)) {
		Editor *editor = mEditors[metamodelName];
		if (editor->isLoaded()) {
			qDebug() << "Metamodel already loaded";
			return editor;
		} else {
			qDebug() << "ERROR: cycle detected";
			return NULL;
		}
	} else {
		Editor *editor = new Editor(this, &mApi, metamodelId);
		if (!editor->load()) {
			qDebug() << "ERROR: Failed to load file";
			delete editor;
			return NULL;
		}
		mEditors[metamodelName] = editor;
		return editor;
	}
}

Diagram *MetaCompiler::getDiagram(QString const &diagramName)
{
	foreach (Editor *editor, mEditors) {
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram)
			return diagram;
	}
	return NULL;
}

void MetaCompiler::generateCode()
{
	qDebug() << "editors: " << mEditors.keys() << mCurrentEditor;

	foreach (Editor *editor, mEditors) {
		foreach (Diagram *diagram, editor->diagrams().values()) {
			diagram->print();
		}
	}

	return;
//	if (!mEditors.contains(mCurrentEditor)) {
//		qDebug() << "ERROR: Main editor xml was not loaded, generation aborted";
//		return;
//	}
//
//	generateElementClasses();
//	generatePluginHeader();
//	generatePluginSource();
//	generateResourceFile();
}

void MetaCompiler::addResource(QString const &resourceName)
{
	mResources += resourceName;
}

void MetaCompiler::generateElementClasses()
{
	OutFile out("generated/elements.h");
	out() << "#pragma once\n\n"
		<< "#include <QBrush>\n"
		<< "#include <QPainter>\n\n"
		<< "#include \"../../../qrgui/umllib/elementImpl.h\"\n"
		<< "#include \"../../../qrgui/umllib/uml_element.h\"\n"
		<< "#include \"../../../qrgui/umllib/elementRepoInterface.h\"\n"
		<< "#include \"../../../qrgui/umllib/elementTitleHelpers.h\"\n\n"
		<< "namespace UML {\n\n";

//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			type->generateCode(out);

	out() << "}\n\n";
}

void MetaCompiler::generatePluginHeader()
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
		<< "#include \"../../editorInterface.h\"\n"
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
		<< "\tvirtual UML::ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString getPropertyType(QString const &element, QString const &property) const;\n"
		<< "\tvirtual QString getPropertyDefaultValue(QString const &element, QString const &property) const;\n"
		<< "\tvirtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QStringList getEnumValues(QString name) const;\n"
		<< "\n"
		<< "\tvirtual QString editorName() const;\n"
		<< "\tvirtual QString diagramName(QString const &diagram) const;\n"
		<< "\tvirtual QString diagramNodeName(QString const &diagram) const;\n"
		<< "\tvirtual QString elementName(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString elementMouseGesture(QString const &digram, QString const &element) const;\n"
		<< "\n"
		<< 	"\tvirtual QList<qReal::ListenerInterface*> listeners() const;\n"
		<< "\n"
		<< "private:\n"
		<< "\tQMap<QString, QIcon> iconMap;\n"
		<< "\tQMap<QString, QString> diagramNameMap;\n"
		<< "\tQMap<QString, QString> diagramNodeNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > propertyTypes;\n"
		<< "\tQMap<QString, QMap<QString, QString> > propertyDefault;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementsNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementMouseGesturesMap;\n"
		<< "};\n"
		<< "\n";
}

void MetaCompiler::generatePluginSource()
{
	QString fileName = "generated/pluginInterface.cpp"; //mPluginName

	OutFile out(fileName);

//	generateIncludes(out);
//	generateInitPlugin(out);
//	generateNameMappingsRequests(out);
//	generateGraphicalObjectRequest(out);
//	generateProperties(out);
//	generateContainedTypes(out);
//	generateConnections(out);
//	generateUsages(out);
//	generatePossibleEdges(out);
//	generateNodesAndEdges(out);
//	generateEnumValues(out);
//	generatePropertyTypesRequests(out);
//	generatePropertyDefaultsRequests(out);
//
//	mEditors[mCurrentEditor]->generateListenerFactory(out, mPluginName);
}

void MetaCompiler::generateIncludes(OutFile &out)
{
	out() << "#include \"" << "pluginInterface.h\"\n" //mPluginName
		<< "\n";

	out() << "#include \"" << "elements.h" << "\"\n";

	out() << "\n";

//	mEditors[mCurrentEditor]->generateListenerIncludes(out);

	out() << "Q_EXPORT_PLUGIN2(qreal_editors, " << mPluginName << "Plugin)\n\n"
		<< mPluginName << "Plugin::" << mPluginName << "Plugin()\n{\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n";
}
void MetaCompiler::generateInitPlugin(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPlugin()\n{\n"
		<< "\tinitNameMap();\n"
		<< "\tinitMouseGestureMap();\n"
		<< "\tinitPropertyMap();\n"
		<< "\tinitPropertyDefaultsMap();\n"
		<< "}\n\n";

//	generateNameMappings(out);
//	generateMouseGestureMap(out);
//	generatePropertyMap(out);
//	generatePropertyDefaultsMap(out);
}

void MetaCompiler::generateNameMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initNameMap()\n{\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tdiagramNameMap[\"" << diagramName << "\"] = \"" << diagram->displayedName() << " \"" << ";\n";
		out() << "\tdiagramNodeNameMap[\"" << diagramName << "\"] = \"" << diagram->nodeName() << "\"" << ";\n";
		out() << "\n";
	}

//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			type->generateNameMapping(out);

	// property types

	out() << "}\n\n";
}

void MetaCompiler::generateMouseGestureMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initMouseGestureMap()\n{\n";
//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			type->generateMouseGesturesMap(out);
	out() << "}\n\n";
}

void MetaCompiler::generatePropertyMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPropertyMap()\n{\n";
//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			type->generatePropertyTypes(out);
	out() << "}\n\n";

}

void MetaCompiler::generatePropertyDefaultsMap(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPropertyDefaultsMap()\n{\n";
//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			type->generatePropertyDefaults(out);
	out() << "}\n\n";

}

void MetaCompiler::generatePropertyTypesRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyType(QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertyTypes[element][property];\n" // TODO: merge with getPropertyNames()!!11
		<< "}\n\n";
}

void MetaCompiler::generatePropertyDefaultsRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyDefaultValue(QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertyDefault[element][property];\n" // TODO: merge with getPropertyNames()!!11
		<< "}\n\n";
}

void MetaCompiler::generateNameMappingsRequests(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::diagrams() const\n{\n"
		<< "\treturn diagramNameMap.keys();\n"
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

		<< "QString " << mPluginName << "Plugin::elementMouseGesture(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementMouseGesturesMap[diagram][element];\n"
		<< "}\n\n";
}

void MetaCompiler::generateGraphicalObjectRequest(OutFile &out)
{
	out() << "UML::ElementImpl* " << mPluginName
		<< "Plugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const\n{\n";

	bool isNotFirst = false;

//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			isNotFirst |= type->generateObjectRequestString(out, isNotFirst);

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
class MetaCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const = 0;
};

class MetaCompiler::PropertiesGenerator: public MetaCompiler::ListMethodGenerator {
public:
//	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
//		return type->generateProperties(out, isNotFirst);
//	}
};

class MetaCompiler::ContainedTypesGenerator: public MetaCompiler::ListMethodGenerator {
public:
//	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
//		return type->generateContainedTypes(out, isNotFirst);
//	}
};

class MetaCompiler::ConnectionsGenerator: public MetaCompiler::ListMethodGenerator {
public:
//	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
//		return type->generateConnections(out, isNotFirst);
//	}
};

class MetaCompiler::UsagesGenerator: public MetaCompiler::ListMethodGenerator {
public:
//	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
//		return type->generateUsages(out, isNotFirst);
//	}
};

class MetaCompiler::PossibleEdgesGenerator: public MetaCompiler::ListMethodGenerator {
public:
//	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
//		return type->generatePossibleEdges(out, isNotFirst);
//	}
};

class MetaCompiler::EnumValuesGenerator: public MetaCompiler::ListMethodGenerator {
public:
//	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
//		return type->generateEnumValues(out, isNotFirst);
//	}
};

//void MetaCompiler::generateListMethod(OutFile &out, QString const &signature, ListMethodGenerator const &generator)
//{
//	out() << "QStringList " << mPluginName << "Plugin::" << signature << " const\n"
//		<< "{\n"
//		<< "\tQStringList result;\n";
//
//	bool isNotFirst = false;
//
////	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
////		foreach (Type *type, diagram->types().values())
////			isNotFirst |= generator.generate(type, out, isNotFirst);
//
//	if (!isNotFirst)
//		out() << "\tQ_UNUSED(element);\n";
//	out() << "\treturn result;\n"
//		<< "}\n\n";
//}

void MetaCompiler::generatePossibleEdges(utils::OutFile &out)
{
//	PossibleEdgesGenerator generator;
		out() << "QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > " << mPluginName << "Plugin::getPossibleEdges(QString const &element) const\n"
			<< "{\n"
			<< "\tQList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;\n";
	bool isNotFirst = false;

//	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
//		foreach (Type *type, diagram->types().values())
//			isNotFirst |= generator.generate(type, out, isNotFirst);

	if (!isNotFirst)
		out() << "\tQ_UNUSED(element);\n";
		out() << "\treturn result;\n"
		<< "}\n\n";
}

void MetaCompiler::generateNodesAndEdges(utils::OutFile &out)
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

void MetaCompiler::generateProperties(OutFile &)
{
//	generateListMethod(out, "getPropertyNames(QString const &/*diagram*/, QString const &element)", PropertiesGenerator());
}

void MetaCompiler::generateContainedTypes(OutFile &)
{
//	generateListMethod(out, "getTypesContainedBy(QString const &element)", ContainedTypesGenerator());
}

void MetaCompiler::generateConnections(OutFile &)
{
//	generateListMethod(out, "getConnectedTypes(QString const &element)", ConnectionsGenerator());
}

void MetaCompiler::generateUsages(utils::OutFile &)
{
//	generateListMethod(out, "getUsedTypes(QString const &element)", UsagesGenerator());
}

void MetaCompiler::generateResourceFile()
{
	OutFile out("generated/shapes/plugin.qrc");// mPluginName
	out() << mResources
		<< "</qresource>\n"
		<< "</RCC>\n";
}

void MetaCompiler::generateEnumValues(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::getEnumValues(QString name) const \n{\n"
		<< "\tQStringList result;\n";

//	EnumValuesGenerator generator;
	bool isNotFirst = false;

//	foreach (EnumType *type, mEditors[mCurrentEditor]->getAllEnumTypes())
//		isNotFirst |= generator.generate(type, out, isNotFirst);

	if (!isNotFirst)
		out() << "\tQ_UNUSED(name);\n";
	out() << "\treturn result;\n"
		<< "}\n\n";
}
