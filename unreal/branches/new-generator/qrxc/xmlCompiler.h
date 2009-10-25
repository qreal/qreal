#pragma once

#include <QMap>
#include <QString>

class Editor;
class Diagram;
class OutFile;

class XmlCompiler
{
public:
	XmlCompiler();
	~XmlCompiler();
	void compile(QString const &inputXmlFileName);
	Editor *loadXmlFile(QString inputXmlFileName);
	Diagram *getDiagram(QString const &diagramName);
	void addResource(QString const &resourceName);

private:
	QMap<QString, Editor*> mEditors;
	QString mPluginName;
	QString mResources;

	void generateCode();
	void generateElementClasses();
	void generatePluginHeader();
	void generatePluginSource();
	void generateIncludes(OutFile &out);
	void generateNameMappings(OutFile &out);
	void generateNameMappingsRequests(OutFile &out);
	void generateGraphicalObjectRequest(OutFile &out);
	void generateProperties(OutFile &out);
	void generateResourceFile();
};
