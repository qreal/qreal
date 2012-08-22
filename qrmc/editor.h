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
		void generate(QString const &headerTemplate, QString const &sourceTemplate,
					  QString const &nodeTemplate, QString const &edgeTemplate,
					  QString const & elementsHeaderTemplate, QString const &resourceTemplate,
					  QString const &projectTemplate, QMap<QString, QString> const &utils);

		Type *findType(QString const &name);
		QSet<EnumType*> getAllEnumTypes();
		Diagram *findDiagram(QString const &name);
		QMap<QString, Diagram*> diagrams();

		QString name();

	private:
		bool generatePluginHeader(QString const &headerTemplate);
		bool generatePluginSource();
		bool generateElementsClasses();
		bool generateResourceFile(QString const &resourceTemplate);
		bool generateProjectFile(QString const &projectTemplate);

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
		void generatePluginMethod(QString const &tag, MethodGenerator const &generator);
	};

}
