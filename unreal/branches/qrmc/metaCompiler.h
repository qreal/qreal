#pragma once

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QList>

#include "utils/defs.h"
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

	QString getTemplateUtils(QString const &tmpl) const;

private:
	qrRepo::RepoApi mApi;
	QMap<QString, Editor*> mEditors;
	QString mPluginName;
	QString mResources;
	QString mCurrentEditor;

	QString mPluginHeaderTemplate;
	QString mPluginSourceTemplate;
	QString mNodeTemplate;
	QString mEdgeTemplate;
	QMap<QString, QString> mTemplateUtils;

	QDir mDirectory;

	bool changeDir(const QString &path);
	bool loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate);
	bool loadPluginHeaderTemplate();
	bool loadPluginSourceTemplate();
	bool loadTemplateUtils();
	bool loadNodeTemplate();

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
