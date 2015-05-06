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

#include <QtCore/QHash>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../../qrkernel/ids.h"
#include "../../../qrrepo/logicalRepoApi.h"

namespace metaEditor {

/// Parses existing .xml file with metamodel
class XmlParser
{
public:
	explicit XmlParser(qrRepo::LogicalRepoApi &api);

	void parseFile(QString const &fileName);
	void loadIncludeList(QString const &fileName);

private:
	QStringList getIncludeList(QString const &fileName);
	qReal::Id getPackageId();
	void initMetamodel(QDomDocument const &document, QString const &directoryName
			, QString const &baseName, QString const &pathToRoot, qReal::Id const &id);
	qReal::Id initListener(QString const &name, QString const &className, QString const &fileName);
	void createDiagramAttributes(QDomElement const &diagram, qReal::Id const &diagramId);
	void createNonGraphicElements(QDomElement const &type, qReal::Id const &diagramId);
	void createGraphicElements(QDomElement const &type, qReal::Id const &diagramId);
	void initEnum(QDomElement const &enumElement, qReal::Id const &diagramId);
	void initNode(QDomElement const &node, qReal::Id const &diagramId);
	void initEdge(QDomElement const &edge, qReal::Id const &diagramId);
	void initImport(QDomElement const &import, qReal::Id const &diagramId);
	void setStandartConfigurations(qReal::Id const &id, qReal::Id const &parent, QString const &name,
			QString const &displayedName);
	void setEnumAttributes(QDomElement const &enumElement, qReal::Id const &enumId);
	void setNodeAttributes(QDomElement const &node, qReal::Id const &nodeId);
	void setEdgeAttributes(QDomElement const &edge, qReal::Id const &edgeId);
	void setNodeConfigurations(QDomElement const &tag, qReal::Id const &nodeId);
	void setEdgeConfigurations(QDomElement const &tag, qReal::Id const &edgeId);
	void setGeneralization(QDomElement const &element, qReal::Id const &elementId);
	void setContainers(QDomElement const &element, qReal::Id const &elementId);
	void setContainerProperties(QDomElement const &element, qReal::Id const &elementId);
	void setBoolValuesForContainer(QString const &tagName, QDomElement const &property, qReal::Id const &id);
	void setSizesForContainer(QString const &tagName, QDomElement const &property, qReal::Id const &id);
	void setProperties(QDomElement const &element, qReal::Id const &elementId);
	void setConnections(QDomElement const &element, qReal::Id const &elementId);
	void setAssociations(QDomElement const &element, qReal::Id const &elementId);
	void setUsages(QDomElement const &element, qReal::Id const &elementId);
	void setPossibleEdges(QDomElement const &element, qReal::Id const &elementId);
	void setFields(QDomElement const &element, qReal::Id const &elementId);
	void setAction(qReal::Id const &elementId);
	void setLineType(QDomElement const &tag, qReal::Id const &edgeId);
	void initPossibleEdge(QDomElement const &possibleEdge, qReal::Id const &elementId);
	void initDiagram(QDomElement const &diagram, qReal::Id const &parent,
			QString const &name, QString const &displayedName);
	void initProperty(QDomElement const &property, qReal::Id const &elementId);
	void initConnection(QDomElement const &connection, qReal::Id const &elementId);
	void initUsage(QDomElement const &usage, qReal::Id const &elementId);
	void initGeneralization(QString const &diagramName);
	void setParents(qReal::Id const &id, QString const &diagramName);
	void initInheritance(qReal::Id const &idFrom, qReal::Id const &idTo);
	void initContainer(QString const &diagramName);
	void setContains(qReal::Id const &id, QString const &diagramName);
	void initContains(qReal::Id const &idFrom, qReal::Id const &idTo);
	qReal::Id getParentId(QString const &elementName);

	void setChildrenPositions(qReal::Id const &id, unsigned cellWidth, unsigned cellHeight);
	void setElementPosition(qReal::Id const &id);
	void checkIndex();
	void manageParents(qReal::IdList const &parents);

	bool containsName(QString const &name);
	void clear();

	qrRepo::LogicalRepoApi &mApi;
	qReal::Id mMetamodel;
	QHash<QString, qReal::Id> mElements;
	QHash<qReal::Id, QStringList> mParents;
	QHash<qReal::Id, QStringList> mContainers;
	int mElementsColumn;
	int mElementCurrentColumn;
	int mMoveWidth;
	int mMoveHeight;
	int mCurrentWidth;
	int mCurrentHeight;
	int mParentPositionX;
};

}
