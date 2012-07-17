#pragma once

#include <QtCore/QHash>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../../qrkernel/ids.h"
#include "../../../qrrepo/logicalRepoApi.h"
#include "../../../pluginManager/editorManagerInterface.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorManagerInterface;

	namespace parsers {

		class XmlParser
		{
		public:
			explicit XmlParser(qrRepo::LogicalRepoApi &api, EditorManagerInterface const *editorManagerInter);

			void parseFile(QString const &fileName);
			void loadIncludeList(QString const &fileName);

		private:
			qrRepo::LogicalRepoApi &mApi;
			EditorManagerInterface const *mEditorManagerInter;
			Id mMetamodel;
			QHash<QString, Id> mElements;
			QHash<Id, QStringList> mParents;
			QHash<Id, QStringList> mContainers;
			int mElementsColumn;
			int mElementCurrentColumn;
			int mMoveWidth;
			int mMoveHeight;
			int mCurrentWidth;
			int mCurrentHeight;
			int mParentPositionX;

			QStringList getIncludeList(QString const &fileName);
			Id getPackageId();
			void initMetamodel(QDomDocument const &document, QString const &directoryName, QString const &baseName, QString const &pathToRoot, Id const &id);
			Id initListener(QString const &name, QString const &className, QString const &fileName);
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
			void initGeneralization(QString const &diagramName);
			void setParents(Id const &id, QString const &diagramName);
			void initInheritance(Id const &idFrom, Id const &idTo);
			void initContainer(QString const &diagramName);
			void setContains(Id const &id, QString const &diagramName);
			void initContains(Id const &idFrom, Id const &idTo);
			Id getParentId(QString const &elementName);

			void setChildrenPositions(Id const &id, unsigned cellWidth, unsigned cellHeight);
			void setElementPosition(Id const &id);
			void checkIndex();
			void manageParents(IdList const &parents);

			bool containsName(QString const &name);
			void clear();
		};
	}
}
