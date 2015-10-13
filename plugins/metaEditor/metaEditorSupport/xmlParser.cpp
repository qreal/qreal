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

#include "xmlParser.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtCore/QPointF>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPolygonF>
#include <QtXml/QDomDocument>

#include "math.h"

#include <qrrepo/repoApi.h>
#include <qrutils/xmlUtils.h>
#include <qrkernel/exception/exception.h>

using namespace qReal;
using namespace metaEditor;

XmlParser::XmlParser(qrRepo::LogicalRepoApi &api)
		: mApi(api)
		, mElementsColumn(0)
		, mElementCurrentColumn(0)
		, mMoveWidth(180)
		, mMoveHeight(100)
		, mCurrentWidth(0)
		, mCurrentHeight(0)
		, mParentPositionX(280)
{
}

void XmlParser::parseFile(const QString &fileName)
{
	QFileInfo directoryName(fileName);

	// search for the directory, where xml-file is
	QDir fileDir = directoryName.absoluteDir();
	QString filePathName = fileDir.absolutePath();
	QString fileBaseName = directoryName.baseName();

	QString fileProFullName = filePathName + "/" + fileBaseName + ".pro";
	QFile file(fileProFullName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString pathToQRealSourceFiles;
	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.contains("ROOT")) {
			pathToQRealSourceFiles = line.mid(line.indexOf("=") + 1);
			pathToQRealSourceFiles = pathToQRealSourceFiles.trimmed();
		}
	}
	file.close();

	if (containsName(fileBaseName))
		return;
	QDomDocument const doc = utils::xmlUtils::loadDocument(fileName);

	Id const packageId = getPackageId();
	initMetamodel(doc, filePathName, fileBaseName, pathToQRealSourceFiles, packageId);

	QDomNodeList const listeners = doc.elementsByTagName("listener");
	int listenerPositionY = 100;
	for (int i = 0; i < listeners.length(); ++i) {
		QDomElement listener = listeners.at(i).toElement();
		Id id = initListener("(Listener)", listener.attribute("class", ""), listener.attribute("file", ""));
		mApi.setProperty(id, "position", QPointF(0,listenerPositionY));
		listenerPositionY += 90;
	}

	QDomNodeList const diagrams = doc.elementsByTagName("diagram");

	mElementsColumn = ceil(sqrt(static_cast<qreal>(diagrams.length())));

	for (int i = 0; i < diagrams.length(); ++i) {
		QDomElement diagram = diagrams.at(i).toElement();
		initDiagram(diagram, mMetamodel, diagram.attribute("name", "Unknown Diagram"),
				diagram.attribute("displayedName", "Unknown Diagram"));
	}
	clear();
}

void XmlParser::clear()
{
	mElementsColumn = 0;
	mElementCurrentColumn = 0;
	mMoveWidth = 180;
	mMoveHeight = 100;
	mCurrentWidth = 0;
	mCurrentHeight = 0;
	mParentPositionX = 280;
	mElements.clear();
	mParents.clear();
	mContainers.clear();
}

QStringList XmlParser::getIncludeList(const QString &fileName)
{
	QDomDocument const doc = utils::xmlUtils::loadDocument(fileName);

	QDomNodeList const includeList = doc.elementsByTagName("include");
	QStringList includeFilesList;
	for (int i = 0; i < includeList.length(); ++i) {
		QDomElement include = includeList.at(i).toElement();
		QFileInfo info(fileName);
		QFileInfo name(include.text());
		if (!containsName(name.baseName())) {
			includeFilesList.append(getIncludeList(info.absoluteDir().path() + "/" + include.text()));
			includeFilesList.append(info.absoluteDir().path() + "/" + include.text());
		}
	}
	return includeFilesList;
}

void XmlParser::loadIncludeList(const QString &fileName)
{
	QStringList includeList = getIncludeList(fileName);
	if (includeList.isEmpty())
		return;
	if (QMessageBox::question(nullptr, QObject::tr("loading.."),"Do you want to load connected metamodels?"
			, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
	{
		foreach (QString const &include, includeList) {
			if (!containsName(include))
				parseFile(include);
		}
	}
}

bool XmlParser::containsName(const QString &name)
{
	IdList idList = mApi.children(Id::rootId());
	foreach (Id const &id, idList) {
		if (mApi.name(id) == name)
			return true;
	}

	return false;
}

Id XmlParser::getPackageId()
{
	IdList const children = mApi.children(Id::rootId());
	foreach (Id id, children) {
		if (id.element() == "PackageDiagram")
			return id;
	}
	Id const packageId("MetaEditor", "MetaEditor", "PackageDiagram",
					   QUuid::createUuid().toString());
	setStandartConfigurations(packageId, Id::rootId(), "Package", "Package");
	return packageId;
}

void XmlParser::initMetamodel(const QDomDocument &document, const QString &directoryName, const QString &baseName
		, const QString &pathToRoot, const Id &id)
{
	QString fileBaseName = baseName;

	Id metamodelId = Id("MetaEditor", "MetaEditor", "MetamodelDiagram",
			QUuid::createUuid().toString());

	QDomNodeList const includeList = document.elementsByTagName("include");
	QString includeListString = "";

	for (int i = 0; i < includeList.length(); ++i) {
		QDomElement include = includeList.at(i).toElement();
		includeListString += include.text() + ", ";
	}
	setStandartConfigurations(metamodelId, id, "Empty_" + fileBaseName, "");
	mApi.setProperty(metamodelId, "version", document.documentElement().attribute("version"));
	mApi.setProperty(metamodelId, "include", includeListString);
	mApi.setProperty(metamodelId, "name of the directory", directoryName);
	mApi.setProperty(metamodelId, "relative path to QReal Source Files", pathToRoot);

	mMetamodel = Id("MetaEditor", "MetaEditor", "MetamodelDiagram",
					QUuid::createUuid().toString());
	setStandartConfigurations(mMetamodel, Id::rootId(), fileBaseName, "");
	mApi.setProperty(mMetamodel, "include", includeListString);
	mApi.setProperty(mMetamodel, "name of the directory", directoryName);
	mApi.setProperty(mMetamodel, "relative path to QReal Source Files", pathToRoot);

	mApi.addExplosion(metamodelId, mMetamodel);
}

Id XmlParser::initListener(const QString &name, const QString &className, const QString &fileName)
{
	Id listenerId("MetaEditor", "MetaEditor", "Listener",
			QUuid::createUuid().toString());
	setStandartConfigurations(listenerId, mMetamodel, name, name);
	mApi.setProperty(listenerId, "class", className);
	mApi.setProperty(listenerId, "file", fileName);
	return listenerId;
}

void XmlParser::initDiagram(const QDomElement &diagram, const Id &parent,
		const QString &name, const QString &displayedName)
{
	Id diagramId("MetaEditor", "MetaEditor", "MetaEditorDiagramNode",
			QUuid::createUuid().toString());

	setStandartConfigurations(diagramId, parent, name, displayedName);
	mApi.setProperty(diagramId, "nodeName", diagram.attribute("nodeName", ""));

	createDiagramAttributes(diagram, diagramId);

	setElementPosition(diagramId);
}

void XmlParser::createDiagramAttributes(const QDomElement &diagram, const Id &diagramId)
{
	QDomNodeList diagramAttributes = diagram.childNodes();

	for (int i = 0; i < diagramAttributes.length(); ++i) {
		QDomElement type = diagramAttributes.at(i).toElement();
		if (type.tagName() == "nonGraphicTypes")
			createNonGraphicElements(type, diagramId);
		if (type.tagName() == "graphicTypes")
			createGraphicElements(type, diagramId);
	}
	QString const diagramName = mApi.name(diagramId);
	initGeneralization(diagramName);
	initContainer(diagramName);
}

void XmlParser::createNonGraphicElements(const QDomElement &type, const Id &diagramId)
{
	QDomNodeList enums = type.childNodes();

	for (int i = 0; i < enums.length(); ++i) {
		QDomElement enumElement = enums.at(i).toElement();
		if (enumElement.tagName() == "enum")
			initEnum(enumElement, diagramId);
	}
}

void XmlParser::createGraphicElements(const QDomElement &type, const Id &diagramId)
{
	QDomNodeList graphicElements = type.childNodes();

	for (int i = 0; i < graphicElements.length(); ++i) {
		QDomElement graphicElement = graphicElements.at(i).toElement();
		if (graphicElement.tagName() == "node")
			initNode(graphicElement, diagramId);
		if (graphicElement.tagName() == "edge")
			initEdge(graphicElement, diagramId);
		if (graphicElement.tagName() == "import")
			initImport(graphicElement, diagramId);
	}
}

void XmlParser::initEnum(const QDomElement &enumElement, const Id &diagramId)
{
	Id enumId("MetaEditor", "MetaEditor", "MetaEntityEnum",
			QUuid::createUuid().toString());

	setStandartConfigurations(enumId, diagramId, enumElement.attribute("name", ""),
			enumElement.attribute("displayedName", ""));

	setEnumAttributes(enumElement, enumId);
}

void XmlParser::initNode(const QDomElement &node, const Id &diagramId)
{
	QString const nodeName = node.attribute("name", "");

	if (!(mElements.contains(nodeName))) {
		Id nodeId("MetaEditor", "MetaEditor", "MetaEntityNode",
				QUuid::createUuid().toString());
		mElements.insert(nodeName, nodeId);

		setStandartConfigurations(nodeId, diagramId, nodeName,
				node.attribute("displayedName", ""));
		mApi.setProperty(nodeId, "path", node.attribute("path", ""));
		mApi.setProperty(nodeId, "description", node.attribute("description", ""));

		setNodeAttributes(node, nodeId);
	}
}

void XmlParser::initEdge(const QDomElement &edge, const Id &diagramId)
{
	QString const edgeName = edge.attribute("name", "");

	if (!(mElements.contains(edgeName))) {
		Id edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge",
				QUuid::createUuid().toString());
		mElements.insert(edgeName, edgeId);

		setStandartConfigurations(edgeId, diagramId, edgeName,
				edge.attribute("displayedName", ""));

		setEdgeAttributes(edge, edgeId);
	}
}

void XmlParser::initImport(const QDomElement &import, const Id &diagramId)
{
	QString const importName = import.attribute("name", "");

	if (!(mElements.contains(importName))) {
		Id importId("MetaEditor", "MetaEditor", "MetaEntityImport",
				QUuid::createUuid().toString());
		QStringList nameList = import.attribute("name", "").split("::", QString::SkipEmptyParts);
		setStandartConfigurations(importId, diagramId, nameList.at(1),
				import.attribute("displayedName", ""));
		mApi.setProperty(importId, "as", import.attribute("as", ""));
		mApi.setProperty(importId, "importedFrom", nameList.at(0));
		mElements.insert(importName, importId);
	}
}

void XmlParser::setEnumAttributes(QDomElement const &enumElement, Id const &enumId)
{
	QDomNodeList const values = enumElement.childNodes();

	for (int i = 0; i < values.length(); ++i) {
		QDomElement value = values.at(i).toElement();
		if (value.tagName() == "value"){
			Id const valueId("MetaEditor", "MetaEditor", "MetaEntityValue"
					, QUuid::createUuid().toString());

			setStandartConfigurations(valueId, enumId, value.text(),
					value.attribute("displayedName", ""));

			mApi.setProperty(valueId, "valueName", value.attribute("name", ""));
		}
	}
}

void XmlParser::setNodeAttributes(const QDomElement &node, const Id &nodeId)
{
	QDomNodeList nodeList = node.childNodes();

	for (int i = 0; i < nodeList.length(); ++i) {
		QDomElement tag = nodeList.at(i).toElement();
		if (tag.tagName() == "logic")
			setNodeConfigurations(tag, nodeId);
		if (tag.tagName() == "graphics") {
			QDomDocument document;
			document.createElement("document");
			QDomNode nodeCopy = nodeList.at(i).cloneNode();
			document.importNode(nodeList.at(i), true);
			document.appendChild(nodeCopy);
			mApi.setProperty(nodeId, "shape", document.toString());
		}
	}
}

void XmlParser::setEdgeAttributes(const QDomElement &edge, const Id &edgeId)
{
	QDomNodeList edgeList = edge.childNodes();

	for (int i = 0; i < edgeList.length(); ++i) {
		QDomElement tag = edgeList.at(i).toElement();
		if (tag.tagName() == "graphics")
			setLineType(tag, edgeId);
		if (tag.tagName() == "logic")
			setEdgeConfigurations(tag, edgeId);
	}
}

void XmlParser::setNodeConfigurations(const QDomElement &tag, const Id &nodeId)
{
	QDomNodeList nodeAttributes = tag.childNodes();

	for (int i = 0; i < nodeAttributes.length(); ++i) {
		QDomElement attribute = nodeAttributes.at(i).toElement();
		if (attribute.tagName() == "generalizations")
			setGeneralization(attribute, nodeId);
		else if (attribute.tagName() == "properties")
			setProperties(attribute, nodeId);
		else if (attribute.tagName() == "container")
			setContainers(attribute, nodeId);
		else if (attribute.tagName() == "connections")
			setConnections(attribute, nodeId);
		else if (attribute.tagName() == "usages")
			setUsages(attribute, nodeId);
		else if (attribute.tagName() == "action")
			setAction(nodeId);
		else if (attribute.tagName() == "bonusContextMenuFields")
			setFields(attribute, nodeId);
	}
}

void XmlParser::setLineType(const QDomElement &tag, const Id &edgeId)
{
	QDomNodeList graphics = tag.childNodes();

	if (graphics.length() > 0) {
		QDomElement lineType = graphics.at(0).toElement();
		mApi.setProperty(edgeId, "lineType", lineType.attribute("type", ""));
	}
	// quick workaround for #349, just saving a part of XML into `labels' property
	// TODO: make it somehow more elegant
	for(int i = 0; i < graphics.length(); ++i){
		QDomElement element = graphics.at(i).toElement();
		if (element.tagName() == "labels"){
			QString labels;
			QTextStream out(&labels);
			element.save(out, 4);
			mApi.setProperty(edgeId, "labels", labels);
		}
	}
}

void XmlParser::setEdgeConfigurations(const QDomElement &tag, const Id &edgeId)
{
	QDomNodeList edgeAttributes = tag.childNodes();

	for (int i = 0; i < edgeAttributes.length(); ++i) {
		QDomElement attribute = edgeAttributes.at(i).toElement();
		if (attribute.tagName() == "generalizations")
			setGeneralization(attribute, edgeId);
		else if (attribute.tagName() == "properties")
			setProperties(attribute, edgeId);
		else if (attribute.tagName() == "associations")
			setAssociations(attribute, edgeId);
		else if (attribute.tagName() == "possibleEdges")
			setPossibleEdges(attribute, edgeId);
	}
}

void XmlParser::setGeneralization(const QDomElement &element, const Id &elementId)
{
	QDomNodeList generalizations = element.childNodes();
	QStringList parents;

	for (int i = 0; i < generalizations.length(); ++i) {
		QDomElement generalization = generalizations.at(i).toElement();
		if (generalization.tagName() == "parent")
			parents.insert(0, generalization.attribute("parentName", ""));
		mParents.insert(elementId, parents);
	}
}

void XmlParser::setProperties(const QDomElement &element, const Id &elementId)
{
	QDomNodeList properties = element.childNodes();

	for (int i = 0; i < properties.length(); ++i) {
		QDomElement property = properties.at(i).toElement();
		if (property.tagName() == "property")
			initProperty(property, elementId);
	}
}

void XmlParser::setFields(const QDomElement &element, const Id &elementId)
{
	QDomNodeList fields = element.childNodes();

	for (int i = 0; i < fields.length(); ++i) {
		QDomElement field = fields.at(i).toElement();
		if (field.tagName() == "field") {
			Id fieldId("MetaEditor", "MetaEditor", "MetaEntityContextMenuField", QUuid::createUuid().toString());
			setStandartConfigurations(fieldId, elementId, field.attribute("name", ""),
					field.attribute("displayedName", ""));
		}
	}
}

void XmlParser::setContainers(const QDomElement &element, const Id &elementId)
{
	QDomNodeList containsElements = element.childNodes();
	QStringList containers;
	for (int i = 0; i < containsElements.length(); ++i) {
		QDomElement contains = containsElements.at(i).toElement();
		if (contains.tagName() == "contains") {
			QString type = contains.attribute("type", "");
			containers.insert(0, type);
			/*QString existingContainers;
				if (mApi.hasProperty(elementId, "container"))
				existingContainers = mApi.stringProperty(elementId, "container");
				if (!existingContainers.isEmpty())
					existingContainers += ",";
				existingContainers += type;

				mApi.setProperty(elementId, "container", existingContainers);*/
		}
		if (contains.tagName() == "properties")
			setContainerProperties(contains, elementId);
	}
	mContainers.insert(elementId, containers);
}

void XmlParser::setContainerProperties(const QDomElement &element, const Id &elementId)
{
	QDomNodeList properties = element.childNodes();
	if (properties.size() > 0) {
		Id containerProperties("MetaEditor", "MetaEditor",
				"MetaEntityPropertiesAsContainer", QUuid::createUuid().toString());
		setStandartConfigurations(containerProperties, elementId,
				"properties", "");
		for (int i = 0; i < properties.length(); ++i) {
			QDomElement property = properties.at(i).toElement();
			setBoolValuesForContainer("sortContainer", property, containerProperties);
			setBoolValuesForContainer("minimizeToChildren", property, containerProperties);
			setBoolValuesForContainer("maximizeChildren", property, containerProperties);
			setBoolValuesForContainer("banChildrenMove", property, containerProperties);

			setSizesForContainer("forestalling", property, containerProperties);
			setSizesForContainer("childrenForestalling", property, containerProperties);
		}
	}
}

void XmlParser::setBoolValuesForContainer(const QString &tagName, const QDomElement &property, const Id &id)
{
	if (property.tagName() == tagName)
		mApi.setProperty(id, tagName, "true");
}

void XmlParser::setSizesForContainer(const QString &tagName, const QDomElement &property, const Id &id)
{
	if (property.tagName() == tagName)
		mApi.setProperty(id, tagName + "Size", property.attribute("size", "0"));
}

void XmlParser::setConnections(const QDomElement &element, const Id &elementId)
{
	QDomNodeList connections = element.childNodes();

	for (int i = 0; i < connections.length(); ++i) {
		QDomElement connection = connections.at(i).toElement();
		if (connection.tagName() == "connection")
			initConnection(connection, elementId);
	}
}

void XmlParser::setUsages(const QDomElement &element, const Id &elementId)
{
	QDomNodeList usages = element.childNodes();

	for (int i = 0; i < usages.length(); ++i) {
		QDomElement usage = usages.at(i).toElement();
		if (usage.tagName() == "usage")
			initUsage(usage, elementId);
	}
}

void XmlParser::setAssociations(const QDomElement &element, const Id &elementId)
{
	Id associationId("MetaEditor", "MetaEditor", "MetaEntityAssociation",
			QUuid::createUuid().toString());
	QDomNodeList associations = element.childNodes();

	QDomElement association = associations.at(0).toElement();

	setStandartConfigurations(associationId, elementId, association.attribute("name", ""),
			association.attribute("displayedName", ""));

	mApi.setProperty(associationId, "beginType", element.attribute("beginType", ""));
	mApi.setProperty(associationId, "endType", element.attribute("endType", ""));
	mApi.setProperty(associationId, "beginName", association.attribute("beginName", ""));
	mApi.setProperty(associationId, "endName", association.attribute("endName", ""));
}

void XmlParser::setPossibleEdges(const QDomElement &element, const Id &elementId)
{
	QDomNodeList possibleEdges = element.childNodes();

	for (int i = 0; i < possibleEdges.length(); ++i) {
		QDomElement possibleEdge = possibleEdges.at(i).toElement();
		if (possibleEdge.tagName() == "possibleEdge")
			initPossibleEdge(possibleEdge, elementId);
	}
}

void XmlParser::setAction(const Id &elementId)
{
	mApi.setProperty(elementId, "isAction", "true");
}

void XmlParser::initPossibleEdge(const QDomElement &possibleEdge, const Id &elementId)
{
	Id possibleEdgeId("MetaEditor", "MetaEditor", "MetaEntityPossibleEdge",
			QUuid::createUuid().toString());

	setStandartConfigurations(possibleEdgeId, elementId, possibleEdge.attribute("name", ""),
			possibleEdge.attribute("displayedName", ""));

	mApi.setProperty(possibleEdgeId, "beginName", possibleEdge.attribute("beginName", ""));
	mApi.setProperty(possibleEdgeId, "endName", possibleEdge.attribute("endName", ""));
	mApi.setProperty(possibleEdgeId, "directed", possibleEdge.attribute("directed", "false"));
}

void XmlParser::initProperty(const QDomElement &property, const Id &elementId)
{
	Id propertyId("MetaEditor", "MetaEditor", "MetaEntity_Attribute",
			QUuid::createUuid().toString());

	setStandartConfigurations(propertyId, elementId, property.attribute("name", ""),
			property.attribute("displayedName", ""));

	mApi.setProperty(propertyId, "type", property.attribute("type", ""));
	mApi.setProperty(propertyId, "attributeType", property.attribute("type", "0"));

	QDomNodeList defaultValue = property.childNodes();
	if (!defaultValue.isEmpty())
		mApi.setProperty(propertyId, "defaultValue",
				defaultValue.at(0).toElement().text());
}

void XmlParser::initConnection(const QDomElement &connection, const Id &elementId)
{
	Id connectionId("MetaEditor", "MetaEditor", "MetaEntityConnection",
			QUuid::createUuid().toString());

	setStandartConfigurations(connectionId, elementId, connection.attribute("name", ""),
			connection.attribute("displayedName", ""));

	mApi.setProperty(connectionId, "type", connection.attribute("type", ""));
}

void XmlParser::initUsage(const QDomElement &usage, const Id &elementId)
{
	Id usageId("MetaEditor", "MetaEditor", "MetaEntityUsage",
			QUuid::createUuid().toString());

	setStandartConfigurations(usageId, elementId, usage.attribute("name", ""),
			usage.attribute("displayedName", ""));

	mApi.setProperty(usageId, "type", usage.attribute("type", ""));
}

void XmlParser::initGeneralization(const QString &diagramName)
{
	foreach (Id const id, mParents.keys()) {
		setParents(id, diagramName);
	}
}

void XmlParser::initContainer(const QString &diagramName)
{
	foreach (Id const id, mContainers.keys()) {
		setContains(id, diagramName);
	}
}

void XmlParser::setParents(const Id &id, const QString &diagramName)
{
	IdList parents;
	foreach (QString const elementName, mParents[id]) {
		QStringList name = elementName.split("::");
		QString baseElementName;
		if (name.size() < 2)
			baseElementName = elementName;
		else
			baseElementName = (name[0] == diagramName) ? name[1] : elementName;
		if ((mElements.contains(baseElementName))) {
			initInheritance(mElements[baseElementName], id);
		}
		else {
			Id const parentId = getParentId(baseElementName);
			parents.append(parentId);
			initInheritance(parentId, id);
			mElements.insert(baseElementName, parentId);
		}
	}
	if (!parents.isEmpty())
		manageParents(parents);
}

void XmlParser::initInheritance(const Id &idFrom, const Id &idTo)
{
	Id inheritanceId("MetaEditor", "MetaEditor", "Inheritance",
			QUuid::createUuid().toString());

	QString const name = mApi.name(idTo) + "_Inherits_" + mApi.name(idFrom);

	setStandartConfigurations(inheritanceId, mMetamodel, name, name);
	mApi.setFrom(inheritanceId, idFrom);
	mApi.setTo(inheritanceId, idTo);
}

Id XmlParser::getParentId(const QString &elementName)
{
	Id parentId("MetaEditor", "MetaEditor", "MetaEntityImport",
			QUuid::createUuid().toString());

	setStandartConfigurations(parentId, mMetamodel, elementName, elementName);
	return parentId;
}

void XmlParser::setContains(const Id &id, const QString &diagramName)
{
	foreach (QString const elementName, mContainers[id]) {
		QStringList name = elementName.split("::");
		QString baseElementName;
		if (name.size() < 2)
			baseElementName = elementName;
		else
			baseElementName = (name[0] == diagramName) ? name[1] : elementName;
		if ((mElements.contains(baseElementName))) {
			initContains(id, mElements[baseElementName]);
		}
	}
}

void XmlParser::initContains(const Id &idFrom, const Id &idTo)
{
	Id containerId("MetaEditor", "MetaEditor", "Container",
			QUuid::createUuid().toString());

	QString const name = mApi.name(idFrom) + "_Contains_" + mApi.name(idTo);

	setStandartConfigurations(containerId, mMetamodel, name, name);
	mApi.setFrom(containerId, idFrom);
	mApi.setTo(containerId, idTo);
}

void XmlParser::manageParents(const IdList &parents)
{
	foreach (Id const id, parents) {
		mApi.setProperty(id, "position", QPointF(mParentPositionX, 0));
		mParentPositionX += 120;
	}
}

void XmlParser::setStandartConfigurations(Id const &id, Id const &parent,
		const QString &name, const QString &displayedName)
{
	mApi.addChild(parent, id);
	mApi.setProperty(id, "name", name);
	if (displayedName != "")
		mApi.setProperty(id, "displayedName", displayedName);
	mApi.setFrom(id, Id::rootId());
	mApi.setTo(id, Id::rootId());
	mApi.setProperty(id, "fromPort", 0.0);
	mApi.setProperty(id, "toPort", 0.0);
	mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));

	mApi.setProperty(id, "position", QPointF(0,0));
	mApi.setProperty(id, "configuration", QVariant(QPolygon()));
}

void XmlParser::setChildrenPositions(const Id &id, unsigned cellWidth, unsigned cellHeight)
{
	int rowWidth = ceil(sqrt(static_cast<qreal>(mApi.children(id).count())));
	int currentRow = 0;
	int currentColumn = 0;
	int sizeyElement = 100;
	int sizeyElements = 0;

	foreach(Id element, mApi.children(id)) {
		mApi.setProperty(element, "position", QPointF(currentColumn * (cellWidth + 40) + 50, sizeyElement));
		if (mApi.children(element).isEmpty())
			sizeyElement += 180;
		else
			sizeyElement += cellHeight * mApi.children(element).length() + 80;
		++currentRow;
		if (currentRow >= rowWidth) {
			currentRow = 0;
			++currentColumn;
			if (sizeyElement > sizeyElements)
				sizeyElements = sizeyElement;
			sizeyElement = 100;
		}
	}
	mCurrentWidth = rowWidth * cellWidth + 30;
	mCurrentHeight = sizeyElements;
}

void XmlParser::checkIndex()
{
	++mElementCurrentColumn;
	if (mElementCurrentColumn >= mElementsColumn) {
		mElementCurrentColumn = 0;
		mMoveHeight = 0;
		mMoveWidth += mCurrentWidth;
	}
}

void XmlParser::setElementPosition(const Id &id)
{
	mMoveHeight += mCurrentHeight;
	mApi.setProperty(id, "position", QPointF(mMoveWidth, mMoveHeight));
	setChildrenPositions(id, 160, 50);
	QRect const value = QRect(mMoveWidth, mMoveHeight, mCurrentWidth, mCurrentHeight);
	mApi.setProperty(id, "configuration", QVariant(QPolygon(value, false)));
	checkIndex();
}
