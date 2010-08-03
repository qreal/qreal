#include "xmlParser.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtXml/QDomDocument>
#include <QtCore/QProcess>
#include <QMessageBox>
#include <QPointF>
#include <QPolygonF>

#include "math.h"

#include "../../../qrrepo/repoApi.h"
#include "../../../utils/xmlUtils.h"
#include "../../editorManager/editorManager.h"
#include "../../kernel/exception/exception.h"

using namespace qReal;
using namespace parsers;

XmlParser::XmlParser(qrRepo::RepoApi &api, EditorManager const &editorManager)
	: mApi(api), mEditorManager(editorManager), mElementsColumn(0), mElementCurrentColumn(0),
	mMoveWidth(10), mMoveHeight(10), mCurrentWidth(0), mCurrentHeight(0)
{
}

void XmlParser::parseFile(const QString &fileName)
{
	QDomDocument const doc = utils::xmlUtils::loadDocument(fileName);

	Id const packageId = getPackageId();
	initMetamodel(doc, fileName, packageId);

	QDomNodeList const diagrams = doc.elementsByTagName("diagram");

	mElementsColumn =  ceil(sqrt(static_cast<qreal>(diagrams.length())));

	for (unsigned i = 0; i < diagrams.length(); ++i) {
		QDomElement diagram = diagrams.at(i).toElement();
		initDiagram(diagram, mMetamodel, diagram.attribute("name", "Unknown Diagram"),
				diagram.attribute("displayedName", "Unknown Diagram"));
	}
}

QStringList XmlParser::getIncludeList(const QString &fileName)
{
	QDomDocument const doc = utils::xmlUtils::loadDocument(fileName);

	QDomNodeList const includeList = doc.elementsByTagName("include");
	QStringList includeFilesList;
	for (unsigned i = 0; i < includeList.length(); ++i) {
		QDomElement include = includeList.at(i).toElement();
		QFileInfo info(fileName);
		if (!mIncludeList.contains(info.baseName()))
			mIncludeList.append(info.baseName());
		QFileInfo name(include.text());
		if (!mIncludeList.contains(name.baseName())) {
			includeFilesList.append(getIncludeList(info.absoluteDir().path() + "/" + include.text()));
			includeFilesList.append(info.absoluteDir().path() + "/" + include.text());
			mIncludeList.append(name.baseName());
		}
	}
	return includeFilesList;
}

void XmlParser::loadIncludeList(const QString &fileName)
{
	QStringList includeList = getIncludeList(fileName);
	if (includeList.isEmpty())
		return;
	if (QMessageBox::question(NULL, QObject::tr("loading.."),"Do you want to load connected metamodels?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		foreach (QString const &include, includeList)
			parseFile(include);
	}
}

Id XmlParser::getPackageId()
{
	IdList const children = mApi.children(ROOT_ID);
	foreach (Id id, children) {
		if (id.element() == "PackageDiagram")
			return id;
	}
	Id const packageId("Meta_editor", "MetaEditor", "PackageDiagram",
			QUuid::createUuid().toString());
	setStandartConfigurations(packageId, ROOT_ID, "Package", "Package");
	return packageId;
}

void XmlParser::initMetamodel(const QDomDocument &document, const QString &directoryName, const Id &id)
{
	QFileInfo fileName(directoryName);
	QString fileBaseName = fileName.baseName();

	Id metamodelId = Id("Meta_editor", "MetaEditor", "MetamodelDiagram",
			QUuid::createUuid().toString());

	QDomNodeList const includeList = document.elementsByTagName("include");
	QString includeListString = "";

	for (unsigned i = 0; i < includeList.length(); ++i) {
		QDomElement include = includeList.at(i).toElement();
		includeListString += include.text() + ", ";
	}
	setStandartConfigurations(metamodelId, id, "Empty_" + fileBaseName, "");
	mApi.setProperty(metamodelId, "include", includeListString);
	mApi.setProperty(metamodelId, "name of the directory", fileBaseName);

	mMetamodel = Id("Meta_editor", "MetaEditor", "MetamodelDiagram",
					QUuid::createUuid().toString());
	setStandartConfigurations(mMetamodel, ROOT_ID, fileBaseName, "");
	mApi.setProperty(mMetamodel, "include", includeListString);
	mApi.setProperty(mMetamodel, "name of the directory", fileBaseName);
	mApi.connect(metamodelId, mMetamodel);
}

void XmlParser::initDiagram(const QDomElement &diagram, const Id &parent,
		const QString &name, const QString &displayedName)
{
	Id diagramId("Meta_editor", "MetaEditor", "MetaEditorDiagramNode",
			QUuid::createUuid().toString());

	mDiagram = diagramId;

	setStandartConfigurations(diagramId, parent, name, displayedName);
	mApi.setProperty(diagramId, "nodeName", diagram.attribute("nodeName", ""));

	createDiagramAttributes(diagram, diagramId);

	setElementPosition(diagramId);
}

void XmlParser::createDiagramAttributes(const QDomElement &diagram, const Id &diagramId)
{
	QDomNodeList diagramAttributes = diagram.childNodes();

	for (unsigned i = 0; i < diagramAttributes.length(); ++i) {
		QDomElement type = diagramAttributes.at(i).toElement();
		if (type.tagName() == "nonGraphicTypes")
			createNonGraphicElements(type, diagramId);
		if (type.tagName() == "graphicTypes")
			createGraphicElements(type, diagramId);
	}
}

void XmlParser::createNonGraphicElements(const QDomElement &type, const Id &diagramId)
{
	QDomNodeList enums = type.childNodes();

	for (unsigned i = 0; i < enums.length(); ++i) {
		QDomElement enumElement = enums.at(i).toElement();
		if (enumElement.tagName() == "enum")
			initEnum(enumElement, diagramId);
	}
}

void XmlParser::createGraphicElements(const QDomElement &type, const Id &diagramId)
{
	QDomNodeList graphicElements = type.childNodes();

	for (unsigned i = 0; i < graphicElements.length(); ++i) {
		QDomElement graphicElement = graphicElements.at(i).toElement();
		if (graphicElement.tagName() == "node")
			initNode(graphicElement, diagramId);
		if (graphicElement.tagName() == "edge")
			initEdge(graphicElement, diagramId);
		if (graphicElement.tagName() == "import")
			initImport(graphicElement, diagramId);
	}
	initContainer();
}

void XmlParser::initEnum(const QDomElement &enumElement, const Id &diagramId)
{
	Id enumId("Meta_editor", "MetaEditor", "MetaEntityEnum",
			  QUuid::createUuid().toString());

	setStandartConfigurations(enumId, diagramId, enumElement.attribute("name", ""),
			enumElement.attribute("displayedName", ""));

	setEnumAttributes(enumElement, enumId);
}

void XmlParser::initNode(const QDomElement &node, const Id &diagramId)
{
	Id nodeId("Meta_editor", "MetaEditor", "MetaEntityNode",
			QUuid::createUuid().toString());
	mElements.insert(nodeId, node.attribute("name", ""));

	setStandartConfigurations(nodeId, diagramId, node.attribute("name", ""),
			node.attribute("displayedName", ""));
	mApi.setProperty(nodeId, "path", node.attribute("path", ""));

	setNodeAttributes(node, nodeId);
}

void XmlParser::initEdge(const QDomElement &edge, const Id &diagramId)
{
	Id edgeId("Meta_editor", "MetaEditor", "MetaEntityEdge",
			  QUuid::createUuid().toString());
	mElements.insert(edgeId, edge.attribute("name", ""));

	setStandartConfigurations(edgeId, diagramId, edge.attribute("name", ""),
			edge.attribute("displayedName", ""));

	setEdgeAttributes(edge, edgeId);
}

void XmlParser::initImport(const QDomElement &import, const Id &diagramId)
{
	Id importId("Meta_editor", "MetaEditor", "MetaEntityImport",
				QUuid::createUuid().toString());
	QStringList nameList = import.attribute("name", "").split("::", QString::SkipEmptyParts);
	setStandartConfigurations(importId, diagramId, nameList.at(1),
			import.attribute("displayedName", ""));
	mApi.setProperty(importId, "as", import.attribute("as", ""));
	mApi.setProperty(importId, "importedFrom", nameList.at(0));
	mElements.insert(importId, import.attribute("as", ""));
}

void XmlParser::setEnumAttributes(const QDomElement &enumElement, const Id &enumId)
{
	QDomNodeList values = enumElement.childNodes();

	for (unsigned i = 0; i < values.length(); ++i) {
		QDomElement value = values.at(i).toElement();
		if (value.tagName() == "value"){
			Id valueId("Meta_editor", "MetaEditor", "MetaEntityValue",
					QUuid::createUuid().toString());

			setStandartConfigurations(valueId, enumId, value.text(),
					value.attribute("displayedName", ""));

			mApi.setProperty(valueId, "valueName", value.text());
		}
	}
}
void XmlParser::setNodeAttributes(const QDomElement &node, const Id &nodeId)
{
	QDomNodeList nodeList = node.childNodes();

	for (unsigned i = 0; i < nodeList.length(); ++i) {
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

	for (unsigned i = 0; i < edgeList.length(); ++i) {
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

	for (unsigned i = 0; i < nodeAttributes.length(); ++i) {
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
		else if (attribute.tagName() == "pin")
			setPin(nodeId);
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
	for(unsigned i = 0; i < graphics.length(); ++i){
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

	for (unsigned i = 0; i < edgeAttributes.length(); ++i) {
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

	for (unsigned i = 0; i < generalizations.length(); ++i) {
		QDomElement generalization = generalizations.at(i).toElement();
		if (generalization.tagName() == "parent")
			initGeneralization(generalization, elementId);
	}
}

void XmlParser::setProperties(const QDomElement &element, const Id &elementId)
{
	QDomNodeList properties = element.childNodes();

	for (unsigned i = 0; i < properties.length(); ++i) {
		QDomElement property = properties.at(i).toElement();
		if (property.tagName() == "property")
			initProperty(property, elementId);
	}
}

void XmlParser::setFields(const QDomElement &element, const Id &elementId)
{
	QDomNodeList fields = element.childNodes();

	for (unsigned i = 0; i < fields.length(); ++i) {
		QDomElement field = fields.at(i).toElement();
		if (field.tagName() == "field") {
			Id fieldId("Meta_editor", "MetaEditor", "MetaEntityContextMenuField", QUuid::createUuid().toString());
			setStandartConfigurations(fieldId, elementId, field.attribute("name", ""),
					field.attribute("displayedName", ""));
		}
	}
}

void XmlParser::setContainers(const QDomElement &element, const Id &elementId)
{
	QDomNodeList containsElements = element.childNodes();
	for (unsigned i = 0; i < containsElements.length(); ++i) {
		QDomElement contains = containsElements.at(i).toElement();
		if (contains.tagName() == "contains") {
			QString type = contains.attribute("type", "");
			if (!mContainerList[elementId].contains(type))
				mContainerList[elementId] << type;
		}
		if (contains.tagName() == "properties")
			setContainerProperties(contains, elementId);
	}
}

void XmlParser::setContainerProperties(const QDomElement &element, const Id &elementId)
{
	QDomNodeList properties = element.childNodes();
	if (properties.size() > 0) {
		Id containerProperties("Meta_editor", "MetaEditor",
				"MetaEntityPropertiesAsContainer", QUuid::createUuid().toString());
		setStandartConfigurations(containerProperties, elementId,
				"properties", "");
		for (unsigned i = 0; i < properties.length(); ++i) {
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

	for (unsigned i = 0; i < connections.length(); ++i) {
		QDomElement connection = connections.at(i).toElement();
		if (connection.tagName() == "connection")
			initConnection(connection, elementId);
	}
}

void XmlParser::setUsages(const QDomElement &element, const Id &elementId)
{
	QDomNodeList usages = element.childNodes();

	for (unsigned i = 0; i < usages.length(); ++i) {
		QDomElement usage = usages.at(i).toElement();
		if (usage.tagName() == "usage")
			initUsage(usage, elementId);
	}
}

void XmlParser::setAssociations(const QDomElement &element, const Id &elementId)
{
	Id associationId("Meta_editor", "MetaEditor", "MetaEntityAssociation",
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

	for (unsigned i = 0; i < possibleEdges.length(); ++i) {
		QDomElement possibleEdge = possibleEdges.at(i).toElement();
		if (possibleEdge.tagName() == "possibleEdge")
			initPossibleEdge(possibleEdge, elementId);
	}
}

void XmlParser::setPin(const Id &elementId)
{
	mApi.setProperty(elementId, "isPin", "true");
}

void XmlParser::setAction(const Id &elementId)
{
	mApi.setProperty(elementId, "isAction", "true");
}

void XmlParser::initPossibleEdge(const QDomElement &possibleEdge, const Id &elementId)
{
	Id possibleEdgeId("Meta_editor", "MetaEditor", "MetaEntityPossibleEdge",
			QUuid::createUuid().toString());

	setStandartConfigurations(possibleEdgeId, elementId, possibleEdge.attribute("name", ""),
			possibleEdge.attribute("displayedName", ""));

	mApi.setProperty(possibleEdgeId, "beginName", possibleEdge.attribute("beginName", ""));
	mApi.setProperty(possibleEdgeId, "endName", possibleEdge.attribute("endName", ""));
	mApi.setProperty(possibleEdgeId, "directed", possibleEdge.attribute("directed", "false"));
}

void XmlParser::initProperty(const QDomElement &property, const Id &elementId)
{
	Id propertyId("Meta_editor", "MetaEditor", "MetaEntity_Attribute",
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
	Id connectionId("Meta_editor", "MetaEditor", "MetaEntityConnection",
			QUuid::createUuid().toString());

	setStandartConfigurations(connectionId, elementId, connection.attribute("name", ""),
			connection.attribute("displayedName", ""));

	mApi.setProperty(connectionId, "type", connection.attribute("type", ""));
}

void XmlParser::initUsage(const QDomElement &usage, const Id &elementId)
{
	Id usageId("Meta_editor", "MetaEditor", "MetaEntityUsage",
			QUuid::createUuid().toString());

	setStandartConfigurations(usageId, elementId, usage.attribute("name", ""),
			usage.attribute("displayedName", ""));

	mApi.setProperty(usageId, "type", usage.attribute("type", ""));
}

void XmlParser::initGeneralization(const QDomElement &generalization, const Id &elementId)
{
	Id generalizationId("Meta_editor", "MetaEditor", "MetaEntityParent",
			QUuid::createUuid().toString());

	setStandartConfigurations(generalizationId, elementId, generalization.attribute("parentName", ""),
			generalization.attribute("displayedName", ""));

	/*Id inheritanceId("Meta_editor", "MetaEditor", "Inheritance",
			QUuid::createUuid().toString());

	setStandartConfigurations(inheritanceId, mDiagram, generalization.attribute("name", ""),
			generalization.attribute("displayedName", ""));*/
}

Id XmlParser::findIdByType(QString const &type) const
{
	QString const name = type.section("::", -1);
	QString const diagramName = type.section("::", -2, -2);
	foreach (Id element, mElements.keys()) {
		QString const containerName = element.element() == "MetaEntityImport"
				? mApi.stringProperty(element, "as") : mApi.name(element);
		Id parent = mApi.parents(element)[0];
		if (containerName == name && (diagramName.isEmpty() || mApi.name(parent) == diagramName))
			return element;
	}
	return Id();
}

void XmlParser::initContainer()
{
	foreach (Id key, mContainerList.keys()) {
		foreach (QString type, mContainerList[key]) {
			Id containedElement = findIdByType(type);
			if (containedElement == Id())
				continue;

			QString existingContainers;
			if (mApi.hasProperty(containedElement, "container"))
				existingContainers = mApi.stringProperty(containedElement, "container");
			if (!existingContainers.isEmpty())
				existingContainers += ",";

			QString newContainer = mApi.name(key);
			if (!mApi.parents(key).isEmpty()) {
				Id parent = mApi.parents(key)[0];
				newContainer = mApi.name(parent) + "::" + newContainer;
			}

			if (existingContainers.split(',').contains(newContainer))
				continue;

			existingContainers += newContainer;
			mApi.setProperty(containedElement, "container", existingContainers);

		}
	}
}

void XmlParser::setStandartConfigurations(Id const &id, Id const &parent,
		const QString &name, const QString &displayedName)
{
	if (!mEditorManager.hasElement(id.type()))
		throw Exception(QString("%1 doesn't exist").arg(id.type().toString()));
	mApi.addChild(parent, id);
	mApi.setProperty(id, "name", name);
	if (displayedName != "")
		mApi.setProperty(id, "displayedName", displayedName);
	mApi.setProperty(id, "from", ROOT_ID.toVariant());
	mApi.setProperty(id, "to", ROOT_ID.toVariant());
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
