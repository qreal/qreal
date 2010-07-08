#include "xmlParser.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtXml/QDomDocument>
#include <QtCore/QProcess>
#include <QPointF>
#include <QPolygonF>

#include "math.h"

#include "../../../qrrepo/repoApi.h"
#include "../../../utils/xmlUtils.h"

using namespace qReal;
using namespace parsers;

XmlParser::XmlParser(qrRepo::RepoApi &api, EditorManager const &editorManager)
	: mApi(api), mEditorManager(editorManager)
{
}

void XmlParser::parseFile(const QString &fileName)
{
	QDomDocument doc = utils::xmlUtils::loadDocument(fileName);

	initMetamodel(doc);

	QDomNodeList diagrams = doc.elementsByTagName("diagram");

	for (unsigned i = 0; i < diagrams.length(); ++i) {
		QDomElement diagram = diagrams.at(i).toElement();
		initDiagram (diagram, mMetamodel, diagram.attribute("name", "diagram"), diagram.attribute("displayedName", "diagram"));
	}
}

void XmlParser::initMetamodel(const QDomDocument &document)
{
	mMetamodel = Id("Meta_editor", "MetaEditor", "MetaEditor_MetamodelDiagram", QUuid::createUuid().toString());

	QDomNodeList includeList = document.elementsByTagName("include");
	QString includeListString = "";

	for (unsigned i = 0; i < includeList.length(); ++i) {
		QDomElement include = includeList.at(i).toElement();
		includeListString += include.text() + ", ";
	}
	setStandartConfigurations(mMetamodel, ROOT_ID, "Metamodel", "");
	mApi.setProperty(mMetamodel, "include", includeListString);
}

void XmlParser::initDiagram(const QDomElement &diagram, const Id &parent, const QString &name, const QString &displayedName)
{
	Id diagramId("Meta_editor", "MetaEditor", "MetaEditor_MetaEditorDiagramNode", QUuid::createUuid().toString());

	setStandartConfigurations(diagramId, parent, name, displayedName);

	mApi.setProperty(diagramId, "position", QPointF(0,0));
	mApi.setProperty(diagramId, "configuration", QVariant(QPolygon()));

	createDiagramAttributes(diagram, diagramId);
}

void XmlParser::createDiagramAttributes(const QDomElement &diagram, Id diagramId)
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

void XmlParser::createNonGraphicElements(const QDomElement &type, Id diagramId)
{
	QDomNodeList enums = type.childNodes();

	for (unsigned i = 0; i < enums.length(); ++i) {
		QDomElement enumElement = enums.at(i).toElement();
		if (enumElement.tagName() == "enum")
			initEnum(enumElement, diagramId);
	}
}

void XmlParser::createGraphicElements(const QDomElement &type, Id diagramId)
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

void XmlParser::initEnum(const QDomElement &enumElement, Id diagramId)
{
	Id enumId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityEnum", QUuid::createUuid().toString());

	setStandartConfigurations(enumId, diagramId, enumElement.attribute("name", ""), enumElement.attribute("displayedName", ""));

	mApi.setProperty(enumId, "position", QPointF(0,0));
	mApi.setProperty(enumId, "configuration", QVariant(QPolygon()));

	setEnumAttributes(enumElement, enumId);
}

void XmlParser::initNode(const QDomElement &node, Id diagramId)
{
	Id nodeId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityNode", QUuid::createUuid().toString());
	mElements.insert(nodeId, node.attribute("name", ""));

	setStandartConfigurations(nodeId, diagramId, node.attribute("name", ""), node.attribute("displayedName", ""));

	mApi.setProperty(nodeId, "position", QPointF(0,0));
	mApi.setProperty(nodeId, "configuration", QVariant(QPolygon()));

	setNodeAttributes(node, nodeId);
}

void XmlParser::initEdge(const QDomElement &edge, Id diagramId)
{
	Id edgeId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityEdge", QUuid::createUuid().toString());
	mElements.insert(edgeId, edge.attribute("name", ""));

	setStandartConfigurations(edgeId, diagramId, edge.attribute("name", ""), edge.attribute("displayedName", ""));

	mApi.setProperty(edgeId, "position", QPointF(0,0));
	mApi.setProperty(edgeId, "configuration", QVariant(QPolygon()));

	setEdgeAttributes(edge, edgeId);
}

void XmlParser::initImport(const QDomElement &import, Id diagramId)
{
	Id importationId("Meta_editor", "MetaEditor", "MetaEditor_Importation", QUuid::createUuid().toString());

	QStringList nameList = import.attribute("name", "").split("::", QString::SkipEmptyParts);

	setStandartConfigurations(importationId, diagramId, nameList.at(0), import.attribute("displayedName", ""));

	mApi.setProperty(importationId, "as", import.attribute("as", ""));

	mApi.setProperty(importationId, "position", QPointF(0,0));
	mApi.setProperty(importationId, "configuration", QVariant(QPolygon()));

	Id importObjectId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityImported", QUuid::createUuid().toString());

	setStandartConfigurations(importObjectId, diagramId, nameList.at(1), nameList.at(1));

	mApi.setProperty(importObjectId, "position", QPointF(0,0));
	mApi.setProperty(importObjectId, "configuration", QVariant(QPolygon()));
}

void XmlParser::setEnumAttributes(const QDomElement &enumElement, Id enumId)
{
	QDomNodeList values = enumElement.childNodes();

	for (unsigned i = 0; i < values.length(); ++i) {
		QDomElement value = values.at(i).toElement();
		if (value.tagName() == "value"){
			Id valueId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityValue", QUuid::createUuid().toString());

			setStandartConfigurations(valueId, enumId, value.attribute("name", ""), value.attribute("displayedName", ""));

			mApi.setProperty(valueId, "position", QPointF(0,0));
			mApi.setProperty(valueId, "configuration", QVariant(QPolygon()));

			mApi.setProperty(valueId, "valueName", value.text());
		}
	}
}
void XmlParser::setNodeAttributes(const QDomElement &node, Id nodeId)
{
	QDomNodeList nodeList = node.childNodes();

	for (unsigned i = 0; i < nodeList.length(); ++i) {
		QDomElement tag = nodeList.at(i).toElement();
		if (tag.tagName() == "logic")
			setNodeConfigurations(tag, nodeId);
	}
}

void XmlParser::setEdgeAttributes(const QDomElement &edge, Id edgeId)
{
	QDomNodeList edgeList = edge.childNodes();

	for (unsigned i = 0; i < edgeList.length(); ++i) {
		QDomElement tag = edgeList.at(i).toElement();
		if (tag.tagName() == "logic")
			setEdgeConfigurations(tag, edgeId);
	}
}

void XmlParser::setNodeConfigurations(const QDomElement &tag, Id nodeId)
{
	QDomNodeList nodeAttributes = tag.childNodes();

	for (unsigned i = 0; i < nodeAttributes.length(); ++i) {
		QDomElement attribute = nodeAttributes.at(i).toElement();
		if (attribute.tagName() == "generalizations")
			setGeneralization(attribute, nodeId);
		if (attribute.tagName() == "properties")
			setProperties(attribute, nodeId);
		if (attribute.tagName() == "container")
			setContainers(attribute, nodeId);
		if (attribute.tagName() == "connections")
			setConnections(attribute, nodeId);
		if (attribute.tagName() == "usages");
		setUsages(attribute, nodeId);
	}
}

void XmlParser::setEdgeConfigurations(const QDomElement &tag, Id edgeId)
{
	QDomNodeList edgeAttributes = tag.childNodes();

	for (unsigned i = 0; i < edgeAttributes.length(); ++i) {
		QDomElement attribute = edgeAttributes.at(i).toElement();
		if (attribute.tagName() == "generalizations")
			setGeneralization(attribute, edgeId);
		if (attribute.tagName() == "properties")
			setProperties(attribute, edgeId);
		if (attribute.tagName() == "container")
			setContainers(attribute, edgeId);
		if (attribute.tagName() == "assotiations")
			setAssotiations(attribute, edgeId);
	}
}

void XmlParser::setGeneralization(const QDomElement &element, Id elementId)
{
	QDomNodeList generalizations = element.childNodes();

	for (unsigned i = 0; i < generalizations.length(); ++i) {
		QDomElement generalization = generalizations.at(i).toElement();
		if (generalization.tagName() == "parent")
			initGeneralization(generalization, elementId);
	}
}

void XmlParser::setProperties(const QDomElement &element, Id elementId)
{
	QDomNodeList properties = element.childNodes();

	for (unsigned i = 0; i < properties.length(); ++i) {
		QDomElement property = properties.at(i).toElement();
		if (property.tagName() == "property")
			initProperty(property, elementId);
	}
}

void XmlParser::setContainers(const QDomElement &element, Id elementId)
{
	QDomNodeList contains = element.childNodes();

	for (unsigned i = 0; i < contains.length(); ++i) {
		QDomElement contain = contains.at(i).toElement();
		mContainerList.insert(contain.attribute("name", ""),element.attribute("name", ""));
	}
}

void XmlParser::setConnections(const QDomElement &element, Id elementId)
{
	QDomNodeList connections = element.childNodes();

	for (unsigned i = 0; i < connections.length(); ++i) {
		QDomElement connection = connections.at(i).toElement();
		if (connection.tagName() == "connection")
			initConnection(connection, elementId);
	}
}

void XmlParser::setUsages(const QDomElement &element, Id elementId)
{
	QDomNodeList usages = element.childNodes();

	for (unsigned i = 0; i < usages.length(); ++i) {
		QDomElement usage = usages.at(i).toElement();
		if (usage.tagName() == "usage")
			initUsage(usage, elementId);
	}
}

void XmlParser::setAssotiations(const QDomElement &element, Id elementId)
{
	Id assotiationId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityAssotiation", QUuid::createUuid().toString());
	QDomNodeList assotiations = element.childNodes();

	QDomElement assotiation = assotiations.at(0).toElement();

	setStandartConfigurations(assotiationId, elementId, assotiation.attribute("name", ""),
							  assotiation.attribute("displayedName", ""));

	mApi.setProperty(assotiationId, "beginType", element.attribute("beginType", ""));
	mApi.setProperty(assotiationId, "endType", element.attribute("endType", ""));
	mApi.setProperty(assotiationId, "beginName", assotiation.attribute("beginName", ""));
	mApi.setProperty(assotiationId, "endName", assotiation.attribute("endName", ""));

	mApi.setProperty(assotiationId, "position", QPointF(0,0));
	mApi.setProperty(assotiationId, "configuration", QVariant(QPolygon()));
}

void XmlParser::initProperty(const QDomElement &property, Id elementId)
{
	Id propertyId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntity_Attribute", QUuid::createUuid().toString());

	setStandartConfigurations(propertyId, elementId, property.attribute("name", ""), property.attribute("displayedName", ""));

	mApi.setProperty(propertyId, "type", property.attribute("type", ""));
	mApi.setProperty(propertyId, "attributeType", property.attribute("attributeType", "0"));

	QDomNodeList defaultValue = property.childNodes();
	if (!defaultValue.isEmpty())
		mApi.setProperty(propertyId, "defaultValue", defaultValue.at(0).toElement().attribute("defaultValue", "false"));

	mApi.setProperty(propertyId, "position", QPointF(0,0));
	mApi.setProperty(propertyId, "configuration", QVariant(QPolygon()));
}

void XmlParser::initConnection(const QDomElement &connection, Id elementId)
{
	Id connectionId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityConnection", QUuid::createUuid().toString());

	setStandartConfigurations(connectionId, elementId, connection.attribute("name", ""), connection.attribute("displayedName", ""));

	mApi.setProperty(connectionId, "type", connection.attribute("type", ""));

	mApi.setProperty(connectionId, "position", QPointF(0,0));
	mApi.setProperty(connectionId, "configuration", QVariant(QPolygon()));
}

void XmlParser::initUsage(const QDomElement &usage, Id elementId)
{
	Id usageId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityUsage", QUuid::createUuid().toString());

	setStandartConfigurations(usageId, elementId, usage.attribute("name", ""), usage.attribute("displayedName", ""));

	mApi.setProperty(usageId, "type", usage.attribute("type", ""));

	mApi.setProperty(usageId, "position", QPointF(0,0));
	mApi.setProperty(usageId, "configuration", QVariant(QPolygon()));
}

void XmlParser::initGeneralization(const QDomElement &generalization, Id elementId)
{
	Id generalizationId("Meta_editor", "MetaEditor", "MetaEditor_MetaEntityImported", QUuid::createUuid().toString());

	setStandartConfigurations(generalizationId, elementId, generalization.attribute("name", ""), generalization.attribute("displayedName", ""));

	mApi.setProperty(generalizationId, "parentName", generalization.attribute("parentName", ""));

	mApi.setProperty(generalizationId, "position", QPointF(0,0));
	mApi.setProperty(generalizationId, "configuration", QVariant(QPolygon()));

	Id inheritanceId("Meta_editor", "MetaEditor", "MetaEditor_Inheritance", QUuid::createUuid().toString());

	setStandartConfigurations(inheritanceId, elementId, generalization.attribute("name", ""), generalization.attribute("displayedName", ""));
	mApi.setProperty(inheritanceId, "position", QPointF(0,0));
	mApi.setProperty(inheritanceId, "configuration", QVariant(QPolygon()));
}

void XmlParser::initContainer()
{
	foreach (Id key, mElements.keys()) {
		QString name = mElements[key];
		mApi.setProperty(key, "container", mContainerList[name]);
	}

}

void XmlParser::setStandartConfigurations(Id id, Id parent, const QString &name, const QString &displayedName)
{
	mApi.addChild(parent, id);
	mApi.setProperty(id, "name", name);
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

}
