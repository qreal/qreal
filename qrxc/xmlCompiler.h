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
	bool compile(QString const &inputXmlFileName, QString const &sourcesRootFolder);
	Editor *loadXmlFile(QDir const &currentDir, QString const &inputXmlFileName);
	Diagram *getDiagram(QString const &diagramName);
	void addResource(QString const &resourceName);

private:
	QMap<QString, Editor*> mEditors;
	QString mPluginName;
	QString mResources;
	QString mCurrentEditor;
	QString mSourcesRootFolder;

	void generateCode();
	void generateElementClasses();
	void generatePluginHeader();
	void generatePluginSource();
	void generateIncludes(utils::OutFile &out);
	void generateInitPlugin(utils::OutFile &out);
	void generateNameMappings(utils::OutFile &out);
	void generateMouseGestureMap(utils::OutFile &out);
	void generatePropertyMap(utils::OutFile &out);
	void generatePropertyDefaultsMap(utils::OutFile &out);
	void generateDescriptionMappings(utils::OutFile &out);
	void generateParentsMappings(utils::OutFile &out);
	void generateNameMappingsRequests(utils::OutFile &out);
	void generateGraphicalObjectRequest(utils::OutFile &out);
	void generateIsParentOfRequest(utils::OutFile &out);
	void generateGetParentsOfRequest(utils::OutFile &out);
	void generateProperties(utils::OutFile &out);
	void generateContainedTypes(utils::OutFile &out);
	void generateConnections(utils::OutFile &out);
	void generateUsages(utils::OutFile &out);
	void generatePossibleEdges(utils::OutFile &out);
	void generateNodesAndEdges(utils::OutFile &out);
	void generateEnumValues(utils::OutFile &out);
	void generateResourceFile();
	void generatePropertyTypesRequests(utils::OutFile &out);
	void generatePropertyDefaultsRequests(utils::OutFile &out);
	void generatePaletteGroupsLists(utils::OutFile &out);
	void generatePaletteGroupsDescriptions(utils::OutFile &out);

	class ListMethodGenerator;
	class PropertiesGenerator;
	class ContainedTypesGenerator;
	class ConnectionsGenerator;
	class UsagesGenerator;
	class PossibleEdgesGenerator;
	class EnumValuesGenerator;

	void generateListMethod(utils::OutFile &out, QString const &signature, ListMethodGenerator const &generator);
};
