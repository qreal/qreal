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

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QPair>
#include <QtCore/QSet>

#include "../qrrepo/repoApi.h"
#include "classes/port.h"

namespace qrmc {
	class MetaCompiler;
	class Diagram;
	class Type;
	class EnumType;

	namespace utils {
		class OutFile;
	}

	class Editor
	{
	public:
		Editor(MetaCompiler *metaCompiler, qrRepo::LogicalRepoApi *api, const qReal::Id &id);
		~Editor();
		MetaCompiler *metaCompiler();
		qReal::Id id();

		bool isLoaded();
		bool load();
		void generate(const QString &headerTemplate, const QString &sourceTemplate,
					  const QString &nodeTemplate, const QString &edgeTemplate,
					  const QString &elementsHeaderTemplate, const QString &resourceTemplate,
					  const QString &projectTemplate, QMap<QString, QString> const &utils);

		Type *findType(const QString &name);
		QSet<EnumType*> getAllEnumTypes();
		QStringList getAllPortNames() const;
		Diagram *findDiagram(const QString &name);
		QMap<QString, Diagram*> diagrams();

		QString name();

	private:
		bool generatePluginHeader(const QString &headerTemplate);
		bool generatePluginSource();
		bool generateElementsClasses();
		bool generateResourceFile(const QString &resourceTemplate);
		bool generateProjectFile(const QString &projectTemplate);

		void generateDiagramsMap();
		void generateDiagramNodeNamesMap();
		void generateNamesMap();
		void generateMouseGesturesMap();
		void generatePropertiesMap();
		void generatePropertyDisplayedNamesMap();
		void generateElementDescriptionMap();
		void generateParentsMap();
		void generatePropertyDefaultsMap();
		void generateElementsFactory();
		void generateContainers();
		void generateReferenceProperties();
		void generatePortTypes();
		void generatePropertyNames();
		void generateConnections();
		void generateUsages();
		void generateIsNodeOrEdge();
		void generateEnums();
		void generatePossibleEdges();

		MetaCompiler *mMetaCompiler;
		qrRepo::LogicalRepoApi *mApi;
		qReal::Id mId;
		QString mName;
		QString mNameOfMetamodel;
		bool mLoadingComplete;
		QList<Editor*> mIncludes;
		QMap<QString, Diagram*> mDiagrams;

		QMap<QString, QString> mUtilsTemplate;
		QString mSourceTemplate;
		QString mNodeTemplate;
		QString mEdgeTemplate;
		QString mElementsHeaderTemplate;

		class MethodGenerator;
		class ContainersGenerator;
		class ReferencePropertiesGenerator;
		class PortTypesGenerator;
		class PropertyNameGenerator;
		class ConnectionsGenerator;
		class UsagesGenerator;
		class FactoryGenerator;
		class IsNodeOrEdgeGenerator;
		class NamesGenerator;
		class MouseGesturesGenerator;
		class PropertiesGenerator;
		class PropertyDefaultsGenerator;
		class PropertyDisplayedNamesGenerator;
		class ElementDescriptionGenerator;
		class ParentsMapGenerator;
		class PossibleEdgesGenerator;
		void generatePluginMethod(const QString &tag, const MethodGenerator &generator);
	};

}
