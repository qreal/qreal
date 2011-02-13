#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include <QtCore/QPair>

#include "../qrrepo/repoApi.h"

namespace qrmc {
	class Type;
	class Editor;

	class Diagram
	{
	public:
                Diagram(qReal::Id const &id, qrRepo::RepoApi *api, Editor *editor);
		~Diagram();
		bool init();
		bool resolve();
		Editor *editor() const;
		Type *findType(QString name);
		QMap<QString, Type*> types() const;
		QString name() const;
		QString nodeName() const;
		QString displayedName() const;

		QString generateNamesMap(QString const &lineTemplate) const;
		QString generateMouseGesturesMap(QString const &lineTemplate) const;
		QString generatePropertiesMap(QString const &lineTemplate) const;
		QString generatePropertyDefaultsMap(QString const &lineTemplate) const;
		QString generateContainers(QString const &lineTemplate) const;
		QString generateConnections(QString const &lineTemplate) const;
		QString generateUsages(QString const &lineTemplate) const;
		QString generateFactory(QString const &lineTemplate) const;
		QString generateIsNodeOrEdge(QString const &lineTemplate) const;
		QString generateEnums(QString const &lineTemplate) const;
		QString generatePossibleEdges(QString const &lineTemplate) const;

		QString generateNodeClasses(QString const &nodeTemplate) const;
		QString generateEdgeClasses(QString const &edgeTemplate) const;

		QString generateResourceFile(QString const &resourceTemplate) const;

		void print();

	private:
		struct ImportSpecification {
			QString name;
			QString as;
			QString displayedName;
		};
                qReal::Id mId;
		qrRepo::RepoApi *mApi;
		QMap<QString, Type*> mTypes;
		QString mDiagramName;
		QString mDiagramNodeName; // TODO: replace with QStringList for multiple nodeNames
		QString mDiagramDisplayedName;
		Editor *mEditor;
		QList<ImportSpecification> mImports;

		class ListMethodGenerator;
		class UsagesGenerator;
		class ConnectionsGenerator;
		class ContainersGenerator;
		class FactoryGenerator;
		class IsNodeOrEdgeGenerator;
		class EnumsGenerator;
		class PossibleEdgesGenerator;
		QString generateListMethod(QString const &lineTemplate, ListMethodGenerator const &generator) const;

		class MapMethodGenerator;
		class NamesGenerator;
		class MouseGesturesGenerator;
		class PropertyNamesGenerator;
		class PropertyDefaultsGenerator;
		class NodesGenerator;
		class EdgesGenerator;
		class ResourceGenerator;
		QString generateMapMethod(QString const& lineTemplate, MapMethodGenerator const &generator) const;
	};
}
