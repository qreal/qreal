#pragma once

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDir>

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
	void generateExplosionsMappings(utils::OutFile &out);
	void generateNameMappingsRequests(utils::OutFile &out);
	void generateGraphicalObjectRequest(utils::OutFile &out);
	void generateIsParentOfRequest(utils::OutFile &out);
	void generateGetParentsOfRequest(utils::OutFile &out);
	void generateProperties(utils::OutFile &out);
	void generatePortTypes(utils::OutFile &out);
	void generateReferenceProperties(utils::OutFile &out);
	void generateContainedTypes(utils::OutFile &out);
	void generatePossibleEdges(utils::OutFile &out);
	void generateNodesAndEdges(utils::OutFile &out);
	void generateGroupsXML(utils::OutFile &out);
	void generateEnumValues(utils::OutFile &out);
	void generateEditableEnums(utils::OutFile &out);
	void generateResourceFile();
	void generatePropertyTypesRequests(utils::OutFile &out);
	void generatePropertyDefaultsRequests(utils::OutFile &out);
	void generatePaletteGroupsLists(utils::OutFile &out);
	void generatePaletteGroupsDescriptions(utils::OutFile &out);
	void generateShallPaletteBeSorted(utils::OutFile &out);

	class ListMethodGenerator;
	class PropertiesGenerator;
	class PortsGenerator;
	class ReferencePropertiesGenerator;
	class ContainedTypesGenerator;
	class PossibleEdgesGenerator;
	class EnumValuesGenerator;

	void generateListMethod(utils::OutFile &out, QString const &signature, ListMethodGenerator const &generator);

	QMap<QString, Editor *> mEditors;
	QString mPluginName;
	QString mPluginVersion;
	QString mResources;
	QString mCurrentEditor;
	QString mSourcesRootFolder;
};
