#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../kernel/NewType.h"
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
                        NewType mMetamodel;
                        QHash<QString, NewType> mElements;
                        QHash<NewType, QStringList> mParents;
                        QHash<NewType, QStringList> mContainers;
			int mElementsColumn;
			int mElementCurrentColumn;
			int mMoveWidth;
			int mMoveHeight;
			int mCurrentWidth;
			int mCurrentHeight;
			int mParentPositionX;

			QStringList getIncludeList(QString const &fileName);
                        NewType getPackageId();
                        void initMetamodel(QDomDocument const &document, QString const &directoryName, NewType const &type);
                        NewType initListener(QString const &name, QString const &className, QString const &fileName);
                        void createDiagramAttributes(QDomElement const &diagram, NewType const &diagramId);
                        void createNonGraphicElements(QDomElement const &type, NewType const &diagramId);
                        void createGraphicElements(QDomElement const &type, NewType const &diagramId);
                        void initEnum(QDomElement const &enumElement, NewType const &diagramId);
                        void initNode(QDomElement const &node, NewType const &diagramId);
                        void initEdge(QDomElement const &edge, NewType const &diagramId);
                        void initImport(QDomElement const &import, NewType const &diagramId);
                        void setStandartConfigurations(NewType const &type, NewType const &parent, QString const &name,
					QString const &displayedName);
                        void setEnumAttributes(QDomElement const &enumElement, NewType const &enumId);
                        void setNodeAttributes(QDomElement const &node, NewType const &nodeId);
                        void setEdgeAttributes(QDomElement const &edge, NewType const &edgeId);
                        void setNodeConfigurations(QDomElement const &tag, NewType const &nodeId);
                        void setEdgeConfigurations(QDomElement const &tag, NewType const &edgeId);
                        void setGeneralization(QDomElement const &element, NewType const &elementId);
                        void setContainers(QDomElement const &element, NewType const &elementId);
                        void setContainerProperties(QDomElement const &element, NewType const &elementId);
                        void setBoolValuesForContainer(QString const &tagName, QDomElement const &property, NewType const &type);
                        void setSizesForContainer(QString const &tagName, QDomElement const &property, NewType const &type);
                        void setProperties(QDomElement const &element, NewType const &elementId);
                        void setConnections(QDomElement const &element, NewType const &elementId);
                        void setAssociations(QDomElement const &element, NewType const &elementId);
                        void setUsages(QDomElement const &element, NewType const &elementId);
                        void setPossibleEdges(QDomElement const &element, NewType const &elementId);
                        void setFields(QDomElement const &element, NewType const &elementId);
                        void setPin(NewType const &elementId);
                        void setAction(NewType const &elementId);
                        void setLineType(QDomElement const &tag, NewType const &edgeId);
                        void initPossibleEdge(QDomElement const &possibleEdge, NewType const &elementId);
                        void initDiagram(QDomElement const &diagram, NewType const &parent,
					QString const &name, QString const &displayedName);
                        void initProperty(QDomElement const &property, NewType const &elementId);
                        void initConnection(QDomElement const &connection, NewType const &elementId);
                        void initUsage(QDomElement const &usage, NewType const &elementId);
			void initGeneralization(QString const &diagramName);
                        void setParents(NewType const &type, QString const &diagramName);
                        void initInheritance(NewType const &idFrom, NewType const &idTo);
			void initContainer(QString const &diagramName);
                        void setContains(NewType const &type, QString const &diagramName);
                        void initContains(NewType const &idFrom, NewType const &idTo);
                        NewType getParentId(QString const &elementName);

                        void setChildrenPositions(NewType const &type, unsigned cellWidth, unsigned cellHeight);
                        void setElementPosition(NewType const &type);
			void checkIndex();
                        void manageParents(TypeList const &parents);

			bool containsName(QString const &name);
			void clear();
		};
	}
}

