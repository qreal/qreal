#pragma once

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QList>

#include "utils/defs.h"
#include "../qrrepo/repoApi.h"

namespace qrmc {
	class Editor;
	class Diagram;

	class MetaCompiler
	{
	public:
		MetaCompiler(QString const &qrmcDir, qrRepo::LogicalRepoApi *mLogicalRepoApi);
		~MetaCompiler();
		bool compile(QString const &metamodel = ""
				, QString const pathToQrealRoot = "../../../"
				, QString const generatedCodeDir = "../qrmc/plugins"
				, QString const destinationDir = "bin/plugins");
		Editor *loadMetaModel(qReal::Id const &id, QString const generatedCodeDir);
		Diagram *getDiagram(QString const &diagramName);
		void addResource(QString const &resourceName);

		QString getTemplateUtils(QString const &tmpl) const;

	private:
		qrRepo::LogicalRepoApi *mApi;
		QMap<QString, Editor*> mEditors;

		QString mLocalDir;
		QString mPluginName;
		QString mResources;
		QString mCurrentEditor;

		QString mPluginHeaderTemplate;
		QString mPluginSourceTemplate;
		QString mNodeTemplate;
		QString mEdgeTemplate;
		QString mElementsHeaderTemplate;
		QString mResourceTemplate;
		QString mProjectTemplate;
		QString mPluginsProjectTemplate;
		QMap<QString, QString> mTemplateUtils;

		QDir mDirectory;

		QString mTargetMetamodel;

		bool changeDir(QString const &path);
		bool loadTemplateFromFile(QString const &templateFileName, QString &loadedTemplate);
		bool loadPluginHeaderTemplate();
		bool loadPluginSourceTemplate();
		bool loadTemplateUtils();
		bool loadNodeTemplate();

		void generateCode(QString const &generatedCodeDir = "../qrmc/plugins"
				, QString const &pathToQrealRoot = "../../../"
				, QString const &destinationDir = "bin/plugins/");
		void generateElementClasses();
		void generatePluginHeader();
		void generatePluginSource();
		void generateResourceFile();


		class ListMethodGenerator;
		class PropertiesGenerator;
		class ContainedTypesGenerator;
		class ConnectionsGenerator;
		class UsagesGenerator;
		class PossibleEdgesGenerator;
		class EnumValuesGenerator;

	};
}
