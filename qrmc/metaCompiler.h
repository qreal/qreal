/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
		MetaCompiler(const QString &qrmcDir, qrRepo::LogicalRepoApi *mLogicalRepoApi);
		~MetaCompiler();
		bool compile(const QString &metamodel = "");
		Editor *loadMetaModel(const qReal::Id &id);
		Diagram *getDiagram(const QString &diagramName);
		void addResource(const QString &resourceName);

		QString getTemplateUtils(const QString &tmpl) const;

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

		bool changeDir(const QString &path);
		bool loadTemplateFromFile(const QString &templateFileName, QString &loadedTemplate);
		bool loadPluginHeaderTemplate();
		bool loadPluginSourceTemplate();
		bool loadTemplateUtils();
		bool loadNodeTemplate();

		void generateCode();
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
