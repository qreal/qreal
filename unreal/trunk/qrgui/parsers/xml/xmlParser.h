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
			void loadIncludeList(QString const &fileName);

		private:
			qrRepo::RepoApi &mApi;
			EditorManager const &mEditorManager;
			Id mMetamodel;
			Id mDiagram;
			QHash<Id, QString> mElements;
			QHash<Id, QStringList> mContainerList;
			int mElementsColumn;
			int mElementCurrentColumn;
			int mMoveWidth;
			int mMoveHeight;
			int mCurrentWidth;
			int mCurrentHeight;
			QStringList mIncludeList;

			QStringList getIncludeList(QString const &fileName);
			Id getPackageId();
			void initMetamodel(QDomDocument const &document, QString const &directoryName, Id const &id);
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
			void setBoolValuesForContainer(QString const &tagName, QDomElement const &property, Id const &id);
			void setSizesForContainer(QString const &tagName, QDomElement const &property, Id const &id);
			void setProperties(QDomElement const &element, Id const &elementId);
			void setConnections(QDomElement const &element, Id const &elementId);
			void setAssociations(QDomElement const &element, Id const &elementId);
			void setUsages(QDomElement const &element, Id const &elementId);
			void setPossibleEdges(QDomElement const &element, Id const &elementId);
			void setFields(QDomElement const &element, Id const &elementId);
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
			Id findIdByType(QString const &type) const;

			void setChildrenPositions(Id const &id, unsigned cellWidth, unsigned cellHeight);
			void setElementPosition(Id const &id);
			void checkIndex();
		};
	}
}

