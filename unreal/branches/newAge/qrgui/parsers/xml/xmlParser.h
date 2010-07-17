#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../kernel/ids.h"
#include <QtCore/QHash>
#include <QtCore/QFileInfo>

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

			void initMetamodel(QDomDocument const &document, QString const &directoryName);
			void createDiagramAttributes(QDomElement const &diagram, Id const &diagramId);
			void createNonGraphicElements(QDomElement const &type, Id const &diagramId);
			void createGraphicElements(QDomElement const &type, Id const &diagramId);
			void initEnum(QDomElement const &enumElement, Id const &diagramId);
			void initNode(QDomElement const &node, Id const &diagramId);
			void initEdge(QDomElement const &edge, Id const &diagramId);
			void initImport(QDomElement const &import, Id const &diagramId);
			void setStandartConfigurations(Id const &id, Id const &parent, QString const &name,
					QString const &displayedName);
			void setEnumAttributes(QDomElement const &enumElement, Id const &enumId);
			void setNodeAttributes(QDomElement const &node, Id const &nodeId);
			void setEdgeAttributes(QDomElement const &edge, Id const &edgeId);
			void setNodeConfigurations(QDomElement const &tag, Id const &nodeId);
			void setEdgeConfigurations(QDomElement const &tag, Id const &edgeId);
			void setGeneralization(QDomElement const &element, Id const &elementId);
			void setContainers(QDomElement const &element, Id const &elementId);
			void setContainerProperties(QDomElement const &element, Id const &elementId);
			void setProperties(QDomElement const &element, Id const &elementId);
			void setConnections(QDomElement const &element, Id const &elementId);
			void setAssotiations(QDomElement const &element, Id const &elementId);
			void setUsages(QDomElement const &element, Id const &elementId);
			void setPossibleEdges(QDomElement const &element, Id const &elementId);
			void setPin(Id const &elementId);
			void setAction(Id const &elementId);
			void setLineType(QDomElement const &tag, Id const &edgeId);
			void initPossibleEdge(QDomElement const &possibleEdge, Id const &elementId);
			void initDiagram(QDomElement const &diagram, Id const &parent,
					QString const &name, QString const &displayedName);
			void initProperty(QDomElement const &property, Id const &elementId);
			void initConnection(QDomElement const &connection, Id const &elementId);
			void initUsage(QDomElement const &usage, Id const &elementId);
			void initGeneralization(QDomElement const &generalization, Id const &elementId);
			void initContainer();
		};
	}
}

