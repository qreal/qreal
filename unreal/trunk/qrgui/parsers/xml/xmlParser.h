#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../kernel/ids.h"
#include <QMap>

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorManager;

	namespace parsers {

		class XmlParser
		{
		public:
			explicit XmlParser(qrRepo::RepoApi &api, EditorManager const &editorManager);

			void parseFile(QString const &fileName);

		private:
			qrRepo::RepoApi &mApi;
			EditorManager const &mEditorManager;
			Id mMetamodel;
			QHash<Id, QString> mElements;
			QHash<QString, QString> mContainerList;

			void initMetamodel(QDomDocument const &document);
			void createDiagramAttributes(QDomElement const &diagram, Id diagramId);
			void createNonGraphicElements(QDomElement const &type, Id diagramId);
			void createGraphicElements(QDomElement const &type, Id diagramId);
			void initEnum(QDomElement const &enumElement, Id diagramId);
			void initNode(QDomElement const &node, Id diagramId);
			void initEdge(QDomElement const &edge, Id diagramId);
			void initImport(QDomElement const &import, Id diagramId);
			void setStandartConfigurations(Id id, Id parent, QString const &name,
					QString const &displayedName);
			void setEnumAttributes(QDomElement const &enumElement, Id enumId);
			void setNodeAttributes(QDomElement const &node, Id nodeId);
			void setEdgeAttributes(QDomElement const &edge, Id edgeId);
			void setNodeConfigurations(QDomElement const &tag, Id nodeId);
			void setEdgeConfigurations(QDomElement const &tag, Id edgeId);
			void setGeneralization(QDomElement const &element, Id elementId);
			void setContainers(QDomElement const &element, Id elementId);
			void setProperties(QDomElement const &element, Id elementId);
			void setConnections(QDomElement const &element, Id elementId);
			void setAssotiations(QDomElement const &element, Id elementId);
			void setUsages(QDomElement const &element, Id elementId);
			void initDiagram(QDomElement const &diagram, Id const &parent,
					QString const &name, QString const &displayedName);
			void initProperty(QDomElement const &property, Id elementId);
			void initConnection(QDomElement const &connection, Id elementId);
			void initUsage(QDomElement const &usage, Id elementId);
			void initGeneralization(QDomElement const &generalization, Id elementId);
			void initContainer();
		};
	}
}

