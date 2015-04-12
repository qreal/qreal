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
		Diagram(const qReal::Id &id, qrRepo::LogicalRepoApi *api, Editor *editor);
		~Diagram();
		bool init();
		bool resolve();
		Editor *editor() const;
		Type *findType(QString name);
		QMap<QString, Type*> types() const;
		QString name() const;
		QString nodeName() const;
		QString displayedName() const;

		QString generateNamesMap(const QString &lineTemplate) const;
		QString generateMouseGesturesMap(const QString &lineTemplate) const;
		QString generatePropertiesMap(const QString &lineTemplate) const;
		QString generatePropertyDefaultsMap(const QString &lineTemplate) const;
		QString generatePropertyDisplayedNamesMap(const QString &lineTemplate) const;
		QString generateParentsMap(const QString &lineTemplate) const;
		QString generateContainers(const QString &lineTemplate) const;
		QString generateReferenceProperties(const QString &lineTemplate) const;
		QString generatePortTypes(const QString &lineTemplate) const;//fix
		QString generatePropertyName(const QString &lineTemplate) const;
		QString generateConnections(const QString &lineTemplate) const;
		QString generateUsages(const QString &lineTemplate) const;
		QString generateFactory(const QString &lineTemplate) const;
		QString generateIsNodeOrEdge(const QString &lineTemplate) const;
		QString generateEnums(const QString &lineTemplate) const;
		QString generatePossibleEdges(const QString &lineTemplate) const;

		QString generateNodeClasses(const QString &nodeTemplate) const;
		QString generateEdgeClasses(const QString &edgeTemplate) const;

		QString generateResourceFile(const QString &resourceTemplate) const;

		void print();

	private:
		struct ImportSpecification {
			QString name;
			QString as;
			QString displayedName;
		};
		qReal::Id mId;
		qrRepo::LogicalRepoApi *mApi;
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
		class ReferencePropertiesGenerator;
		class PortTypesGenerator;//fix
		class PropertyNameGenerator;
		class FactoryGenerator;
		class IsNodeOrEdgeGenerator;
		class EnumsGenerator;
		class PossibleEdgesGenerator;
		QString generateListMethod(const QString &lineTemplate, const ListMethodGenerator &generator) const;

		class MapMethodGenerator;
		class NamesGenerator;
		class MouseGesturesGenerator;
		class PropertyNamesGenerator;
		class PropertyDefaultsGenerator;
		class PropertyDisplayedNamesGenerator;
		class ParentsMapGenerator;
		class NodesGenerator;
		class EdgesGenerator;
		class ResourceGenerator;
		QString generateMapMethod(const QString& lineTemplate, const MapMethodGenerator &generator) const;
	};
}
