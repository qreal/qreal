#pragma once

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDir>

#include "../../trunk/qrrepo/repoApi.h"

class Editor;
class Diagram;
namespace utils {
	class OutFile;
}

class MetaCompiler
{
public:
	MetaCompiler(QString const &workingCopyDir);
	~MetaCompiler();
	bool compile();
	Editor *loadMetaModel(qReal::Id const &id);
	Diagram *getDiagram(QString const &diagramName);
	void addResource(QString const &resourceName);

private:
	qrRepo::RepoApi mApi;
	QMap<QString, Editor*> mEditors;
	QString mPluginName;
	QString mResources;
	QString mCurrentEditor;

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
	void generateNameMappingsRequests(utils::OutFile &out);
	void generateGraphicalObjectRequest(utils::OutFile &out);
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

	class ListMethodGenerator;
	class PropertiesGenerator;
	class ContainedTypesGenerator;
	class ConnectionsGenerator;
	class UsagesGenerator;
	class PossibleEdgesGenerator;
	class EnumValuesGenerator;

};
