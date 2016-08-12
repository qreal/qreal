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

#include "qrsMetamodelLoader.h"

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

QList<Metamodel *> QrsMetamodelLoader::load(const QString &pathToQrs)
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

Metamodel *QrsMetamodelLoader::parseMetamodel(const qrRepo::RepoApi &repo, const Id &metamodelDiagram)
{
	Metamodel *result = new Metamodel();
	result->setId(validateName(repo, metamodelDiagram));
	result->setFriendlyName(stringProperty(repo, metamodelDiagram, "displayedName"));
	result->setVersion(stringProperty(repo, metamodelDiagram, "version"));

	for (const Id &id : repo.children(metamodelDiagram)) {
		if (id.type() == metamodelDiagramType && repo.isLogicalElement(id)) {
			parseDiagram(repo, *result, id);
		}
	}

	return result;
}

void QrsMetamodelLoader::parseDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram)
{
	const QString diagramName = validateName(repo, diagram);
	metamodel.addDiagram(diagramName);

	metamodel.setDiagramFriendlyName(diagramName, stringProperty(repo, diagram, "displayedName"));
	metamodel.setDiagramNode(diagramName, validateRootNode(repo, diagram));

	parseObjectsOnDiagram(repo, metamodel, diagram);
	parseLinksOnDiagram(repo, metamodel, diagram);
}

void QrsMetamodelLoader::parseObjectsOnDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram)
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

void QrsMetamodelLoader::parseImport(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(metamodel)
	Q_UNUSED(id)
}

void QrsMetamodelLoader::parseNode(const qrRepo::RepoApi &repo
		, Metamodel &metamodel, const Id &diagram, const Id &id)
{
	NodeElementType *node = new NodeElementType(metamodel);

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

	metamodel.addElement(*node);
}

void QrsMetamodelLoader::parseEdge(const qrRepo::RepoApi &repo
		, Metamodel &metamodel, const Id &diagram, const Id &id)
{
	EdgeElementType *edge = new EdgeElementType(metamodel);

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
		if (labelType.contains("static", Qt::CaseInsensitive)) {
			label = LabelProperties(0, 0, 0, labelText, 0);
		} else if (labelType.contains("dynamic", Qt::CaseInsensitive)) {
			label = LabelProperties(0, 0, 0, labelText, false, 0);
		} else {
			emit errorOccured(tr("Incorrect label type"), id);
		}

		label.setHard(boolProperty(repo, id, "hardLabel"));
		edge->addLabel(label);
	}

	parseAssociations(repo, *edge, id);
	parseProperties(repo, *edge, id);
	parsePorts(repo, *edge, id, "from");
	parsePorts(repo, *edge, id, "to");

	metamodel.addElement(*edge);
}

void QrsMetamodelLoader::parseEnum(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	const QString enumName = validateName(repo, id);
	metamodel.addEnum(enumName, parseEnumValues(repo, id));
	metamodel.setEnumEditable(enumName, boolProperty(repo, id, "editable"));
}

void QrsMetamodelLoader::parsePort(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(metamodel)
	Q_UNUSED(id)
}

void QrsMetamodelLoader::parseGroup(const qrRepo::RepoApi &repo
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

void QrsMetamodelLoader::parseProperties(const qrRepo::RepoApi &repo, ElementType &element, const Id &id)
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

void QrsMetamodelLoader::parsePorts(const qrRepo::RepoApi &repo
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

QList<QPair<QString, QString>> QrsMetamodelLoader::parseEnumValues(const qrRepo::RepoApi &repo, const Id &id)
{
	QList<QPair<QString, QString>> result;
	for (const Id &child : repo.children(id)) {
		if (child.type() == metamodelEnumValueType && repo.isLogicalElement(child)) {
			result << qMakePair(validateName(repo, child), stringProperty(repo, child, "displayedName"));
		}
	}

	return result;
}

void QrsMetamodelLoader::parseGroupNodes(const qrRepo::RepoApi &repo, QDomElement &parent, const Id &id)
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

void QrsMetamodelLoader::parseAssociations(const qrRepo::RepoApi &repo, EdgeElementType &edge, const Id &id)
{
	Q_UNUSED(repo)
	Q_UNUSED(edge)
	Q_UNUSED(id)
	/// @todo: This method should set shape`s begin and end types. For now it is only implemented in generated mode.
	/// Drawind code should be migrated from qrxc to engine and then here we simply set generate types as-is.
}

void QrsMetamodelLoader::parseSdfGraphics(const qrRepo::RepoApi &repo, NodeElementType &node, const Id &id)
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
	node.setSize(QSizeF(width, height));
	parseLabels(node, labels, width, height);
	parseNodePorts(node, ports, width, height);
}

void QrsMetamodelLoader::parseLabels(NodeElementType &node, const QDomElement &labels, int width, int height)
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

void QrsMetamodelLoader::parseNodePorts(NodeElementType &node, const QDomElement &portsTag, int width, int height)
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

	for (QDomElement circlePort = portsTag.firstChildElement("circularPort")
			; !circlePort.isNull()
			; circlePort = circlePort.nextSiblingElement("circularPort"))
	{
		const utils::ScalableCoordinate x = utils::ScalableItem::initCoordinate(circlePort.attribute("x"), width);
		const utils::ScalableCoordinate y = utils::ScalableItem::initCoordinate(circlePort.attribute("y"), height);
		const qreal r = circlePort.attribute("r").toDouble();
		const int initialWidth = width;
		const int initialHeight = height;
		const QString type = circlePort.attribute("type", "NonTyped");

		node.addCircularPort(CircularPortInfo(QPointF(x.value(), y.value()), r, x.isScalable(), y.isScalable()
				, initialWidth, initialHeight, type));
	}
}

void QrsMetamodelLoader::parseContainerProperties(const qrRepo::RepoApi &repo, NodeElementType &node, const Id &id)
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

void QrsMetamodelLoader::parseLinksOnDiagram(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &diagram)
{
	QHash<QPair<ElementType *, ElementType *>, QString> overridingProperties;
	QSet<ElementType *> elements;
	const QString diagramName = validateName(repo, diagram);

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
					parseGeneralization(repo, metamodel, inLink, diagramName, from, to, overridingProperty);
					overridingProperties[qMakePair(from, to)] = overridingProperty;
					elements.insert(from);
					elements.insert(to);
				} else if (inLink.type() == metamodelContainmentLinkType) {
					parseContainer(repo, metamodel, inLink, diagramName);
				} else if (inLink.type() == metamodelExplosionLinkType) {
					parseExplosion(repo, metamodel, inLink, diagramName);
				}
			}
		}
	}

	resolveInheritance(elements, overridingProperties);
}

void QrsMetamodelLoader::parseGeneralization(const qrRepo::RepoApi &repo, Metamodel &metamodel, const Id &id
		, const QString &diagram, ElementType *&fromElement, ElementType *&toElement, QString &overridingProperties)
{
	const Id from = repo.from(id);
	const Id to = repo.to(id);
	if (from.isNull() || to.isNull()) {
		qWarning() << "Generalization" << id << "is not connected!";
		return;
	}

	const QString fromName = validateName(repo, from);
	const QString toName = validateName(repo, to);
	fromElement = &metamodel.elementType(diagram, fromName);
	toElement = &metamodel.elementType(diagram, toName);
	metamodel.produceEdge(*toElement, *fromElement, ElementType::generalizationLinkType);

	overridingProperties = stringProperty(repo, id, "overrides");
}

void QrsMetamodelLoader::parseContainer(const qrRepo::RepoApi &repo, Metamodel &metamodel
		, const Id &id, const QString &diagram)
{
	const Id from = repo.from(id);
	const Id to = repo.to(id);
	if (from.isNull() || to.isNull()) {
		qWarning() << "Containment link" << id << "is not connected!";
		return;
	}

	const QString fromName = validateName(repo, from);
	const QString toName = validateName(repo, to);
	ElementType &fromElement = metamodel.elementType(diagram, fromName);
	ElementType &toElement = metamodel.elementType(diagram, toName);
	metamodel.produceEdge(fromElement, toElement, ElementType::containmentLinkType);
}

void QrsMetamodelLoader::parseExplosion(const qrRepo::RepoApi &repo, Metamodel &metamodel
		, const Id &id, const QString &diagram)
{
	const Id from = repo.from(id);
	const Id to = repo.to(id);
	if (from.isNull() || to.isNull()) {
		qWarning() << "Explosion" << id << "is not connected!";
		return;
	}

	const QString fromName = validateName(repo, from);
	const QString toName = validateName(repo, to);
	ElementType &fromElement = metamodel.elementType(diagram, fromName);
	ElementType &toElement = metamodel.elementType(diagram, toName);
	metamodel.addExplosion(fromElement, toElement, boolProperty(repo, id, "makeReusable")
			, boolProperty(repo, id, "requireImmediateLinkage"));
}

void QrsMetamodelLoader::resolveInheritance(QSet<ElementType *> &elements
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
			for (const qrgraph::Edge *edge : child->outgoingEdges(ElementType::generalizationLinkType)) {
				ElementType *parent = static_cast<ElementType *>(edge->end());
				if (elements.contains(parent)) {
					allParentsAreVisited = false;
					break;
				}
			}

			if (allParentsAreVisited) {
				elements.remove(child);
				for (const qrgraph::Edge *edge : child->outgoingEdges(ElementType::generalizationLinkType)) {
					ElementType *parent = static_cast<ElementType *>(edge->end());
					inherit(*child, *parent, overridingProperties[qMakePair(child, parent)]);
				}

				break;
			}
		}
	}
}

void QrsMetamodelLoader::inherit(ElementType &child, const ElementType &parent
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

QString QrsMetamodelLoader::validateRootNode(const qrRepo::RepoApi &repo, const Id &diagram)
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

QString QrsMetamodelLoader::validateName(const qrRepo::RepoApi &repo, const Id &id)
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

QString QrsMetamodelLoader::validatePortName(const qrRepo::RepoApi &repo, const Id &id, const QString &name)
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

QString QrsMetamodelLoader::stringProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, const QString &defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	return repo.stringProperty(id, propertyName);
}

bool QrsMetamodelLoader::boolProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, bool defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	return repo.stringProperty(id, propertyName) == "true";
}

Qt::PenStyle QrsMetamodelLoader::parsePenStyle(const QString &penStyle, const Id &id)
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

LinkShape QrsMetamodelLoader::parseLinkShape(const QString &linkShape, const Id &id)
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

int QrsMetamodelLoader::parseInt(const QString &string, const Id &id)
{
	bool ok;
	const int result = string.toInt(&ok);
	if (!ok) {
		emit errorOccured(tr("%1 is not a valid integer number").arg(string), id);
	}

	return result;
}

int QrsMetamodelLoader::intProperty(const qrRepo::RepoApi &repo, const Id &id
		, const QString &propertyName, int defaultValue)
{
	if (!repo.hasProperty(id, propertyName)) {
		return defaultValue;
	}

	return parseInt(repo.stringProperty(id, propertyName), id);
}

QVector<int> QrsMetamodelLoader::intVectorProperty(const qrRepo::RepoApi &repo, const Id &id
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
