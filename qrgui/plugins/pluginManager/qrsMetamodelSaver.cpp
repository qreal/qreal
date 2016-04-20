/* Copyright 2016 QReal Research Group, Dmitry Mordvinov
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

#include "qrsMetamodelSaver.h"

#include <QtXml/QDomDocument>

#include <qrutils/scalableItem.h>
#include <qrrepo/repoApi.h>
#include <metaMetaModel/metamodel.h>
#include <metaMetaModel/nodeElementType.h>
#include <metaMetaModel/edgeElementType.h>
#include <metaMetaModel/patternType.h>

using namespace qReal;

const Id metamodelRootDiagramType = Id("MetaEditor", "MetaEditor", "MetamodelDiagram");
const Id metamodelDiagramType = Id("MetaEditor", "MetaEditor", "MetaEditorDiagramNode");

const Id metamodelNodeType = Id("MetaEditor", "MetaEditor", "MetaEntityNode");
const Id metamodelPortType = Id("MetaEditor", "MetaEditor", "MetaEntityPort");
const Id metamodelEdgeType = Id("MetaEditor", "MetaEditor", "MetaEntityEdge");
const Id metamodelEnumType = Id("MetaEditor", "MetaEditor", "MetaEntityEnum");
const Id metamodelImportType = Id("MetaEditor", "MetaEditor", "MetaEntityImport");

const Id metamodelAttributeType = Id("MetaEditor", "MetaEditor", "MetaEntity_Attribute");
const Id metamodelEnumValueType = Id("MetaEditor", "MetaEditor", "MetaEntityValue");
const Id metamodelGroupType = Id("MetaEditor", "MetaEditor", "MetaEntityGroup");
const Id metamodelGroupNodeType = Id("MetaEditor", "MetaEditor", "MetaEntityGroupNode");

const Id metamodelInheritanceLinkType = Id("MetaEditor", "MetaEditor", "Inheritance");
const Id metamodelContainmentLinkType = Id("MetaEditor", "MetaEditor", "Container");
const Id metamodelExplosionLinkType = Id("MetaEditor", "MetaEditor", "Explosion");
const Id metamodelPropertiesAsContainerType = Id("MetaEditor", "MetaEditor", "MetaEntityPropertiesAsContainer");

bool QrsMetamodelSaver::save(const QList<Metamodel *> &metamodels, const QString &path)
{
	qrRepo::RepoApi repo(QString(), false);
	for (const Metamodel * const metamodel : metamodels) {
		saveMetamodel(repo, *metamodel);
	}

	repo.saveTo(path);
	return true;
}

void QrsMetamodelSaver::saveMetamodel(qrRepo::RepoApi &repo, const Metamodel &metamodel)
{
	const Id metamodelId = metamodelRootDiagramType.sameTypeId();
	repo.addChild(Id::rootId(), metamodelId);
	repo.setName(metamodelId, metamodel.id());
	repo.setProperty(metamodelId, "displayedName", metamodel.friendlyName());
	repo.setProperty(metamodelId, "version", metamodel.version());

	Id lastDiagramId;
	for (const QString &diagram : metamodel.diagrams()) {
		saveDiagram(repo, metamodel, diagram, metamodelId, lastDiagramId);
	}

	if (lastDiagramId.isNull()) {
		// Metamodel contained no diagrams, nowhere to save enums and ports, giving up.
		return;
	}

	for (const QString &enumName : metamodel.enumNames()) {
		saveEnum(repo, metamodel, enumName, lastDiagramId);
	}
}

void QrsMetamodelSaver::saveDiagram(qrRepo::RepoApi &repo, const Metamodel &metamodel
		, const QString &diagramName, const Id &metamodelId, Id &resultingId)
{
	const Id diagramId = metamodelDiagramType.sameTypeId();
	resultingId = diagramId;
	repo.addChild(metamodelId, diagramId);
	repo.setName(diagramId, diagramName);
	repo.setProperty(diagramId, "displayedName", metamodel.diagramFriendlyName(diagramName));
	if (const ElementType *node = metamodel.diagramNode(diagramName)) {
		repo.setProperty(diagramId, "nodeName", node->name());
	}

	QMap<const ElementType *, Id> elements;
	saveObjectsOnDiagram(repo, metamodel, diagramName, diagramId, elements);
	saveLinksInMetamodel(repo, metamodel, diagramName, diagramId, elements);
}

void QrsMetamodelSaver::saveObjectsOnDiagram(qrRepo::RepoApi &repo, const Metamodel &metamodel
		, const QString &diagramName, const Id &diagramId, QMap<const ElementType *, Id> &elements)
{
	for (const ElementType *type : metamodel.elements(diagramName)) {
		Id id;
		switch (type->type()) {
		case ElementType::Type::node:
			id = saveNode(repo, type->toNode(), diagramId);
			break;
		case ElementType::Type::edge:
			id = saveEdge(repo, type->toEdge(), diagramId);
			break;
		case ElementType::Type::pattern:
			id = saveGroup(repo, type->toPattern(), diagramId);
			break;
		}

		elements[type] = id;
	}
}

Id QrsMetamodelSaver::saveNode(qrRepo::RepoApi &repo, const NodeElementType &node, const Id &diagram)
{
	const Id nodeId = metamodelNodeType.sameTypeId();
	repo.addChild(diagram, nodeId);
	repo.setName(nodeId, node.name());
	repo.setProperty(nodeId, "displayedName", node.friendlyName());
	repo.setProperty(nodeId, "description", node.description());
	repo.setProperty(nodeId, "path", node.mouseGesture());
	repo.setProperty(nodeId, "abstract", node.isHidden());
	repo.setProperty(nodeId, "isResizeable", node.isResizable());
	repo.setProperty(nodeId, "createChildrenFromMenu", node.createChildrenFromMenu());

	saveSdfGraphics(repo, node, nodeId);
	saveContainerProperties(repo, node, nodeId);
	saveProperties(repo, node, nodeId);
	return nodeId;
}

Id QrsMetamodelSaver::saveEdge(qrRepo::RepoApi &repo, const EdgeElementType &edge, const Id &diagram)
{
	const Id edgeId = metamodelEdgeType.sameTypeId();
	repo.addChild(diagram, edgeId);
	repo.setName(edgeId, edge.name());
	repo.setProperty(edgeId, "displayedName", edge.friendlyName());
	repo.setProperty(edgeId, "description", edge.description());
	repo.setProperty(edgeId, "isDividable", edge.isDividable());
	repo.setProperty(edgeId, "lineType", penStyleToString(edge.penStyle()));
	repo.setProperty(edgeId, "shape", linkShapeToString(edge.shapeType()));

	const QList<LabelProperties> &labels = edge.labels();
	if (!labels.isEmpty()) {
		const LabelProperties &label = labels.first();
		repo.setProperty(edgeId, "labelText", label.isStatic() ? label.text() : label.binding());
		repo.setProperty(edgeId, "labelType", label.isStatic() ? "staticText" : "dynamicText");
		repo.setProperty(edgeId, "hardLabel", label.isHard());
	}

	saveAssociations(repo, edge, edgeId);
	saveProperties(repo, edge, edgeId);
	savePorts(repo, edge, edgeId, "from");
	savePorts(repo, edge, edgeId, "to");
	return edgeId;
}

Id QrsMetamodelSaver::saveGroup(qrRepo::RepoApi &repo, const PatternType &pattern, const Id &diagramId)
{
	/// @todo: Group saving will be not implemented unless objects list will be returned by PatternType instead
	/// of that idiotic XML.
	Q_UNUSED(repo)
	Q_UNUSED(pattern)
	Q_UNUSED(diagramId)
	return Id();
}

void QrsMetamodelSaver::saveEnum(qrRepo::RepoApi &repo, const Metamodel &metamodel
		, const QString &enumName, const Id &diagram)
{
	const Id enumId = metamodelEnumType.sameTypeId();
	repo.addChild(diagram, enumId);
	repo.setName(enumId, enumName);
	repo.setProperty(enumId, "editable", metamodel.isEnumEditable(enumName));
	saveEnumValues(repo, metamodel, enumName, enumId);
}

void QrsMetamodelSaver::saveProperties(qrRepo::RepoApi &repo, const ElementType &element, const Id &id)
{
	for (const QString &propertyName : element.propertyNames()) {
		const Id propertyId = metamodelAttributeType.sameTypeId();
		repo.addChild(id, propertyId);
		repo.setName(propertyId, propertyName);
		repo.setProperty(propertyId, "attributeType", element.propertyType(propertyName));
		repo.setProperty(propertyId, "defaultValue", element.propertyDefaultValue(propertyName));
		repo.setProperty(propertyId, "displayedName", element.propertyDisplayedName(propertyName));
		repo.setProperty(propertyId, "description", element.propertyDescription(propertyName));
	}
}

void QrsMetamodelSaver::saveEnumValues(qrRepo::RepoApi &repo, const Metamodel &metamodel
		, const QString &enumName, const Id &enumId)
{
	for (const QPair<QString, QString> &value : metamodel.enumValues(enumName)) {
		const Id valueId = metamodelEnumValueType.sameTypeId();
		repo.addChild(enumId, valueId);
		repo.setName(valueId, value.first);
		repo.setProperty(valueId, "displayedName", value.second);
	}
}

void QrsMetamodelSaver::saveGroupNodes(qrRepo::RepoApi &repo, const QDomElement &parent, const Id &id)
{
	/// @todo: Group saving will be not implemented unless objects list will be returned by PatternType instead
	/// of that idiotic XML.
	Q_UNUSED(repo)
	Q_UNUSED(parent)
	Q_UNUSED(id)
}

void QrsMetamodelSaver::saveAssociations(qrRepo::RepoApi &repo, const EdgeElementType &edge, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(edge)
	Q_UNUSED(id)
	/// @todo: This method should set shape`s begin and end types. For now it is only implemented in generated mode.
	/// Drawind code should be migrated from qrxc to engine and then here we simply set generate types as-is.
}

void QrsMetamodelSaver::savePorts(qrRepo::RepoApi &repo, const EdgeElementType &edge
		, const Id &id, const QString &direction)
{
	/// @todo: Support it
	Q_UNUSED(repo)
	Q_UNUSED(edge)
	Q_UNUSED(id)
	Q_UNUSED(direction)
}

void QrsMetamodelSaver::saveSdfGraphics(qrRepo::RepoApi &repo, const NodeElementType &node, const Id &id)
{
	QDomDocument sdfDocument;
	QDomElement graphicsElement = sdfDocument.createElement("graphics");
	sdfDocument.appendChild(graphicsElement);

	QDomElement pictureElement = sdfDocument.importNode(node.sdf(), true).toElement();
	graphicsElement.appendChild(pictureElement);

	QDomElement labels = sdfDocument.createElement("labels");
	QDomElement ports = sdfDocument.createElement("ports");
	graphicsElement.appendChild(labels);
	graphicsElement.appendChild(ports);

	saveLabels(node, labels, node.size().width(), node.size().height());
	saveNodePorts(node, ports);

	repo.setProperty(id, "shape", sdfDocument.toString(4));
}

void QrsMetamodelSaver::saveLabels(const NodeElementType &node, QDomElement &labels, int width, int height)
{
	for (const LabelProperties &label : node.labels()) {
		QDomElement labelElement = labels.ownerDocument().createElement("label");
		labels.appendChild(labelElement);
		const int x = static_cast<int>(label.x() * width);
		const int y = static_cast<int>(label.y() * height);
		labelElement.setAttribute("x", QString::number(x) + (label.scalingX() ? "a" : ""));
		labelElement.setAttribute("y", QString::number(y) + (label.scalingY() ? "a" : ""));
		labelElement.setAttribute("text", label.text());
		labelElement.setAttribute("textBinded", label.binding());
		labelElement.setAttribute("prefix", label.prefix());
		labelElement.setAttribute("suffix", label.suffix());
		labelElement.setAttribute("readOnly", label.isReadOnly() ? "true" : "false");
		labelElement.setAttribute("rotation", label.rotation());
		labelElement.setAttribute("background", label.background().name());
		labelElement.setAttribute("hard", label.isHard() ? "true" : "false");
		labelElement.setAttribute("isPlainText", label.isPlainTextMode() ? "true" : "false");
	}
}

void QrsMetamodelSaver::saveNodePorts(const NodeElementType &node, QDomElement &portsTag)
{
	for (const PointPortInfo &pointPort : node.pointPorts()) {
		QDomElement portElement = portsTag.ownerDocument().createElement("pointPort");
		portsTag.appendChild(portElement);
		const int x = static_cast<int>(pointPort.point.x() * pointPort.initWidth);
		const int y = static_cast<int>(pointPort.point.y() * pointPort.initHeight);
		portElement.setAttribute("x", QString::number(x) + (pointPort.scalesX ? "a" : ""));
		portElement.setAttribute("y", QString::number(y) + (pointPort.scalesY ? "a" : ""));
		portElement.setAttribute("type", pointPort.type);
	}

	for (const LinePortInfo &linePort : node.linePorts()) {
		QDomElement portElement = portsTag.ownerDocument().createElement("linePort");
		QDomElement startElement = portsTag.ownerDocument().createElement("start");
		QDomElement endElement = portsTag.ownerDocument().createElement("end");
		portsTag.appendChild(portElement);
		portElement.appendChild(startElement);
		portElement.appendChild(endElement);
		const int x1 = static_cast<int>(linePort.line.x1() * linePort.initWidth);
		const int y1 = static_cast<int>(linePort.line.y1() * linePort.initHeight);
		const int x2 = static_cast<int>(linePort.line.x2() * linePort.initWidth);
		const int y2 = static_cast<int>(linePort.line.y2() * linePort.initHeight);
		startElement.setAttribute("startx", QString::number(x1) + (linePort.scalesX1 ? "a" : ""));
		startElement.setAttribute("starty", QString::number(y1) + (linePort.scalesY1 ? "a" : ""));
		endElement.setAttribute("endx", QString::number(x2) + (linePort.scalesX2 ? "a" : ""));
		endElement.setAttribute("endy", QString::number(y2) + (linePort.scalesY2 ? "a" : ""));
		portElement.setAttribute("type", linePort.type);
	}

	for (const CircularPortInfo &circlePort : node.circularPorts()) {
		QDomElement portElement = portsTag.ownerDocument().createElement("circularPort");
		portsTag.appendChild(portElement);
		const int x = static_cast<int>(circlePort.center.x() * circlePort.initWidth);
		const int y = static_cast<int>(circlePort.center.y() * circlePort.initHeight);
		portElement.setAttribute("x", QString::number(x) + (circlePort.scalesX ? "a" : ""));
		portElement.setAttribute("y", QString::number(y) + (circlePort.scalesY ? "a" : ""));
		portElement.setAttribute("r", circlePort.radius);
		portElement.setAttribute("type", circlePort.type);
	}
}

void QrsMetamodelSaver::saveContainerProperties(qrRepo::RepoApi &repo, const NodeElementType &node, const Id &id)
{
	if (!node.isContainer()) {
		return;
	}

	const Id propertiesId = metamodelPropertiesAsContainerType.sameTypeId();
	repo.addChild(id, propertiesId);
	repo.setName(propertiesId, node.name() + "Properties");
	repo.setProperty(propertiesId, "forestallingSize", intVectorToString(node.sizeOfForestalling()));
	repo.setProperty(propertiesId, "childrenForestallingSize", node.sizeOfChildrenForestalling());

	repo.setProperty(propertiesId, "sortContainer", node.isSortingContainer() ? "true" : "false");
	repo.setProperty(propertiesId, "banChildrenMove", node.hasMovableChildren() ? "true" : "false");
	repo.setProperty(propertiesId, "minimizeToChildren", node.minimizesToChildren() ? "true" : "false");
	repo.setProperty(propertiesId, "maximizeChildren", node.maximizesChildren() ? "true" : "false");
}

void QrsMetamodelSaver::saveLinksInMetamodel(qrRepo::RepoApi &repo, const Metamodel &metamodel
		, const QString &diagramName, const Id &diagram, const QMap<const ElementType *, Id> &elements)
{
	for (const ElementType * const type : metamodel.elements(diagramName)) {
		for (const qrgraph::Edge * const edge : type->outgoingEdges()) {
			Id from = elements[static_cast<const ElementType *>(edge->begin())];
			Id to = elements[static_cast<const ElementType *>(edge->end())];
			from = from.isNull() ? Id::rootId() : from;
			to = to.isNull() ? Id::rootId() : to;

			switch (edge->type()) {
			case ElementType::generalizationLinkType: {
				const Id linkId = metamodelInheritanceLinkType.sameTypeId();
				repo.addChild(diagram, linkId);
				repo.setFrom(linkId, to);
				repo.setTo(linkId, from);
				break;
			}
			case ElementType::containmentLinkType: {
				const Id linkId = metamodelContainmentLinkType.sameTypeId();
				repo.addChild(diagram, linkId);
				repo.setFrom(linkId, from);
				repo.setTo(linkId, to);
				break;
			}
			case ElementType::explosionLinkType: {
				const Id linkId = metamodelExplosionLinkType.sameTypeId();
				const Explosion *explosion = static_cast<const Explosion *>(edge);
				repo.addChild(diagram, linkId);
				repo.setFrom(linkId, from);
				repo.setTo(linkId, to);
				repo.setProperty(linkId, "makeReusable", explosion->isReusable());
				repo.setProperty(linkId, "requireImmediateLinkage", explosion->requiresImmediateLinkage());
				break;
			}
			default:
				qWarning() << "Unsupported metamodel link type" << edge->type();
				break;
			}
		}
	}
}

QString QrsMetamodelSaver::penStyleToString(Qt::PenStyle style)
{
	switch (style) {
	case Qt::SolidLine:
		return "solidLine";
	case Qt::DashLine:
		return "dashLine";
	case Qt::DotLine:
		return "dotLine";
	default:
		emit errorOccured(tr("Unknown link style type %1").arg(static_cast<int>(style)), Id());
		return QString();
	}
}

QString QrsMetamodelSaver::linkShapeToString(LinkShape linkShape)
{
	switch (linkShape) {
	case LinkShape::curve:
		return "curve";
	case LinkShape::square:
		return "square";
	case LinkShape::broken:
		return "broken";
	}

	emit errorOccured(tr("Unknown link shape type %1").arg(static_cast<int>(linkShape)), Id());
	return "broken";
}

QString QrsMetamodelSaver::intVectorToString(const QVector<int> &values)
{
	QStringList stringValues;
	stringValues.reserve(values.size());
	for (int i = 0; i < values.size(); ++i) {
		stringValues[i] = QString::number(values[i]);
	}

	return stringValues.join(",");
}
