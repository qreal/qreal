#include "xmlCompiler.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "outFile.h"
#include "diagram.h"
#include "type.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <qDebug>

XmlCompiler::XmlCompiler()
{
	mResources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";
	QDir dir;
	if(!dir.exists("generated"))
	{
		dir.mkdir("generated");
	}
	dir.cd("generated");
	if(!dir.exists("shapes"))
	{
		dir.mkdir("shapes");
	}
	dir.cd("..");
}

XmlCompiler::~XmlCompiler()
{
	foreach(Editor *editor, mEditors.values())
	{
		if (editor)
		{
			delete editor;
		}
	}
}

void XmlCompiler::compile(QString const &inputXmlFileName)
{
	QFileInfo inputXmlFileInfo(inputXmlFileName);
	mPluginName = NameNormalizer::normalize(inputXmlFileInfo.baseName());
	mCurrentEditor = inputXmlFileName;
	loadXmlFile(inputXmlFileName);
	generateCode();
}

Editor* XmlCompiler::loadXmlFile(QString inputXmlFileName)
{
	qDebug() << "Loading file started" << inputXmlFileName;
	QFile inputXmlFile(inputXmlFileName);
	if (!inputXmlFile.open(QIODevice::ReadOnly))
	{
		QFileInfo inputXmlFileInfo(inputXmlFile);
		QDir inputXmlFileDir(inputXmlFileInfo.path());
		inputXmlFileDir.cd("../commonXml");
		inputXmlFileName = inputXmlFileDir.filePath(inputXmlFileInfo.fileName());
		qDebug() << "Loading file trying" << inputXmlFileName;
		inputXmlFile.setFileName(inputXmlFileName);
		if (!inputXmlFile.open(QIODevice::ReadOnly))
		{
			qDebug() << "Error: can't open file" << inputXmlFileName;
			return NULL;
		}
	}
	Editor *editor;
	if (mEditors.contains(inputXmlFileName))
	{
		editor = mEditors[inputXmlFileName];
		inputXmlFile.close();
		if (editor->isLoaded())
		{
			qDebug() << "File already loaded";
			return editor;
		}
		else
		{
			qDebug() << "Error: cycle detected";
			return NULL;
		}
	}
	else
	{
		QDomDocument inputXmlDomDocument;
		QString error = "";
		int errorLine = 0;
		int errorColumn = 0;
		if(!inputXmlDomDocument.setContent(&inputXmlFile, false, &error, &errorLine, &errorColumn))
		{
			qDebug() << "parse error" << "at" << errorLine << "," << errorColumn << error;
			inputXmlFile.close();
			return NULL;
		}
		inputXmlFile.close();
		editor = new Editor(inputXmlDomDocument, this);
		if (!editor->load())
		{
			qDebug() << "Failed to load file";
			delete editor;
			return NULL;
		}
		mEditors[inputXmlFileName] = editor;
		return editor;
	}
}

Diagram * XmlCompiler::getDiagram(QString const &diagramName)
{
	foreach (Editor *editor, mEditors)
	{
		Diagram *diagram = editor->findDiagram(diagramName);
		if (diagram)
		{
			return diagram;
		}
	}
	return NULL;
}

void XmlCompiler::generateCode()
{
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
		<< "#include \"../../../qrgui/umllib/uml_nodeelement.h\"\n\n"
		<< "#include \"../../../qrgui/umllib/uml_edgeelement.h\"\n\n"
		<< "#include <QBrush>\n\n"
		<< "#include <QPainter>\n\n"
		<< "namespace UML {\n\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
	{
		foreach (Type *type, diagram->types().values())
		{
			type->generateCode(out);
		}
	}
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
		<< "\n"
		<< "#include \"../../editorinterface.h\"\n"
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
		<< "\tvirtual QIcon getIcon(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual UML::Element* getGraphicalObject(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual QString editorName() const;\n"
		<< "\tvirtual QString diagramName(QString const &diagram) const;\n"
		<< "\tvirtual QString elementName(QString const &diagram, QString const &element) const;\n"
		<< "\n"
		<< "private:\n"
		<< "\tQMap<QString, QIcon> iconMap;\n"
		<< "\tQMap<QString, QString> diagramNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementsNameMap;\n"
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
}

void XmlCompiler::generateIncludes(OutFile &out)
{
	out() << "#include \"" << "pluginInterface.h\"\n" //mPluginName 
		<< "\n";

	out() << "#include \"" << "elements.h" << "\"\n";

	out() << "\n";
	out() << "Q_EXPORT_PLUGIN2(qreal_editors, " << mPluginName << "Plugin)\n\n"
		<< mPluginName << "Plugin::" << mPluginName << "Plugin(){\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n";
}

void XmlCompiler::generateNameMappings(OutFile &out)
{
	out() << "void " << mPluginName << "Plugin::initPlugin(){\n";

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
	{
		QString diagramName = NameNormalizer::normalize(diagram->name());
		out() << "\tdiagramNameMap[\"" << diagramName << "\"] = \"" << diagram->name() << " \"" << ";\n";
		out() << "\n";
	}

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
	{
		foreach (Type *type, diagram->types().values())
		{
			type->generateNameMapping(out);
		}
	}
	out() << "}\n\n";
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
		<< "}\n\n";
}

void XmlCompiler::generateGraphicalObjectRequest(OutFile &out)
{
	out() << "UML::Element* " << mPluginName
		<< "Plugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const\n{\n";

	bool notIsFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
	{
		foreach (Type *type, diagram->types().values())
		{
			notIsFirst |= type->generateObjectRequestString(out, notIsFirst);
		}
	}

	out() << "	else {\n"
		<< "		Q_ASSERT(!\"Request for creation of an element with unknown name\");\n"
		<< "		return NULL;\n"
		<< "	}\n";
	out() << "}\n\n";
}

void XmlCompiler::generateProperties(OutFile &out)
{
	out() << "QStringList " << mPluginName << "Plugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const\n"
		<< "{\n"
		<< "\tQStringList result;\n";

	bool notIsFirst = false;

	foreach (Diagram *diagram, mEditors[mCurrentEditor]->diagrams().values())
	{
		foreach (Type *type, diagram->types().values())
		{
			notIsFirst |= type->generateProperties(out, notIsFirst);
		}
	}

	out() << "\treturn result;\n"
		<< "}\n";
}

void XmlCompiler::generateResourceFile()
{
	OutFile out("generated/shapes/plugin.qrc");// mPluginName 
	out() << mResources
		<< "</qresource>\n"
		<< "</RCC>\n";
}