#pragma once

#include <QMap>
#include <QString>
#include <QDir>

class Editor;
class Diagram;
namespace utils {
	class OutFile;
}

class XmlCompiler
{
public:
	XmlCompiler();
	~XmlCompiler();
	bool compile(QString const &inputXmlFileName);
	Editor *loadXmlFile(QDir const &currentDir, QString const &inputXmlFileName);
	Diagram *getDiagram(QString const &diagramName);
	void addResource(QString const &resourceName);

private:
	QMap<QString, Editor*> mEditors;
	QString mPluginName;
	QString mResources;
	QString mCurrentEditor;

	void generateCode();
	void generateElementClasses();
	void generatePluginHeader();
	void generatePluginSource();
	void generateIncludes(utils::OutFile &out);
	void generateNameMappings(utils::OutFile &out);
	void generateNameMappingsRequests(utils::OutFile &out);
	void generateGraphicalObjectRequest(utils::OutFile &out);
	void generateProperties(utils::OutFile &out);
	void generateContainedTypes(utils::OutFile &out);
	void generateConnections(utils::OutFile &out);
	void generateResourceFile();

	class ListMethodGenerator;
	class PropertiesGenerator;
	class ContainedTypesGenerator;
	class ConnectionsGenerator;

	void generateListMethod(utils::OutFile &out, QString const &signature, ListMethodGenerator const &generator);
};
