/* Copyright 2012-2016 QReal Research Group
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

#include "interpreterElementImpl.h"

#include <qrutils/outFile.h>
#include <qrutils/scalableItem.h>

using namespace qReal;
using namespace utils;

InterpreterElementImpl::InterpreterElementImpl(Metamodel &metamodel, qrRepo::RepoApi *repo, const Id &metaId)
	: ElementType(metamodel)
	, mMetamodel(metamodel)
	, mEditorRepoApi(repo)
	, mId(metaId)
{
}

//void InterpreterElementImpl::initLabels(const int &width, const int &height, LabelFactoryInterface &factory
//		, QList<LabelInterface*> &titles)
//{
//	int index = 0;
//	for (QDomElement element
//			= mGraphics.firstChildElement("graphics").firstChildElement("labels").firstChildElement("label");
//			!element.isNull();
//			element = element.nextSiblingElement("label"))
//	{
//		const ScalableCoordinate x = utils::ScalableItem::initCoordinate(element.attribute("x"), width);
//		const ScalableCoordinate y = utils::ScalableItem::initCoordinate(element.attribute("y"), height);
//		const QString center = element.attribute("center", "false");
//		const QString text = element.attribute("text");
//		const QString textBinded = element.attribute("textBinded");
//		const QString readOnly = element.attribute("readOnly", "false");
//		const QString background = element.attribute("background", "transparent");
//		const qreal rotation = element.attribute("rotation", "0").toDouble();
//		if (text.isEmpty() && textBinded.isEmpty()) {
//			qDebug() << "ERROR: can't parse label";
//		} else {
//			LabelInterface *title = nullptr;
//			if (text.isEmpty()) {
//				// It is a binded label, text for it will be taken from repository.
//				title = factory.createLabel(index, x.value(), y.value(), textBinded, readOnly == "true", rotation);
//			} else {
//				// This is a statical label, it does not need repository.
//				title = factory.createLabel(index, x.value(), y.value(), text, rotation);
//			}

//			title->setBackground(QColor(background));
//			title->setScaling(x.isScalable(), y.isScalable());
//			title->setFlags(0);
//			title->setTextInteractionFlags(Qt::NoTextInteraction);
//			titles.append(title);
//			mNodeLabels.append(NodeLabel(textBinded, center, title));
//			++index;
//		}
//	}
//}

//void InterpreterElementImpl::initPointPorts(const PortFactoryInterface &factory, QList<PortInterface *> &ports
//		, const int &width, const int &height)
//{
//	const QDomNodeList pointPortsList
//			= mGraphics.firstChildElement("graphics").firstChildElement("ports").elementsByTagName("pointPort");
//	for (int i = 0; i < pointPortsList.size(); i++) {
//		QDomElement portElement = pointPortsList.at(i).toElement();

//		QString x = portElement.attribute("x");
//		bool propX = false;
//		if (x.endsWith("a")) {
//			propX = true;
//			x.chop(1);
//		}

//		QString y = portElement.attribute("y");
//		bool propY = false;
//		if (y.endsWith("a")) {
//			propY = true;
//			y.chop(1);
//		}

//		QPointF point = QPointF(x.toDouble() / static_cast<qreal>(width), y.toDouble() / static_cast<qreal>(height));

//		QString portType = portElement.attribute("type", "NonTyped");
//		ports << factory.createPort(point, propX, propY, width, height, new InterpreterPortImpl(portType));
//	}
//}

//void InterpreterElementImpl::initLinePorts(const PortFactoryInterface &factory, QList<PortInterface *> &ports
//		, const int &width, const int &height)
//{
//	const QDomNodeList linePortsList
//			= mGraphics.firstChildElement("graphics").firstChildElement("ports").elementsByTagName("linePort");

//	for (int i = 0; i < linePortsList.size(); i++) {

//		QString x1 = linePortsList.at(i).firstChildElement("start").attribute("startx");
//		bool propX1 = false;
//		if (x1.endsWith("a")) {
//			propX1 = true;
//			x1.chop(1);
//		}

//		QString y1 = linePortsList.at(i).firstChildElement("start").attribute("starty");
//		bool propY1 = false;
//		if (y1.endsWith("a")) {
//			propY1 = true;
//			y1.chop(1);
//		}

//		QString x2 = linePortsList.at(i).firstChildElement("end").attribute("endx");
//		bool propX2 = false;
//		if (x2.endsWith("a")) {
//			propX2 = true;
//			x2.chop(1);
//		}

//		QString y2 = linePortsList.at(i).firstChildElement("end").attribute("endy");
//		bool propY2 = false;
//		if (y2.endsWith("a")) {
//			propY2 = true;
//			y2.chop(1);
//		}

//		QLineF line = QLineF(x1.toDouble() / static_cast<qreal>(width)
//				, y1.toDouble() / static_cast<qreal>(height)
//				, x2.toDouble() / static_cast<qreal>(width)
//				, y2.toDouble() / static_cast<qreal>(height));

//		QString portType = linePortsList.at(i).toElement().attribute("type", "NonTyped");
//		ports << factory.createPort(line, propX1, propY1, propX2, propY2, width, height
//				, new InterpreterPortImpl(portType));
//	}
//}

//void InterpreterElementImpl::inheritProperties(QList<QDomElement> &elements, const Id &id
//		, QList<PortInterface *> &ports, const PortFactoryInterface &portFactory, SdfRendererInterface *renderer
//		, LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels) const
//{
//	bool overridePictures = false;
//	bool overridePorts = false;
//	bool overrideLabels = false;
//	for (const Id &link : mEditorRepoApi->incomingLinks(id)) {
//		overridePictures = false;
//		overridePorts = false;
//		overrideLabels = false;
//		if (link.element() == "Inheritance") {
//			const Id &parent = mEditorRepoApi->otherEntityFromLink(link, id);
//			if (!parent.isNull()) {
//				inheritProperties(elements, parent, ports, portFactory, renderer, labelFactory, labels);
//				InterpreterElementImpl impl(mMetamodel, mEditorRepoApi, parent);
//				impl.mGraphics.setContent(mEditorRepoApi->stringProperty(parent, "shape"));
//				QDomElement sdfElement = impl.mGraphics.firstChildElement("graphics").firstChildElement("picture");

//				int width = 0;
//				int height = 0;
//				if (!sdfElement.isNull()) {
//					width = sdfElement.attribute("sizex").toInt();
//					height = sdfElement.attribute("sizey").toInt();
//				}

//				if (mEditorRepoApi->stringProperty(link, "overrides").contains("all")) {
//					overridePictures = true;
//					overridePorts = true;
//					overrideLabels = true;
//				}

//				if (mEditorRepoApi->stringProperty(link, "overrides").contains("pictures")) {
//					overridePictures = true;
//				}

//				if (mEditorRepoApi->stringProperty(link, "overrides").contains("ports")) {
//					overridePorts = true;
//				}

//				if (mEditorRepoApi->stringProperty(link, "overrides").contains("labels")) {
//					overrideLabels = true;
//				}

//				if (!overridePictures && (mEditorRepoApi->stringProperty(link, "overrides").isEmpty()
//						|| mEditorRepoApi->stringProperty(link, "overrides") != "pictures"))
//				{
//					elements.append(sdfElement);
//				}

//				if (!overridePorts && (mEditorRepoApi->stringProperty(link, "overrides").isEmpty()
//						|| mEditorRepoApi->stringProperty(link, "overrides") != "ports"))
//				{
//					impl.initPointPorts(portFactory, ports, width, height);
//				}

//				if (!overrideLabels && (mEditorRepoApi->stringProperty(link, "overrides").isEmpty()
//						|| mEditorRepoApi->stringProperty(link, "overrides") != "labels"))
//				{
//					impl.initLabels(width, height, labelFactory, labels);
//				}
//			}
//		}
//	}
//}

//void InterpreterElementImpl::init(QRectF &contents, const PortFactoryInterface &portFactory
//		, QList<PortInterface *> &ports, LabelFactoryInterface &labelFactory
//		, QList<LabelInterface *> &labels, SdfRendererInterface *renderer, ElementRepoInterface *elementRepo)
//{
//	Q_UNUSED(elementRepo);
//	if (mId.element() == "MetaEntityNode") {
//		mGraphics.setContent(mEditorRepoApi->stringProperty(mId, "shape"));
//		QDomDocument classDoc;
//		QDomElement sdfElement = mGraphics.firstChildElement("graphics").firstChildElement("picture");

//		QList<QDomElement> elementsWithGraphic;
//		inheritProperties(elementsWithGraphic, mId, ports, portFactory, renderer, labelFactory, labels);

//		for (QDomElement tempElementWithGraphic : elementsWithGraphic) {
//			sdfElement.appendChild(tempElementWithGraphic.firstChild());
//		}

//		classDoc.appendChild(classDoc.importNode(sdfElement, true));
//		if (!classDoc.childNodes().isEmpty()) {
//			mRenderer = renderer;
//			mRenderer->load(classDoc);
//		}

//		int width = 0;
//		int height = 0;
//		if (!sdfElement.isNull()) {
//			width = sdfElement.attribute("sizex").toInt();
//			height = sdfElement.attribute("sizey").toInt();
//		}

//		initPointPorts(portFactory, ports, width, height);
//		initLinePorts(portFactory, ports, width, height);

//		contents.setWidth(width);
//		contents.setHeight(height);
//		initLabels(width, height, labelFactory, labels);
//	}
//}

//void InterpreterElementImpl::init(LabelFactoryInterface &labelFactory, QList<LabelInterface *> &labels)
//{
//	if (mId.element() == "MetaEntityEdge") {
//		QString labelText = mEditorRepoApi->stringProperty(mId, "labelText");
//		if (!labelText.isEmpty()) {
//			const QString labelType = mEditorRepoApi->stringProperty(mId, "labelType");
//			LabelInterface* title = nullptr;
//			if (labelType == "staticText") {
//				// This is a statical label, it does not need repository.
//				title = labelFactory.createLabel(0, 0, 0, labelText, 0);
//			} else {
//				// It is a binded label, text for it will be taken from repository.
//				title = labelFactory.createLabel(0, 0, 0, labelText, false, 0);
//			}

//			title->setBackground(QColor(Qt::white));
//			title->setScaling(false, false);
//			title->setFlags(0);
//			title->setTextInteractionFlags(Qt::NoTextInteraction);
//			labels.append(title);
//			mEdgeLabels.append(EdgeLabel(labelText, labelType, title));
//		}
//	}
//}

//void InterpreterElementImpl::paint(QPainter *painter, QRectF &contents)
//{
//	if (mId.element() == "MetaEntityNode") {
//		if(!mGraphics.childNodes().isEmpty()) {
//			mRenderer->render(painter, contents);
//		}
//	}
//}

QStringList InterpreterElementImpl::getListOfStr(const QString &labelText) const
{
	QStringList reformedList = labelText.split("##");
	QStringList list;
	int counter = 1;
	foreach (const QString &str, reformedList) {
		list.append(str);
		counter++;
	}

	return list;
}

QString InterpreterElementImpl::getResultStr(const QStringList &list, ElementRepoInterface *repo) const
{
	QString resultStr;
	if (list.count() == 1) {
		if (list.first() == "name") {
			resultStr = repo->name();
		} else {
			resultStr = repo->logicalProperty(list.first());
		}
	} else {
		int counter = 1;
		foreach (const QString &listElement, list) {
			QString field;
			if (counter % 2 == 0) {
				if (listElement == "name") {
					field = repo->name();
				} else {
					field = repo->logicalProperty(listElement);
				}
			} else {
				field = listElement;
			}

			resultStr += field;
			counter++;
		}
	}
	return resultStr;
}

//void InterpreterElementImpl::updateData(ElementRepoInterface *repo) const
//{
//	if (mId.element() == "MetaEntityEdge") {
//		foreach (EdgeLabel edgeLabel, mEdgeLabels) {
//			if (edgeLabel.labelType == "staticText") {
//				// Static label
//				return;
//			}

//			const QStringList list = getListOfStr(edgeLabel.labelText);
//			const QString resultStr = getResultStr(list, repo);
//			edgeLabel.title->setHtml(QString("<center>%1</center>").arg(resultStr).replace("\n", "<br>"));
//		}
//	}

//	if (mId.element() == "MetaEntityNode") {
//		foreach (const NodeLabel &nodeLabel, mNodeLabels) {
//			if (nodeLabel.textBinded.isEmpty()) {
//				// Static label
//				Q_UNUSED(repo);
//				return;
//			}

//			const QStringList list = getListOfStr(nodeLabel.textBinded);
//			const QString resultStr = getResultStr(list, repo);
//			nodeLabel.title->setHtml(QString(nodeLabel.center == "true"
//					? "<center>%1</center>" : "<b>%1</b>").arg(resultStr).replace("\n", "<br>"));
//		}
//	}
//}

bool InterpreterElementImpl::isNode() const
{
	return mId.element() == "MetaEntityNode";
}

bool InterpreterElementImpl::isResizeable() const
{
	if (mId.element() == "MetaEntityNode") {
		return mEditorRepoApi->stringProperty(mId, "isResizeable") == "true";
	}

	return true;
}

Qt::PenStyle InterpreterElementImpl::getPenStyle() const
{
	if (mId.element() == "MetaEntityEdge") {
		const QString QtStyle = "Qt::" + mEditorRepoApi->stringProperty(mId, "lineType").replace(0, 1
				, mEditorRepoApi->stringProperty(mId, "lineType").at(0).toUpper());
		if (QtStyle != "") {
			if (QtStyle == "Qt::NoPen") {
				return Qt::NoPen;
			} else if (QtStyle == "Qt::SolidLine") {
				return Qt::SolidLine;
			} else if (QtStyle == "Qt::DashLine") {
				return Qt::DashLine;
			} else if (QtStyle == "Qt::DotLine") {
				return Qt::DotLine;
			} else if (QtStyle == "Qt::DashDotLine") {
				return Qt::DashDotLine;
			} else if (QtStyle == "Qt::DashDotDotLine") {
				return Qt::DashDotDotLine;
			} else if (QtStyle == "Qt::CustomDashLine") {
				return Qt::CustomDashLine;
			}
		}
	}

	return Qt::SolidLine;
}

int InterpreterElementImpl::getPenWidth() const
{
	if (mId.element() != "MetaEntityEdge") {
		return 0;
	}

	const QDomElement lineWidthElement = mGraphics.firstChildElement("lineWidth");
	if (lineWidthElement.isNull()) {
		return 1;
	}

	const QString lineWidth = lineWidthElement.attribute("width");
	if (lineWidth.isEmpty()) {
		return 0;
	}

	bool success = true;
	const int lineWidthInt = lineWidth.toInt(&success);
	if (!success) {
		return 0;
	} else if (lineWidthInt <= 0) {
		return 0;
	} else {
		return lineWidthInt;
	}
}

QColor InterpreterElementImpl::getPenColor() const
{
	// TODO: Not too colorful.
	return QColor();
}

void InterpreterElementImpl::drawArrow(QPainter *painter, const QString &type) const
{
	if (mId.element() != "MetaEntityEdge") {
		return;
	}

	QString style = "";
	foreach (const Id &edgeChild, mEditorRepoApi->children(mId)) {
		if (edgeChild.element() == "MetaEntityAssociation") {
			 style = mEditorRepoApi->stringProperty(edgeChild, type);
		}
	}

	if (style.isEmpty()) {
		style = "filled_arrow";
	}

	const QBrush oldBrush = painter->brush();
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);

	if (style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow") {
		brush.setColor(Qt::white);
	} else if (style == "filled_arrow" || style == "filled_rhomb") {
		brush.setColor(Qt::black);
	}

	painter->setBrush(brush);

	if (style == "empty_arrow" || style == "filled_arrow") {
		static const QPointF points[] = {QPointF(0, 0), QPointF(-5, 10), QPointF(5, 10)};
		painter->drawPolygon(points, 3);
	} else if (style == "empty_rhomb" || style == "filled_rhomb") {
		static const QPointF points[] = {QPointF(0, 0), QPointF(-5, 10), QPointF(0, 20), QPointF(5, 10)};
		painter->drawPolygon(points, 4);
	} else if (style == "open_arrow") {
		static const QPointF points[] = {QPointF(-5, 10), QPointF(0, 0), QPointF(5, 10)};
		painter->drawPolyline(points, 3);
	} else if (style == "complex_arrow") {
		static const QPointF points[] = {QPointF(-15, 30), QPointF(-10, 10), QPointF(0, 0), QPointF(10, 10)
				, QPointF(15, 30), QPointF(0, 23), QPointF(-15, 30)};
		painter->drawPolyline(points, 7);
	}

	painter->setBrush(oldBrush);
}

void InterpreterElementImpl::drawStartArrow(QPainter *painter) const
{
	drawArrow(painter, "beginType");
}

void InterpreterElementImpl::drawEndArrow(QPainter *painter) const
{
	drawArrow(painter, "endType");
}

bool InterpreterElementImpl::isDividable() const
{
	return true;
}

bool InterpreterElementImpl::hasContainerProperty(const QString &property) const
{
	const QDomElement propertiesElement =
			mGraphics.firstChildElement("logic").firstChildElement("container").firstChildElement("properties");

	if (propertiesElement.hasChildNodes()) {
		if (!propertiesElement.firstChildElement(property).isNull()) {
			return true;
		}
	}

	return false;
}

bool InterpreterElementImpl::isContainer() const
{
	return hasContainerProperty("container");
}

bool InterpreterElementImpl::isSortingContainer() const
{
	return hasContainerProperty("sortContainer");
}

QVector<int> InterpreterElementImpl::getSizeOfContainerProperty(const QString &property) const
{
	QVector<int> size(4, 0);
	const QDomElement propertiesElement =
			mGraphics.firstChildElement("logic").firstChildElement("container").firstChildElement("properties");
	if (propertiesElement.hasChildNodes()) {
		if (!propertiesElement.firstChildElement(property).isNull()) {
			const QStringList sizeStr = propertiesElement.firstChildElement(property).attribute("size").split(',');
			for (int i = 0; i < sizeStr.size(); i++) {
				size[i] = sizeStr[i].toInt();
			}
		}
	}

	return size;
}
QVector<int> InterpreterElementImpl::sizeOfForestalling() const
{
	return getSizeOfContainerProperty("forestallingSize");
}

int InterpreterElementImpl::sizeOfChildrenForestalling() const
{
	return getSizeOfContainerProperty("childrenForestallingSize")[0];
}

bool InterpreterElementImpl::hasMovableChildren() const
{
	return hasContainerProperty("banChildrenMove");
}

bool InterpreterElementImpl::minimizesToChildren() const
{
	return hasContainerProperty("minimizeToChildren");
}

bool InterpreterElementImpl::maximizesChildren() const
{
	return mEditorRepoApi->stringProperty(mId, "maximizeChildren") == "true";
}

QStringList InterpreterElementImpl::fromPortTypes() const
{
	return QStringList("NonTyped");
}

QStringList InterpreterElementImpl::toPortTypes() const
{
	return QStringList("NonTyped");
}

LinkShape InterpreterElementImpl::shapeType() const
{
	QString shape = "";
	if (mEditorRepoApi->hasProperty(mId, "shape")) {
		shape = mEditorRepoApi->stringProperty(mId, "shape");
	}

	return shapeTypeByString(shape);
}

LinkShape InterpreterElementImpl::shapeTypeByString(const QString &type) const
{
	if (type == "broken") {
		return LinkShape::broken;
	} else if (type == "curve") {
		return LinkShape::curve;
	} else {
		return LinkShape::square;
	}
}

bool InterpreterElementImpl::createChildrenFromMenu() const
{
	if (mEditorRepoApi->hasProperty(mId, "createChildrenFromMenu")) {
		return mEditorRepoApi->stringProperty(mId, "createChildrenFromMenu") == "true";
	}

	return false;
}

QList<qreal> InterpreterElementImpl::border() const
{
	QList<qreal> list;
	if (mEditorRepoApi->stringProperty(mId, "isAction") == "true") {
		list << 30 << 15 << 15 << 25;
	} else {
		list << 0 << 0 << 0 << 0;
	}

	return list;
}

void InterpreterElementImpl::updateRendererContent(const QString &shape)
{
	QDomDocument classDoc;
	mGraphics.setContent(shape);
	QDomElement sdfElement = mGraphics.firstChildElement("graphics").firstChildElement("picture");
	classDoc.appendChild(classDoc.importNode(sdfElement, true));
	if (!classDoc.childNodes().isEmpty()) {
		mRenderer->load(classDoc);
	}
}
