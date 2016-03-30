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
