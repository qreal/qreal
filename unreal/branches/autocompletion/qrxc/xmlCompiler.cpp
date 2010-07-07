#include "xmlCompiler.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "../utils/outFile.h"
#include "../utils/xmlUtils.h"
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
	if(!dir.exists("generated"))
		dir.mkdir("generated");
	dir.cd("generated");
	if(!dir.exists("shapes"))
		dir.mkdir("shapes");
	dir.cd("..");
}

XmlCompiler::~XmlCompiler()
{
	foreach(Editor *editor, mEditors.values())
		if (editor)
			delete editor;
}

bool XmlCompiler::compile(QString const &inputXmlFileName)
{
	QFileInfo const inputXmlFileInfo(inputXmlFileName);
	mPluginName = NameNormalizer::normalize(inputXmlFileInfo.baseName());
	mCurrentEditor = inputXmlFileInfo.absoluteFilePath();
	QDir const startingDir = inputXmlFileInfo.dir();
	if (!loadXmlFile(startingDir, inputXmlFileInfo.fileName()))
		return false;
	generateCode();
	return true;
}

Editor* XmlCompiler::loadXmlFile(QDir const &currentDir, QString const &inputXmlFileName)
{
	QFileInfo fileInfo(inputXmlFileName);
	Q_ASSERT(fileInfo.fileName() == inputXmlFileName);  // Проверяем, что нам передали только имя файла, без пути.

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
		if (diagram)
			return diagram;
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
	mResources += resourceName;
}

void XmlCompiler::generateElementClasses()
{
	OutFile out("generated/elements.h");
	out() << "#pragma once\n\n"
		<< "#include <QBrush>\n"
		<< "#include <QPainter>\n\n"
		<< "#include \"../../../qrgui/umllib/elementImpl.h\"\n"
		<< "#include \"../../../qrgui/umllib/elementRepoInterface.h\"\n"
		<< "#include \"../../../qrgui/umllib/sdfrenderer.h\"\n\n"
		<< "namespace UML {\n\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
            foreach (Type *type, diagram->types().values())
			type->generateCode(out);

	out() << "}\n\n";
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
		<< "\n"
		<< "\tvirtual QString id() const { return \"" << mPluginName << "\"; }\n"
		<< "\n"
		<< "\tvirtual QStringList diagrams() const;\n"
		<< "\tvirtual QStringList elements(QString const &diagram) const;\n"
		<< "\n"
		<< "\tvirtual QStringList getTypesContainedBy(QString const &element) const;\n"
		<< "\tvirtual QStringList getConnectedTypes(QString const &element) const;\n"
		<< "\tvirtual QStringList getUsedTypes(QString const &element) const;\n"
		<< "\tvirtual QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > getPossibleEdges(QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual int isNodeOrEdge(QString const &element) const; \n"
		<< "\n"
		<< "\tvirtual QIcon getIcon(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual UML::ElementImpl* getGraphicalObject(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString getPropertyType(QString const &element, QString const &property) const;\n"
		<< "\tvirtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QStringList getEnumValues(QString name) const;\n"
		<< "\n"
		<< "\tvirtual QString editorName() const;\n"
		<< "\tvirtual QString diagramName(QString const &diagram) const;\n"
		<< "\tvirtual QString elementName(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QString elementMouseGesture(QString const &digram, QString const &element) const;\n"
		<< "\n"
		<< 	"\tvirtual QList<qReal::Listener*> listeners() const;\n"
		<< "\n"
		<< "private:\n"
		<< "\tQMap<QString, QIcon> iconMap;\n"
		<< "\tQMap<QString, QString> diagramNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > propertyTypes;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementsNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementMouseGesturesMap;\n"
		<< "};\n"
		<< "\n";
}

void XmlCompiler::generatePluginSource()
{
	QString fileName = "generated/pluginInterface.cpp"; //mPluginName

	OutFile out(fileName);

	generateIncludes(out);
	generateNameMappings(out);
	generateNameMappingsRequests(out);
	generateGraphicalObjectRequest(out);
	generateProperties(out);
	generateContainedTypes(out);
	generateConnections(out);
	generateUsages(out);
	generatePossibleEdges(out);
	generateNodesAndEdges(out);
	generateEnumValues(out);
	generatePropertyTypesRequests(out);

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
		<< mPluginName << "Plugin::" << mPluginName << "Plugin(){\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n";
}

void XmlCompiler::generateNameMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPlugin(){\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values()) {
		QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tdiagramNameMap[\"" << diagramName << "\"] = \"" << diagram->displayedName() << " \"" << ";\n";
		out() << "\n";
	}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			type->generateNameMapping(out);

	// property types

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values())
			type->generatePropertyTypes(out);
	out() << "}\n\n";
}

void XmlCompiler::generatePropertyTypesRequests(OutFile &out)
{
	out() << "QString " << mPluginName << "Plugin::getPropertyType(QString const &element, QString const &property) const\n{\n"
		<< "\treturn propertyTypes[element][property];\n" // TODO: merge with getPropertyNames()!!11
		<< "}\n\n";
}

void XmlCompiler::generateNameMappingsRequests(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::diagrams() const\n{\n"
		<< "\treturn diagramNameMap.keys();\n"
		<< "}\n\n"

		<< "QStringList " << mPluginName << "Plugin::elements(QString const &diagram) const\n{\n"
		<< "\treturn elementsNameMap[diagram].keys();\n"
		<< "}\n\n"

		<< "QIcon " << mPluginName << "Plugin::getIcon(QString const &/*diagram*/, QString const &element) const\n{\n"
		<< "\treturn QIcon(new SdfIconEngineV2(\":/\" + element + \"Class.sdf\"));\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::editorName() const\n{\n"
		<< "\t return \"" << mPluginName << " Editor\";\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::diagramName(QString const &diagram) const\n{\n"
		<< "\treturn diagramNameMap[diagram];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementName(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementsNameMap[diagram][element];\n"
		<< "}\n\n"

		<< "QString " << mPluginName << "Plugin::elementMouseGesture(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementMouseGesturesMap[diagram][element];\n"
		<< "}\n\n";
}

void XmlCompiler::generateGraphicalObjectRequest(OutFile &out)
{
	out() << "UML::ElementImpl* " << mPluginName
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
class XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const = 0;
};

class XmlCompiler::PropertiesGenerator: public XmlCompiler::ListMethodGenerator {
public:
	virtual bool generate(Type *type, OutFile &out, bool isNotFirst) const {
		return type->generateProperties(out, isNotFirst);
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
                        << NameNormalizer::normalize(diagram->name()+"_"+type->displayedName())
                        << "\")\n"
                    << "\t\treturn " << result << ";\n";
            }
        out() << "\treturn 0;\n}\n";
}

void XmlCompiler::generateProperties(OutFile &out)
{
	generateListMethod(out, "getPropertyNames(QString const &/*diagram*/, QString const &element)", PropertiesGenerator());
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
	OutFile out("generated/shapes/plugin.qrc");// mPluginName
	out() << mResources
		<< "</qresource>\n"
		<< "</RCC>\n";
}

void XmlCompiler::generateEnumValues(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::getEnumValues(QString name) const \n{\n"
		<< "\tQStringList result;\n";

	EnumValuesGenerator generator;
	bool isNotFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
		foreach (Type *type, diagram->types().values()){
			isNotFirst |= generator.generate(type, out, isNotFirst);
		}

	if (!isNotFirst)
		out() << "\tQ_UNUSED(name);\n";
	out() << "\treturn result;\n"
		<< "}\n\n";
}
