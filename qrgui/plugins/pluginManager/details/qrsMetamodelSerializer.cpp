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

#include "qrsMetamodelSerializer.h"

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

QList<Metamodel *> QrsMetamodelSerializer::load(const QString &pathToQrs)
{
	QList<Metamodel *> result;
	const qrRepo::RepoApi repo(pathToQrs);
	if (!repo.exist(Id::rootId())) {
		return result;
	}

	for (const Id &id : repo.children(Id::rootId())) {
		if (id.type() == metamodelRootDiagramType && repo.isLogicalElement(id)) {
			result << parseMetamodel(repo, id);
		}
	}

	return result;
}

Metamodel *QrsMetamodelSerializer::parseMetamodel(const qrRepo::RepoApi &repo, const Id &metamodelDiagram)
{
	Metamodel *result = new Metamodel(validateName(repo, metamodelDiagram));
	result->setFriendlyName(stringProperty(repo, metamodelDiagram, "displayedName"));
	result->setVersion(stringProperty(repo, metamodelDiagram, "version"));

	for (const Id &id : repo.children(metamodelDiagram)) {
		if (id.type() == metamodelDiagramType && repo.isLogicalElement(id)) {
			parseDiagram(repo, *result, id);
		}
	}

	return result;
}

void QrsMetamodelSerializer::parseDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram)
{
	const QString diagramName = validateName(repo, diagram);
	metamodel.addDiagram(diagramName);

	metamodel.setDiagramFriendlyName(diagramName, stringProperty(repo, diagram, "displayedName"));
	metamodel.setDiagramNode(diagramName, validateRootNode(repo, diagram));

	parseObjectsOnDiagram(repo, metamodel, diagram);
	parseLinksOnDiagram(repo, metamodel, diagram);
}

void QrsMetamodelSerializer::parseObjectsOnDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram)
{
	if (diagram.isNull() || diagram == Id::rootId()) {
		return;
	}

	for (const Id &id : repo.children(diagram)) {
		if (!repo.isLogicalElement(id)) {
			continue;
		}

		const Id type = id.type();
		if (type == metamodelEnumType) {
			parseEnum(repo, metamodel, id);
		} else if (type == metamodelPortType) {
			parsePort(repo, metamodel, id);
		} else if (type == metamodelGroupType) {
			parseGroup(repo, metamodel, diagram, id);
		} else if (type == metamodelImportType) {
			parseImport(repo, metamodel, id);
		} else if (type == metamodelNodeType) {
			parseNode(repo, metamodel, diagram, id);
		} else if (type == metamodelEdgeType) {
			parseEdge(repo, metamodel, diagram, id);
		}
	}
}

void QrsMetamodelSerializer::parseImport(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(metamodel)
	Q_UNUSED(id)
}

void QrsMetamodelSerializer::parseNode(const qrRepo::RepoApi &repo
		, Metamodel &metamodel, const Id &diagram, const Id &id)
{
	NodeElementType *node = new NodeElementType(metamodel);
	metamodel.addElement(*node);

	node->setName(validateName(repo, id));
	node->setFriendlyName(repo.stringProperty(id, "displayedName"));
	node->setDiagram(repo.name(diagram));
	node->setMouseGesture(stringProperty(repo, id, "path"));
	node->setDescription(stringProperty(repo, id, "description"));
	node->setHidden(boolProperty(repo, id, "abstract"));
	node->setResizable(boolProperty(repo, id, "isResizeable", true));
	node->setCreateChildrenFromMenu(boolProperty(repo, id, "createChildrenFromMenu"));

	parseSdfGraphics(repo, *node, id);
	parseContainerProperties(repo, *node, id);
	parseProperties(repo, *node, id);
}

void QrsMetamodelSerializer::parseEdge(const qrRepo::RepoApi &repo
		, Metamodel &metamodel, const Id &diagram, const Id &id)
{
	EdgeElementType *edge = new EdgeElementType(metamodel);
	metamodel.addElement(*edge);

	edge->setName(validateName(repo, id));
	edge->setFriendlyName(repo.stringProperty(id, "displayedName"));
	edge->setDiagram(repo.name(diagram));
	edge->setDescription(stringProperty(repo, id, "description"));
	edge->setDividable(boolProperty(repo, id, "isDividable"));

	edge->setPenWidth(1);
	edge->setPenColor(Qt::black);
	edge->setPenStyle(parsePenStyle(stringProperty(repo, id, "lineType"), id));
	edge->setShapeType(parseLinkShape(stringProperty(repo, id, "shape"), id));

	const QString labelText = stringProperty(repo, id, "labelText");

	if (!labelText.isEmpty()) {
		LabelProperties label;
		const QString labelType = stringProperty(repo, id, "labelType");
		if (labelType == "staticText") {
			label = LabelProperties(0, 0, 0, labelText, 0);
		} else if (labelType == "dynamicText") {
			label = LabelProperties(0, 0, 0, labelText, false, 0);
		} else {
			emit errorOccured(tr("Incorrect label type"), id);
		}

		label.setHard(boolProperty(repo, id, "hardLabel"));
	}

	parseAssociations(repo, *edge, id);
	parseProperties(repo, *edge, id);
	parsePorts(repo, *edge, id, "from");
	parsePorts(repo, *edge, id, "to");
}

void QrsMetamodelSerializer::parseEnum(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	const QString enumName = validateName(repo, id);
	metamodel.addEnum(enumName, parseEnumValues(repo, id));
	metamodel.setEnumEditable(enumName, boolProperty(repo, id, "editable"));
}

void QrsMetamodelSerializer::parsePort(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(metamodel)
	Q_UNUSED(id)
}

void QrsMetamodelSerializer::parseGroup(const qrRepo::RepoApi &repo
		, Metamodel &metamodel, const Id &diagram, const Id &id)
{
	/// @todo: We should not use XML here, PatternType must not parse XML at all.
	QDomDocument document;
	QDomElement groupElement = document.createElement("group");
	groupElement.setAttribute("name", validateName(repo, id));
	groupElement.setAttribute("rootNode", stringProperty(repo, id, "rootNode"));
	document.appendChild(groupElement);
	parseGroupNodes(repo, groupElement, id);
	PatternType *pattern = new PatternType(metamodel);
	pattern->setXml(document.toString(4));
	pattern->setDiagram(repo.name(diagram));
	metamodel.addElement(*pattern);
}

void QrsMetamodelSerializer::parseProperties(const qrRepo::RepoApi &repo, ElementType &element, const Id &id)
{
	const IdList children = repo.children(id);

	for (const Id &child : children) {
		if (child.type() == metamodelAttributeType && repo.isLogicalElement(id)) {
			const QString type = repo.stringProperty(child, "attributeType");
			element.addProperty(repo.name(child), type
					, stringProperty(repo, child, "defaultValue", "string")
					, stringProperty(repo, child, "displayedName")
					, stringProperty(repo, child, "description")
					, type == "reference");
		}
	}
}

void QrsMetamodelSerializer::parsePorts(const qrRepo::RepoApi &repo
		, EdgeElementType &edge, const Id &id, const QString &direction)
{
	const QString propertyName = direction + "Ports";
	const QStringList ports = stringProperty(repo, id, propertyName).split(',', QString::SkipEmptyParts);
	QStringList result;
	for (const QString &port : ports) {
		const Id portId = Id::loadFromString(port);
		result << repo.name(portId);
	}

	if (direction == "from") {
		edge.setFromPortTypes(result);
	} else {
		edge.setToPortTypes(result);
	}
}

QList<QPair<QString, QString>> QrsMetamodelSerializer::parseEnumValues(const qrRepo::RepoApi &repo, Id const &id)
{
	QList<QPair<QString, QString>> result;
	for (const Id &child : repo.children(id)) {
		if (child.type() == metamodelEnumValueType && repo.isLogicalElement(child)) {
			result << qMakePair(validateName(repo, child), stringProperty(repo, child, "displayedName"));
		}
	}

	return result;
}

void QrsMetamodelSerializer::parseGroupNodes(const qrRepo::RepoApi &repo, QDomElement &parent, const Id &id)
{
	/// @todo: We should not use XML here, PatternType must not parse XML at all.
	for (const Id &child : repo.children(id)) {
		if (repo.isLogicalElement(child)) {
			QDomElement groupNodeTag = parent.ownerDocument().createElement("groupNode");
			groupNodeTag.setAttribute("name", validateName(repo, child));
			groupNodeTag.setAttribute("parent", stringProperty(repo, child, "parent"));
			groupNodeTag.setAttribute("xPosition", stringProperty(repo, child, "xPosition"));
			groupNodeTag.setAttribute("yPosition", stringProperty(repo, child, "yPosition"));

			const Id typeElement = Id::loadFromString(stringProperty(repo, child, "type"));
			groupNodeTag.setAttribute("type", validateName(repo, typeElement));

			parent.appendChild(groupNodeTag);
		}
	}
}

void QrsMetamodelSerializer::parseAssociations(const qrRepo::RepoApi &repo, EdgeElementType &edge, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(edge)
	Q_UNUSED(id)
	/// @todo: This method should set shape`s begin and end types. For now it is only implemented in generated mode.
	/// Drawind code should be migrated from qrxc to engine and then here we simply set generate types as-is.
}

void QrsMetamodelSerializer::parseSdfGraphics(const qrRepo::RepoApi &repo, NodeElementType &node, const Id &id)
{
	const QString sdfString = stringProperty(repo, id, "shape");
	QDomDocument sdfDocument;
	sdfDocument.setContent(sdfString);
	const QDomElement graphicsElement = sdfDocument.documentElement();
	const QDomElement picture = graphicsElement.firstChildElement("picture");
	node.loadSdf(picture);

	const QDomElement labels = graphicsElement.firstChildElement("labels");
	const QDomElement ports = graphicsElement.firstChildElement("ports");
	const int width = picture.attribute("sizex").toInt();
	const int height = picture.attribute("sizey").toInt();
	parseLabels(node, labels, width, height);
	parseNodePorts(node, ports, width, height);
}

void QrsMetamodelSerializer::parseLabels(NodeElementType &node, const QDomElement &labels, int width, int height)
{
	int index = 0;
	for (QDomElement labelTag = labels.firstChildElement("label")
			; !labelTag.isNull()
			; labelTag = labelTag.nextSiblingElement("label"))
	{
		const utils::ScalableCoordinate x = utils::ScalableItem::initCoordinate(labelTag.attribute("x"), width);
		const utils::ScalableCoordinate y = utils::ScalableItem::initCoordinate(labelTag.attribute("y"), height);

		const QString text = labelTag.attribute("text");
		const QString textBinded = labelTag.attribute("textBinded");
		const QString prefix = labelTag.attribute("prefix");
		const QString suffix = labelTag.attribute("suffix");
		const bool readOnly = labelTag.attribute("readOnly", "false") == "true";
		const qreal rotation = labelTag.attribute("rotation", "0").toDouble();

		++index;
		const QString background = labelTag.attribute("background", textBinded.isEmpty() ? "transparent" : "white");
		const bool isHard = labelTag.attribute("hard", "false").toLower().trimmed() == "true";
		const bool isPlainText = labelTag.attribute("isPlainText", "false").toLower().trimmed() == "true";
		if (text.isEmpty() && textBinded.isEmpty()) {
			qWarning() << "ERROR: Text or binded property of label should be non-empty; index:" << index;
			continue;
		}

		LabelProperties label;
		if (text.isEmpty()) {
			label = LabelProperties(index, x.value(), y.value(), textBinded, readOnly, rotation);
		} else {
			label = LabelProperties(index, x.value(), y.value(), text, rotation);
		}

		label.setPlainTextMode(isPlainText);
		label.setBackground(background);
		label.setScalingX(x.isScalable());
		label.setScalingY(y.isScalable());
		label.setHard(isHard);
		label.setPrefix(prefix);
		label.setSuffix(suffix);
		node.addLabel(label);
	}
}

void QrsMetamodelSerializer::parseNodePorts(NodeElementType &node, const QDomElement &portsTag, int width, int height)
{
	for (QDomElement pointPort = portsTag.firstChildElement("pointPort")
			; !pointPort.isNull()
			; pointPort = pointPort.nextSiblingElement("pointPort"))
	{
		const utils::ScalableCoordinate x = utils::ScalableItem::initCoordinate(pointPort.attribute("x"), width);
		const utils::ScalableCoordinate y = utils::ScalableItem::initCoordinate(pointPort.attribute("y"), height);
		const int initialWidth = width;
		const int initialHeight = height;
		const QString type = pointPort.attribute("type", "NonTyped");

		node.addPointPort(PointPortInfo(QPointF(x.value(), y.value()), x.isScalable(), y.isScalable()
				, initialWidth, initialHeight, type));
	}

	for (QDomElement linePort = portsTag.firstChildElement("linePort")
			; !linePort.isNull()
			; linePort = linePort.nextSiblingElement("linePort"))
	{
		const QDomElement start = linePort.firstChildElement("start");
		const QDomElement end = linePort.firstChildElement("end");

		const utils::ScalableCoordinate startX = utils::ScalableItem::initCoordinate(start.attribute("startx"), width);
		const utils::ScalableCoordinate startY = utils::ScalableItem::initCoordinate(start.attribute("starty"), width);
		const utils::ScalableCoordinate endX = utils::ScalableItem::initCoordinate(end.attribute("endx"), height);
		const utils::ScalableCoordinate endY = utils::ScalableItem::initCoordinate(end.attribute("endy"), height);
		const int initialWidth = width;
		const int initialHeight = height;
		const QString type = linePort.attribute("type", "NonTyped");

		node.addLinePort(LinePortInfo(QLineF(startX.value(), startY.value(), endX.value(), endY.value())
				, startX.isScalable(), startY.isScalable(), endX.isScalable(), endY.isScalable()
				, initialWidth, initialHeight, type));
	}
}

void QrsMetamodelSerializer::parseContainerProperties(const qrRepo::RepoApi &repo, NodeElementType &node, const Id &id)
{
	const IdList elements = repo.children(id);

	for (const Id &child : elements) {
		if (child.type() == metamodelPropertiesAsContainerType && repo.isLogicalElement(child)) {
			node.setContainer(true);
			node.setSizeOfForestalling(intVectorProperty(repo, child, "forestallingSize", {0,0,0,0}));
			node.setSizeOfChildrenForestalling(intProperty(repo, child, "childrenForestallingSize"));

			node.setSortingContainer(boolProperty(repo, child, "sortContainer"));
			node.setChildrenMovable(!boolProperty(repo, child, "banChildrenMove"));
			node.setMinimizesToChildren(boolProperty(repo, child, "minimizeToChildren"));
			node.setMaximizesChildren(boolProperty(repo, child, "maximizeChildren"));
		}
	}
}

void QrsMetamodelSerializer::parseLinksOnDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram)
{
	QHash<QPair<ElementType *, ElementType *>, QString> overridingProperties;
	QSet<ElementType *> elements;

	for (const Id &id : repo.children(diagram)) {
		const Id elementType = id.type();
		if ((elementType == metamodelNodeType || elementType == metamodelEdgeType
				|| elementType == metamodelImportType) && repo.isLogicalElement(id))
		{
			const IdList inLinks = repo.incomingLinks(id);
			for (const Id &inLink : inLinks) {
				if (!repo.isLogicalElement(inLink)) {
					continue;
				}

				if (inLink.type() == metamodelInheritanceLinkType) {
					ElementType *from = nullptr;
					ElementType *to = nullptr;
					QString overridingProperty;
					parseGeneralization(repo, metamodel, inLink, from, to, overridingProperty);
					overridingProperties[qMakePair(from, to)] = overridingProperty;
					elements.insert(from);
					elements.insert(to);
				} else if (inLink.type() == metamodelContainmentLinkType) {
					parseContainer(repo, metamodel, inLink);
				} else if (inLink.type() == metamodelExplosionLinkType) {
					parseExplosion(repo, metamodel, inLink);
				}
			}
		}
	}

	resolveInheritance(elements, overridingProperties);
}

void QrsMetamodelSerializer::parseGeneralization(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id
		, ElementType *fromElement, ElementType *toElement, QString &overridingProperties)
{
	const Id from = repo.from(id);
	const Id to = repo.to(id);
	if (from.isNull() || to.isNull()) {
		qWarning() << "Explosion" << id << "is not connected!";
		return;
	}

	fromElement = &metamodel.elementType(from);
	toElement = &metamodel.elementType(to);
	metamodel.produceEdge(*fromElement, *toElement, ElementType::generalizationLinkType);

	overridingProperties = stringProperty(repo, id, "overrides");
}

void QrsMetamodelSerializer::parseContainer(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	const Id from = repo.from(id);
	const Id to = repo.to(id);
	if (from.isNull() || to.isNull()) {
		qWarning() << "Containment link" << id << "is not connected!";
		return;
	}

	ElementType &fromElement = metamodel.elementType(from);
	ElementType &toElement = metamodel.elementType(to);
	metamodel.produceEdge(fromElement, toElement, ElementType::containmentLinkType);
}

void QrsMetamodelSerializer::parseExplosion(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	const Id from = repo.from(id);
	const Id to = repo.to(id);
	if (from.isNull() || to.isNull()) {
		qWarning() << "Explosion" << id << "is not connected!";
		return;
	}

	ElementType &fromElement = metamodel.elementType(from);
	ElementType &toElement = metamodel.elementType(to);
	metamodel.addExplosion(fromElement, toElement, boolProperty(repo, id, "makeReusable")
			, boolProperty(repo, id, "requireImmediateLinkage"));
}

void QrsMetamodelSerializer::resolveInheritance(QSet<ElementType *> &elements
		, const QHash<QPair<ElementType *, ElementType *>, QString> &overridingProperties)
{
	// Here we should copy properties, labels and so on from parent subtypes to child subtypes.
	// So we should traverse multigraph from starting from parent types, then child ones and so on.
	// The situation is a bit more complex because we may have multiple inheritance, then the child node may be
	// visited only when all parent properties are already visited.
	while (!elements.isEmpty()) {
		for (ElementType *child : elements) {
			// Checking that all parents are already visited.
			bool allParentsAreVisited = true;
			for (const qrgraph::Edge *edge : child->outgoingEdges()) {
				ElementType *parent = static_cast<ElementType *>(edge->end());
				if (elements.contains(parent)) {
					allParentsAreVisited = false;
					break;
				}
			}

			if (allParentsAreVisited) {
				elements.remove(child);
				for (const qrgraph::Edge *edge : child->outgoingEdges()) {
					ElementType *parent = static_cast<ElementType *>(edge->end());
					inherit(*child, *parent, overridingProperties[qMakePair(child, parent)]);
				}
			}
		}
	}
}

void QrsMetamodelSerializer::inherit(ElementType &child, const ElementType &parent
		, const QString &generalizationProperties)
{
	const bool overrideAll = generalizationProperties.contains("all");
	const bool overridePictures = overrideAll || generalizationProperties.contains("pictures");
	const bool overridePorts = overrideAll || generalizationProperties.contains("ports");
	const bool overrideLabels = overrideAll || generalizationProperties.contains("labels");

	if (!overrideLabels) {
		for (const LabelProperties &parentLabel : parent.labels()) {
			/// @todo: label index must be corrected here
			child.addLabel(parentLabel);
		}
	}

	if (child.type() != ElementType::Type::node || parent.type() != ElementType::Type::node) {
		return;
	}

	NodeElementType &childNode = child.toNode();
	const NodeElementType &parentNode = parent.toNode();

	if (!overridePorts) {
		for (const PointPortInfo &parentPort : parentNode.pointPorts()) {
			childNode.addPointPort(parentPort);
		}

		for (const LinePortInfo &parentPort : parentNode.linePorts()) {
			childNode.addLinePort(parentPort);
		}
	}

	if (!overridePictures) {
		childNode.loadSdf(parentNode.sdf());
	}
}

QString QrsMetamodelSerializer::validateRootNode(const qrRepo::RepoApi &repo, const Id &diagram)
{
	if (!repo.hasProperty(diagram, "nodeName")) {
		return QString();
	}

	const QString rootNode = repo.property(diagram, "nodeName").toString();
	for (const Id &child : repo.children(diagram)) {
		if (repo.name(child) == rootNode && (child.type() == metamodelNodeType || child.type() == metamodelGroupType)) {
			return rootNode;
		}
	}

	emit errorOccured(QObject::tr("Root node for diagram %1 (which is %2) does not exist!")
			.arg(repo.name(diagram)).arg(rootNode), diagram);
	return rootNode;
}

QString QrsMetamodelSerializer::validateName(const qrRepo::RepoApi &repo, const Id &id)
{
	const QString result = repo.name(id);
	if (result.isEmpty()) {
		emit errorOccured(tr("Name should not be empty"), id);
	}

	const QRegExp patten("[A-Za-z_]+([A-Za-z_0-9 :]*)");
	if (!patten.exactMatch(result)) {
		emit errorOccured(QObject::tr("Name should contain only latin letters, digits, spaces and underscores "
			"and should start with latin letter or underscore"), id);
	}

	return result;
}

QString QrsMetamodelSerializer::validatePortName(const qrRepo::RepoApi &repo, const Id &id, const QString &name)
{
	if (name == "NonTyped") {
		return name;
	}

	for (const Id &port : repo.elementsByType(metamodelPortType.element())) {
		if (repo.name(port) == name) {
			return name;
		}
	}

	emit errorOccured(tr("Port type %1 not declared in metamodel"), id);
	return "NonTyped";
}

QString QrsMetamodelSerializer::stringProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, const QString &defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	return repo.stringProperty(id, propertyName);
}

bool QrsMetamodelSerializer::boolProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, bool defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	return repo.stringProperty(id, propertyName) == "true";
}

Qt::PenStyle QrsMetamodelSerializer::parsePenStyle(const QString &penStyle, const Id &id)
{
	if (penStyle == "solidLine") {
		return Qt::SolidLine;
	} else if (penStyle == "dashLine") {
		return Qt::DashLine;
	} else if (penStyle == "dotLine") {
		return Qt::DotLine;
	}

	emit errorOccured(tr("Unknown link style type %1").arg(penStyle), id);
	return Qt::NoPen;
}

LinkShape QrsMetamodelSerializer::parseLinkShape(const QString &linkShape, const Id &id)
{
	if (linkShape == "curve") {
		return LinkShape::curve;
	} else if (linkShape == "square") {
		return LinkShape::square;
	} else if (linkShape == "broken") {
		return LinkShape::broken;
	}

	emit errorOccured(tr("Unknown link shape type %1").arg(linkShape), id);
	return LinkShape::broken;
}

int QrsMetamodelSerializer::parseInt(const QString &string, const Id &id)
{
	bool ok;
	const int result = string.toInt(&ok);
	if (!ok) {
		emit errorOccured(tr("%1 is not a valid integer number").arg(string), id);
	}

	return result;
}

int QrsMetamodelSerializer::intProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, int defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	return parseInt(repo.stringProperty(id, propertyName), id);
}

QVector<int> QrsMetamodelSerializer::intVectorProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, const QVector<int> &defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	const QStringList values = repo.stringProperty(id, propertyName).split(",", QString::SkipEmptyParts);
	QVector<int> result(values.size());
	for (int i = 0; i < values.size(); ++i) {
		result[i] = parseInt(values[i], id);
	}

	return result;
}

void QrsMetamodelSerializer::saveTo(const Metamodel &metamodel, const QString &pathToQrs)
{
	qrRepo::RepoApi repo(pathToQrs);
}

//IdList QrsMetamodelLoader::elements(const Id &diagram) const
//{
//	IdList result;
//	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
//		foreach (const Id &editor, repo->elementsByType("MetamodelDiagram")) {
//			foreach (const Id &editorChild, repo->children(editor)) {
//				if (diagram.diagram() != repo->name(editorChild) || !repo->isLogicalElement(editorChild)) {
//					continue;
//				}

//				foreach (const Id &diagramChild, repo->children(editorChild)) {
//					if (diagramChild.element() == "MetaEntityEdge") {
//						if (repo->hasProperty(diagramChild,"abstract")) {
//							if (repo->stringProperty(diagramChild, "abstract") == "true") {
//								repo->setProperty(diagramChild, "isHidden", "true");
//							}
//						}

//						if (!repo->hasProperty(diagramChild, "isHidden")) {
//							repo->setProperty(diagramChild, "isHidden", "false");
//						}

//						if (repo->stringProperty(diagramChild, "isHidden") != "true") {
//							result << Id(repo->name(editor), repo->name(editorChild), repo->name(diagramChild));
//						}
//					} else if (repo->hasProperty(diagramChild, "shape")) {
//						if (!repo->stringProperty(diagramChild, "shape").isEmpty()) {
//							if (repo->hasProperty(diagramChild,"abstract")) {
//								if (repo->stringProperty(diagramChild, "abstract") == "true") {
//									repo->setProperty(diagramChild, "isHidden", "true");
//								}
//							}

//							if (!repo->hasProperty(diagramChild, "isHidden")) {
//								repo->setProperty(diagramChild, "isHidden", "false");
//							}

//							if (repo->stringProperty(diagramChild, "isHidden") != "true") {
//								result << Id(repo->name(editor), repo->name(editorChild), repo->name(diagramChild));
//							}
//						}
//					}
//				}
//			}
//		}
//	}

//	return result;
//}


///// @todo Replace this with lambdas.
//class QrsMetamodelLoader::CheckPropertyForParent
//{
//public:
//	virtual ~CheckPropertyForParent() {}
//	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
//			, const QString &propertyName) const = 0;
//};

//class QrsMetamodelLoader::CompareProperty: public QrsMetamodelLoader::CheckPropertyForParent
//{
//public:
//	CompareProperty(QString value)
//			: mValue(value)
//	{
//	}

//	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
//			, const QString &propertyName) const
//	{
//		if (repo->name(parentProperty) == propertyName) {
//			return repo->stringProperty(parentProperty, mValue);
//		}

//		return "";
//	}

//private:
//	QString mValue;
//};

//class QrsMetamodelLoader::GetProperty: public QrsMetamodelLoader::CheckPropertyForParent
//{
//public:
//	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
//			, const QString &propertyName) const
//	{
//		if (parentProperty.element() == propertyName) {
//			return repo->name(parentProperty);
//		}

//		return "";
//	}
//};

//class QrsMetamodelLoader::HasProperty: public QrsMetamodelLoader::CheckPropertyForParent
//{
//public:
//	virtual QString stringProperty(const qrRepo::RepoApi * const repo, const Id &parentProperty
//			, const QString &propertyName) const
//	{
//		if (repo->hasProperty(parentProperty, propertyName)) {
//			return repo->name(parentProperty);
//		}

//		return "";
//	}
//};

//QStringList QrsMetamodelLoader::propertiesFromParents(const Id &id
//		, const QString &propertyName, const CheckPropertyForParent &checker) const
//{
//	QStringList result;
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id &metaId = repoAndMetaIdPair.second;
//	foreach (const Id &link , repo->incomingLinks(metaId)) {
//		if (link.element() == "Inheritance") {
//			foreach (const Id &parentProperty, repo->children(repo->otherEntityFromLink(link, metaId))) {
//				if (!repo->hasProperty(parentProperty, "isHidden")) {
//					repo->setProperty(parentProperty, "isHidden", "false");
//				}

//				if (repo->stringProperty(parentProperty, "isHidden") == "false") {
//					const QString strProperty = checker.stringProperty(repo, parentProperty, propertyName);
//					if (!strProperty.isEmpty()) {
//						result << strProperty;
//					}
//				}
//			}

//			const Id metaChildParent = repo->otherEntityFromLink(link, metaId);
//			if (metaChildParent != Id::rootId()) {
//				QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaChildParent);
//				result << propertiesFromParents(Id(repo->name(editorAndDiagramPair.first)
//						, repo->name(editorAndDiagramPair.second), repo->name(metaChildParent)), propertyName, checker);
//			}
//		}
//	}

//	return result;
//}

//QString QrsMetamodelLoader::valueOfProperty(const Id &id, const QString &propertyName, const QString &value) const
//{
//	QString valueOfProperty = "";
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	const QStringList parentsProperties =  propertiesFromParents(id, propertyName, CompareProperty(value));
//	if (!parentsProperties.isEmpty()) {
//		valueOfProperty = parentsProperties.first();
//	}

//	foreach (const Id &property, repo->children(metaId)) {
//		if (repo->name(property) == propertyName) {
//			if (repo->hasProperty(property, value)) {
//				valueOfProperty = repo->stringProperty(property, value);
//			}
//		}
//	}

//	return valueOfProperty;
//}

//QString QrsMetamodelLoader::propertyDisplayedName(const Id &id, const QString &propertyName) const
//{
//	return valueOfProperty(id, propertyName, "displayedName");
//}

//QString QrsMetamodelLoader::defaultPropertyValue(const Id &id, QString name) const
//{
//	return valueOfProperty(id, name, "defaultValue");
//}

//QString QrsMetamodelLoader::typeName(const Id &id, const QString &name) const
//{
//	return valueOfProperty(id, name, "attributeType");
//}

//Id QrsMetamodelLoader::findElementByType(const QString &type) const
//{
//	foreach (const qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
//		foreach (const Id &editor, repo->elementsByType("MetamodelDiagram")) {
//			foreach (const Id &diagram, repo->children(editor)) {
//				foreach (const Id &element, repo->children(diagram)) {
//					if (type == repo->name(element)) {
//						return Id(repo->name(editor), repo->name(diagram), repo->name(element));
//					}
//				}
//			}
//		}
//	}

//	throw Exception("No type " + type);
//}

//QString QrsMetamodelLoader::loadPlugin(const QString &pluginName)
//{
//	Q_UNUSED(pluginName);
//	return QString();
//}

//QString QrsMetamodelLoader::unloadPlugin(const QString &pluginName)
//{
//	Q_UNUSED(pluginName);
//	return QString();
//}

//QString QrsMetamodelLoader::description(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	if (repoAndMetaIdPair.first->hasProperty(repoAndMetaIdPair.second, "description")) {
//		return repoAndMetaIdPair.first->stringProperty(repoAndMetaIdPair.second, "description");
//	}

//	return "";
//}

//QIcon QrsMetamodelLoader::icon(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	QDomDocument classDoc;
//	QDomElement sdfElement;
//	if (metaId.element() == "MetaEntityEdge") {
//		sdfElement = classDoc.createElement("picture");
//		sdfElement.setAttribute("sizex", 100);
//		sdfElement.setAttribute("sizey", 60);
//		QDomElement lineElement = classDoc.createElement("line");
//		lineElement.setAttribute("fill", "#000000");
//		const QString lineType = repo->stringProperty(metaId, "lineType").remove("Line");
//		lineElement.setAttribute("stroke-style", lineType);
//		lineElement.setAttribute("stroke", "#000000");
//		lineElement.setAttribute("y1", 0);
//		lineElement.setAttribute("x1", 0);
//		lineElement.setAttribute("y2", 60);
//		lineElement.setAttribute("stroke-width", 2);
//		lineElement.setAttribute("x2", 100);
//		lineElement.setAttribute("fill-style", "solid");
//		sdfElement.appendChild(lineElement);
//	} else {
//		QDomDocument graphics;
//		graphics.setContent(repo->stringProperty(metaId, "shape"));
//		sdfElement = graphics.firstChildElement("graphics").firstChildElement("picture");
//	}

//	if (sdfElement.isNull()) {
//		return QIcon();
//	}

//	classDoc.appendChild(classDoc.importNode(sdfElement, true));
//	SdfIconEngineV2 * const engine = new SdfIconEngineV2(classDoc);
//	return QIcon(engine);
//}

//ElementType &QrsMetamodelLoader::elementType(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
////	InterpreterElementImpl * const type = new InterpreterElementImpl(repoAndMetaIdPair.first, repoAndMetaIdPair.second);
////	Q_ASSERT(type);
////	return *type;
//}

//IdList QrsMetamodelLoader::containedTypes(const Id &id) const
//{
//	IdList containedTypes;
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	foreach (const Id &link, repo->outgoingLinks(metaId)) {
//		if (link.element() == "Container") {
//			const Id metaIdTo = repo->otherEntityFromLink(link, metaId);
//			QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaIdTo);
//			containedTypes << Id(repo->name(editorAndDiagramPair.first)
//					, repo->name(editorAndDiagramPair.second), repo->name(metaIdTo));
//		}
//	}

//	return containedTypes;
//}

//bool QrsMetamodelLoader::isEnumEditable(const Id &id, const QString &name) const
//{
//	/// @todo: Support it
//	Q_UNUSED(id)
//	Q_UNUSED(name)
//	return false;
//}

//QList<QPair<QString, QString>> QrsMetamodelLoader::enumValues(const Id &id, const QString &name) const
//{
//	QList<QPair<QString, QString>> result;
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;

//	IdList listOfEnum =  repo->elementsByType("MetaEntityEnum");
//	for (const Id &currentEnum : listOfEnum) {
//		for (const Id &property: repo->children(metaId)) {
//			if (repo->name(property) == name && repo->isLogicalElement(property)) {
//				const QString nameOfEnum = repo->name(currentEnum);
//				const QString attributeType = repo->stringProperty(property, "attributeType");
//				if (nameOfEnum == attributeType) {
//					IdList children = repo->children(currentEnum);
//					for (const Id &current : children) {
//						if (repo->isLogicalElement(current)) {
//							if (repo->hasProperty(current, "valueName")) {
//								const QString valueName = repo->stringProperty(current, "valueName");
//								const QString displayedName = repo->stringProperty(current, "displayedName");
//								result.append(qMakePair(valueName, displayedName));
//							}
//						}
//					}
//				}
//			}
//		}
//	}

//	return result;
//}

//QStringList QrsMetamodelLoader::propertyNames(const Id &id) const
//{
//	QStringList result;
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	const QStringList propertiesFromParentsList = propertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
//	if (!propertiesFromParentsList.isEmpty()) {
//		result << propertiesFromParentsList;
//	}

//	foreach (const Id &idProperty, repo->children(metaId)) {
//		if (idProperty.element() == "MetaEntity_Attribute") {
//			if (!repo->hasProperty(idProperty, "isHidden")) {
//				repo->setProperty(idProperty, "isHidden", "false");
//			}

//			if (repo->stringProperty(idProperty, "isHidden") != "true") {
//				result << repo->name(idProperty);
//			}
//		}
//	}

//	return result;
//}

//bool QrsMetamodelLoader::isParentProperty(const Id &id, const QString &propertyName) const
//{
//	const QStringList propertiesFromParentsList = propertiesFromParents(id, "MetaEntity_Attribute", GetProperty());
//	return propertiesFromParentsList.contains(propertyName);
//}

//void QrsMetamodelLoader::deletePropertyInElement(qrRepo::RepoApi *repo
//		, const Id &diagram, const QString &propertyName) const
//{
//	foreach (const Id &element, repo->children(diagram)) {
//		foreach (const Id &property, repo->children(element)) {
//			if (property.element() == "MetaEntity_Attribute") {
//				if (repo->name(property) == propertyName) {
//					repo->setProperty(property, "isHidden", "true");
//					//repo->removeChild(element, property);
//				}
//			}
//		}
//	}
//}

//void QrsMetamodelLoader::deleteProperty(const QString &propertyName) const
//{
//	foreach (qrRepo::RepoApi * const repo, mEditorRepoApi.values()) {
//		foreach (const Id &editor, repo->elementsByType("MetamodelDiagram")) {
//			foreach (const Id &diagram, repo->children(editor)) {
//				if (repo->isLogicalElement(diagram)) {
//					deletePropertyInElement(repo, diagram, propertyName);
//				}
//			}
//		}
//	}
//}

//void QrsMetamodelLoader::addProperty(const Id &id, const QString &propDisplayedName) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const Id newId = Id(repoAndMetaIdPair.second.editor(), repoAndMetaIdPair.second.diagram()
//			, "MetaEntity_Attribute", QUuid::createUuid().toString());
//	repoAndMetaIdPair.first->addChild(repoAndMetaIdPair.second, newId);
//	repoAndMetaIdPair.first->setProperty(newId, "name", propDisplayedName);
//	repoAndMetaIdPair.first->setProperty(newId, "displayedName", propDisplayedName);
//	QStringList propertyNames;
//	propertyNames << propDisplayedName;
//	repoAndMetaIdPair.first->setProperty(newId, "maskedNames", propertyNames);
//	repoAndMetaIdPair.first->setProperty(newId, "isHidden", "false");
//}

//IdList QrsMetamodelLoader::elementsWithTheSameName(
//		const Id &diagram
//		, const QString &name
//		, const QString type
//		) const
//{
//	IdList result;
//	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
//	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
//	const Id diag = repoAndDiagramPair.second;

//	for (const auto &element: repo->children(diag)) {
//		if (repo->stringProperty(element, "displayedName") == name && element.element() == type
//				&& repo->isLogicalElement(element)) {
//			const QPair<Id, Id> editorAndDiagramPair = editorAndDiagram(repo, element);
//			result << Id(repo->name(editorAndDiagramPair.first), repo->name(editorAndDiagramPair.second)
//					, repo->name(element));
//		}
//	}

//	return result;
//}

//IdList QrsMetamodelLoader::propertiesWithTheSameName(
//		const Id &id
//		, const QString &propertyCurrentName
//		, const QString &propertyNewName
//		) const
//{
//	if (propertyDisplayedName(id, propertyCurrentName) == propertyNewName) {
//		return IdList();
//	}

//	IdList result;
//	const QPair<qrRepo::RepoApi*, Id> repoAndMetaIdPair = repoAndMetaId(id);
//	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;

//	for (const Id &idProperty : repo->children(metaId)) {
//		if (idProperty.element() == "MetaEntity_Attribute") {
//			if (repo->hasProperty(idProperty, "maskedNames")) {
//				if (repo->property(idProperty, "maskedNames").toStringList().contains(propertyNewName)) {
//					result << idProperty;
//				}
//			} else {
//				QStringList propertyNames;
//				propertyNames << repo->stringProperty(idProperty, "displayedName");
//				repo->setProperty(idProperty, "maskedNames", propertyNames);
//				if (repo->stringProperty(idProperty, "displayedName") == propertyNewName) {
//					result << idProperty;
//				}
//			}
//		}
//	}

//	return result;
//}

//QStringList QrsMetamodelLoader::getPropertiesInformation(const Id &id) const
//{
//	QStringList result;

//	QStringList propertyNamesList = propertyNames(id);
//	for (const QString &property: propertyNamesList) {
//		result << propertyDisplayedName(id, property);
//		result << typeName(id, property);
//		result << defaultPropertyValue(id, property);
//	}

//	return result;
//}

//QStringList QrsMetamodelLoader::getSameNamePropertyParams(const Id &propertyId, const QString &propertyName) const
//{
//	QStringList paramsList;
//	qrRepo::RepoApi *repo = nullptr;
//	foreach (qrRepo::RepoApi * const repoApi, mEditorRepoApi.values()) {
//		if (repoApi->exist(propertyId))
//			repo = repoApi;
//	}

//	paramsList << propertyName;
//	QString state = "";
//	if (repo->hasProperty(propertyId, "isHidden")) {
//		if (repo->stringProperty(propertyId, "isHidden") == "true") {
//			state = tr("Deleted");
//		}
//	}

//	if (state.isEmpty()) {
//		if (propertyName.compare(repo->stringProperty(propertyId, "displayedName")) == 0) {
//			state = tr("Existed");
//		} else {
//			state = tr("Renamed to ");
//			state += repo->stringProperty(propertyId, "displayedName");
//		}
//	}

//	paramsList << state;
//	paramsList << repo->stringProperty(propertyId, "attributeType");
//	paramsList << repo->stringProperty(propertyId, "defaultValue");
//	return paramsList;
//}

//void QrsMetamodelLoader::restoreRemovedProperty(const Id &propertyId, const QString &previousName) const
//{
//	qrRepo::RepoApi *repo = nullptr;
//	foreach (qrRepo::RepoApi *const repoApi, mEditorRepoApi.values()) {
//		if (repoApi->exist(propertyId))
//			repo = repoApi;
//	}

//	repo->setProperty(propertyId, "isHidden", "false");
//	if (repo->stringProperty(propertyId, "displayedName") != previousName) {
//		repo->setProperty(propertyId, "displayedName", previousName);
//	}
//}

//void QrsMetamodelLoader::restoreRenamedProperty(const Id &propertyId, const QString &previousName) const
//{
//	qrRepo::RepoApi *repo = nullptr;
//	for (qrRepo::RepoApi * const repoApi : mEditorRepoApi.values()) {
//		if (repoApi->exist(propertyId)) {
//			repo = repoApi;
//		}
//	}

//	if (repo) {
//		repo->setProperty(propertyId, "displayedName", previousName);
//	}
//}

//void QrsMetamodelLoader::setElementEnabled(const Id &type, bool enabled)
//{
//	Q_UNUSED(type)
//	Q_UNUSED(enabled)
//}

//QMap<QString, qrRepo::RepoApi*> QrsMetamodelLoader::listOfMetamodels() const
//{
//	return mEditorRepoApi;
//}

//void QrsMetamodelLoader::setProperty(qrRepo::RepoApi *repo, const Id &id
//		, const QString &property, const QVariant &propertyValue) const
//{
//	repo->setProperty(id, property, propertyValue);
//}

//void QrsMetamodelLoader::updateProperties(const Id &id, const QString &property, const QString &propertyType
//		, const QString &propertyDefaultValue, const QString &propertyDisplayedName) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	Id propertyMetaId;
//	foreach (const Id &propertyId, repo->children(repoAndMetaIdPair.second)) {
//		if (propertyId.element() == "MetaEntity_Attribute" && repo->name(propertyId) == property) {
//			propertyMetaId = propertyId;
//		}
//	}

//	setProperty(repo, propertyMetaId, "attributeType", propertyType);
//	setProperty(repo, propertyMetaId, "defaultValue", propertyDefaultValue);
//	QStringList propertyNames;
//	if (repo->hasProperty(propertyMetaId, "maskedNames")) {
//		propertyNames = repo->property(propertyMetaId, "maskedNames").toStringList();
//	} else {
//		propertyNames << repo->stringProperty(propertyMetaId,"displayedName");
//	}

//	if (!propertyNames.contains(propertyDisplayedName)) {
//		propertyNames << propertyDisplayedName;
//	}

//	setProperty(repo, propertyMetaId, "maskedNames", propertyNames);
//	setProperty(repo, propertyMetaId, "displayedName", propertyDisplayedName);
//}

//QString QrsMetamodelLoader::propertyNameByDisplayedName(const Id &id, const QString &displayedPropertyName) const
//{
//	QString propertyName = "";
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	foreach (const Id &property, repoAndMetaIdPair.first->children(repoAndMetaIdPair.second)) {
//		if (property.element() == "MetaEntity_Attribute"
//			&& repoAndMetaIdPair.first->stringProperty(property, "displayedName") == displayedPropertyName)
//		{
//			propertyName = repoAndMetaIdPair.first->name(property);
//		}
//	}

//	return propertyName;
//}

//IdList QrsMetamodelLoader::children(const Id &parent) const {
//	IdList result;
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(parent);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	foreach (const Id &link , repo->outgoingLinks(metaId)) {
//		if (link.element() == "Inheritance") {
//			const Id metaChild = repo->otherEntityFromLink(link, metaId);
//			QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaChild);
//			const Id child = Id(repo->name(editorAndDiagramPair.first), repo->name(editorAndDiagramPair.second)
//					, repo->name(metaChild));
//			result << child;
//			result << children(child);
//		}
//	}

//	return result;
//}

//QString QrsMetamodelLoader::shape(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	if (metaId.element() == "MetaEntityNode") {
//		return repo->stringProperty(metaId, "shape");
//	}

//	return "";
//}

//void QrsMetamodelLoader::updateShape(const Id &id, const QString &graphics) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	if (repoAndMetaIdPair.second.element() == "MetaEntityNode") {
//		repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "shape", graphics);
//	}
//}

//void QrsMetamodelLoader::resetIsHidden(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "isHidden", "false");
//}

//QString QrsMetamodelLoader::getIsHidden(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	return repo->stringProperty(metaId, "isHidden");
//}

//void QrsMetamodelLoader::deleteElement(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	repo->setProperty(metaId, "isHidden", "true");
//}

//bool QrsMetamodelLoader::isRootDiagramNode(const Id &id) const
//{
//	QPair<qrRepo::RepoApi*, Id> const repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	QPair<Id, Id> const editorAndDiagramPair = editorAndDiagram(repo, metaId);
//	return repo->stringProperty(editorAndDiagramPair.second, "nodeName") == repo->name(metaId);
//}

//void QrsMetamodelLoader::setStandartConfigurations(qrRepo::RepoApi *repo, const Id &id, const Id &parent
//		, const QString &name) const
//{
//	repo->addChild(parent, id);
//	repo->setProperty(id, "name", name);
//	repo->setProperty(id, "displayedName", name);
//	repo->setFrom(id, Id::rootId());
//	repo->setTo(id, Id::rootId());
//	repo->setProperty(id, "fromPort", 0.0);
//	repo->setProperty(id, "toPort", 0.0);
//	repo->setProperty(id, "links", IdListHelper::toVariant(IdList()));
//	repo->setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
//	repo->setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
//	repo->setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
//	repo->setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
//	repo->setProperty(id, "position", QPointF(0,0));
//	repo->setProperty(id, "configuration", QVariant(QPolygon()));
//}


//void QrsMetamodelLoader::addNodeElement(const Id &diagram, const QString &name
//		, const QString &displayedName, bool isRootDiagramNode) const
//{
//	const QString shape =
//			"<graphics>\n"
//			"    <picture sizex=\"50\" sizey=\"50\">\n"
//			"        <image y1=\"0\" name=\"\" x1=\"0\" y2=\"50\" x2=\"50\"/>\n"
//			"    </picture>\n"
//			"    <labels>\n"
//			"        <label x=\"-10\" y=\"60\" textBinded=\"name\"/>\n"
//			"    </labels>\n"
//			"    <ports>\n"
//			"        <pointPort x=\"0\" y=\"25\"/>\n"
//			"        <pointPort x=\"50\" y=\"25\"/>\n"
//			"        <pointPort x=\"25\" y=\"0\"/>\n"
//			"        <pointPort x=\"25\" y=\"50\"/>\n"
//			"    </ports>\n"
//			"    <nonResizeable/>\n"
//			"</graphics>\n"
//			;

//	QPair<qrRepo::RepoApi*, Id> const repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
//	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
//	const Id diag = repoAndDiagramPair.second;
//	const Id nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
//	repo->addChild(diag, nodeId);

//	if (isRootDiagramNode) {
//		repo->setProperty(diag, "nodeName", name);
//	}

//	repo->setProperty(nodeId, "name", name);
//	repo->setProperty(nodeId, "displayedName", displayedName);
//	repo->setProperty(nodeId, "shape", shape);
//	repo->setProperty(nodeId, "isResizeable", "true");
//	repo->setProperty(nodeId, "links", IdListHelper::toVariant(IdList()));
//	repo->setProperty(nodeId, "createChildrenFromMenu", "false");
//	repo->setProperty(nodeId, "isHidden", "false");

//	foreach (const Id &elem, repo->children(diag)) {
//		if (repo->name(elem) == "AbstractNode" && repo->isLogicalElement(elem)) {
//			const Id inheritanceLink("MetaEditor", "MetaEditor", "Inheritance", QUuid::createUuid().toString());
//			setStandartConfigurations(repo, inheritanceLink, Id::rootId(), "Inheritance");
//			repo->setFrom(inheritanceLink, elem);
//			repo->setTo(inheritanceLink, nodeId);

//			const Id containerLink("MetaEditor", "MetaEditor", "Container", QUuid::createUuid().toString());
//			setStandartConfigurations(repo, containerLink, Id::rootId(), "Container");
//			repo->setFrom(containerLink, nodeId);
//			repo->setTo(containerLink, elem);
//		}
//	}
//}

//void QrsMetamodelLoader::addEdgeElement(const Id &diagram, const QString &name
//		, const QString &displayedName, const QString &labelText, const QString &labelType
//		, const QString &lineType, const QString &beginType, const QString &endType) const
//{
//	const QPair<qrRepo::RepoApi*, Id> repoAndDiagramPair = repoAndDiagram(diagram.editor(), diagram.diagram());
//	qrRepo::RepoApi * const repo = repoAndDiagramPair.first;
//	const Id diag = repoAndDiagramPair.second;
//	Id edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge", QUuid::createUuid().toString());
//	repo->addChild(diag, edgeId);
//	Id associationId("MetaEditor", "MetaEditor", "MetaEntityAssociation", QUuid::createUuid().toString());
//	repo->addChild(edgeId, associationId);

//	repo->setProperty(edgeId, "name", name);
//	repo->setProperty(edgeId, "displayedName", displayedName);
//	repo->setProperty(edgeId, "labelText", labelText);
//	repo->setProperty(edgeId, "labelType", labelType);
//	repo->setProperty(edgeId, "lineType", lineType);
//	repo->setProperty(edgeId, "links", "");
//	repo->setProperty(edgeId, "shape", "broken");
//	repo->setProperty(edgeId, "isHidden", "false");

//	repo->setProperty(associationId, "name", name + "Association");
//	repo->setProperty(associationId, "beginType", beginType);
//	repo->setProperty(associationId, "endType", endType);
//}

//void QrsMetamodelLoader::updateGenerationRule(const Id &id, const QString &newRule) const
//{
//	const QPair<qrRepo::RepoApi*, Id> repoAndMetaIdPair = repoAndMetaId(id);
//	if (repoAndMetaIdPair.second.element() == "MetaEntityNode") {
//		repoAndMetaIdPair.first->setProperty(repoAndMetaIdPair.second, "generationRule", newRule);
//	}
//}

//QString QrsMetamodelLoader::generationRule(const Id &id) const
//{
//	const QPair<qrRepo::RepoApi*, Id> repoAndMetaIdPair = repoAndMetaId(id);
//	const qrRepo::RepoApi * const repo = repoAndMetaIdPair.first;
//	const Id metaId = repoAndMetaIdPair.second;
//	if (metaId.element() == "MetaEntityNode") {
//		return repo->stringProperty(metaId, "generationRule");
//	}

//	return "";
//}

//QPair<Id, Id> QrsMetamodelLoader::createEditorAndDiagram(const QString &name) const
//{
//	const Id editor("MetaEditor", "MetaEditor", "MetamodelDiagram", QUuid::createUuid().toString());
//	const Id diagram("MetaEditor", "MetaEditor", "MetaEditorDiagramNode", QUuid::createUuid().toString());
//	qrRepo::RepoApi * const repo = mEditorRepoApi.value("test");
//	repo->addChild(Id::rootId(), editor);
//	repo->setProperty(editor, "name", name);
//	repo->setProperty(editor, "displayedName", name);
//	repo->addChild(editor, diagram);
//	repo->setProperty(diagram, "name", name);
//	repo->setProperty(diagram, "displayedName", name);
//	repo->setProperty(diagram, "nodeName", name);
//	const Id nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
//	repo->addChild(diagram, nodeId);
//	repo->setProperty(nodeId, "name", "AbstractNode");
//	repo->setProperty(nodeId, "displayedName", "AbstractNode");
//	repo->setProperty(nodeId, "shape", "");
//	repo->setProperty(nodeId, "links", IdListHelper::toVariant(IdList()));
//	repo->setProperty(nodeId, "isResizeable", "true");
//	const Id containerLink("MetaEditor", "MetaEditor", "Container", QUuid::createUuid().toString());
//	setStandartConfigurations(repo, containerLink, Id::rootId(), "Container");
//	repo->setFrom(containerLink, nodeId);
//	repo->setTo(containerLink, nodeId);
//	return qMakePair(Id(repo->name(editor)), Id(repo->name(editor), repo->name(diagram)));
//}
