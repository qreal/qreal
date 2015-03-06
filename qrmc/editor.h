#pragma once

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QPair>
#include <QtCore/QSet>

#include "../qrrepo/repoApi.h"

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
		Editor(MetaCompiler *metaCompiler, qrRepo::LogicalRepoApi *api, qReal::Id const &id);
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
		void generateParentsMap();
		void generatePropertyDefaultsMap();
		void generateElementsFactory();
		void generateContainers();
		void generateReferenceProperties();
		void generateConnections();
		void generateUsages();
		void generateIsNodeOrEdge();
		void generateEnums();
		void generatePossibleEdges();

		MetaCompiler *mMetaCompiler;
		qrRepo::LogicalRepoApi *mApi;
		qReal::Id mId;
		QString mName;
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
		class ConnectionsGenerator;
		class UsagesGenerator;
		class FactoryGenerator;
		class IsNodeOrEdgeGenerator;
		class NamesGenerator;
		class MouseGesturesGenerator;
		class PropertiesGenerator;
		class PropertyDefaultsGenerator;
		class PropertyDisplayedNamesGenerator;
		class ParentsMapGenerator;
		class PossibleEdgesGenerator;
		void generatePluginMethod(const QString &tag, MethodGenerator const &generator);
	};

}
