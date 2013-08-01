#include "interpreterElementImpl.h"
#include "../../qrutils/outFile.h"
#include "../../qrutils/scalableItem.h"

using namespace qReal;
using namespace utils;

InterpreterElementImpl::InterpreterElementImpl(qrRepo::RepoApi *repo, Id const &metaId)
		: mEditorRepoApi(repo), mId(metaId)
{
}

void InterpreterElementImpl::initLabels(int const &width, int const &height, LabelFactoryInterface &factory
		, QList<LabelInterface*> &titles)
{
	for (QDomElement element = mGraphics.firstChildElement("graphics").firstChildElement("labels").firstChildElement("label");
			!element.isNull();
			element = element.nextSiblingElement("label"))
	{
		ScalableCoordinate const x = utils::ScalableItem::initCoordinate(element.attribute("x"), width);
		ScalableCoordinate const y = utils::ScalableItem::initCoordinate(element.attribute("y"), height);
		QString const center = element.attribute("center", "false");
		QString const text = element.attribute("text");
		QString const textBinded = element.attribute("textBinded");
		QString const readOnly = element.attribute("readOnly", "false");
		QString const background = element.attribute("background", "transparent");
		qreal const rotation = element.attribute("rotation", "0").toDouble();
		if (text.isEmpty() && textBinded.isEmpty()) {
			qDebug() << "ERROR: can't parse label";
		} else {
			LabelInterface *title = NULL;
			if (text.isEmpty()) {
				// It is a binded label, text for it will be taken from repository.
				title = factory.createTitle(x.value(), y.value(), textBinded, readOnly == "true", rotation);
			} else {
				// This is a statical label, it does not need repository.
				title = factory.createTitle(x.value(), y.value(), text, rotation);
			}
			title->setBackground(QColor(background));
			title->setScaling(x.isScalable(), y.isScalable());
			title->setFlags(0);
			title->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title);
			mNodeLabels.append(NodeLabel(textBinded, center, title));
		}
	}
}

void InterpreterElementImpl::initPointPorts(QList<StatPoint> &pointPorts, QDomDocument &portsDoc
		, QDomNode &portsPicture, int const &width, int const &height)
{
	QDomNodeList const pointPortsList = mGraphics.firstChildElement("graphics").firstChildElement("ports").elementsByTagName("pointPort");
	for (int i = 0; i < pointPortsList.size(); i++) {
		QDomElement portsElement1 = portsDoc.createElement("point");
		portsElement1.setAttribute("stroke-width", 11);
		portsElement1.setAttribute("stroke-style", "solid");
		portsElement1.setAttribute("stroke", "#c3dcc4");
		portsElement1.setAttribute("x1", pointPortsList.at(i).toElement().attribute("x"));
		portsElement1.setAttribute("y1", pointPortsList.at(i).toElement().attribute("y"));
		portsPicture.appendChild(portsElement1);
		QDomElement portsElement2 = portsDoc.createElement("point");
		portsElement2.setAttribute("stroke-width", 3);
		portsElement2.setAttribute("stroke-style", "solid");
		portsElement2.setAttribute("stroke", "#465945");
		portsElement2.setAttribute("x1", pointPortsList.at(i).toElement().attribute("x"));
		portsElement2.setAttribute("y1", pointPortsList.at(i).toElement().attribute("y"));
		portsPicture.appendChild(portsElement2);
		StatPoint pt;
		QString x = pointPortsList.at(i).toElement().attribute("x");
		if (x.endsWith("a")) {
			pt.prop_x = true;
			x.chop(1);
		} else {
			pt.prop_x = false;
		}

		QString y = pointPortsList.at(i).toElement().attribute("y");
		if (y.endsWith("a")) {
			pt.prop_y = true;
			y.chop(1);
		} else {
			pt.prop_y = false;
		}

		pt.point = QPointF(x.toDouble() / static_cast<double>(width), y.toDouble() / static_cast<double>(height));
		pt.initWidth = width;
		pt.initHeight = height;
		pointPorts << pt;
	}
}

void InterpreterElementImpl::initLinePorts(QList<StatLine> &linePorts, QDomDocument &portsDoc
		, QDomNode &portsPicture, int const &width, int const &height)
{
	QDomNodeList const linePortsList = mGraphics.firstChildElement("graphics").firstChildElement("ports").elementsByTagName("linePort");
	for (int i = 0; i < linePortsList.size(); i++) {
		QDomElement lineElement1 = portsDoc.createElement("line");
		lineElement1.setAttribute("x1", linePortsList.at(i).firstChildElement("start").attribute("startx"));
		lineElement1.setAttribute("y1", linePortsList.at(i).firstChildElement("start").attribute("starty"));
		lineElement1.setAttribute("x2", linePortsList.at(i).firstChildElement("end").attribute("endx"));
		lineElement1.setAttribute("y2", linePortsList.at(i).firstChildElement("end").attribute("endy"));
		lineElement1.setAttribute("stroke-width", 7);
		lineElement1.setAttribute("stroke-style", "solid");
		lineElement1.setAttribute("stroke", "#c3dcc4");
		portsPicture.appendChild(lineElement1);
		QDomElement lineElement2 = portsDoc.createElement("line");
		lineElement2.setAttribute("x1", linePortsList.at(i).firstChildElement("start").attribute("startx"));
		lineElement2.setAttribute("y1", linePortsList.at(i).firstChildElement("start").attribute("starty"));
		lineElement2.setAttribute("x2", linePortsList.at(i).firstChildElement("end").attribute("endx"));
		lineElement2.setAttribute("y2", linePortsList.at(i).firstChildElement("end").attribute("endy"));
		lineElement2.setAttribute("stroke-width", 1);
		lineElement2.setAttribute("stroke-style", "solid");
		lineElement2.setAttribute("stroke", "#465945");
		portsPicture.appendChild(lineElement2);
		StatLine ln;
		QString x1 = linePortsList.at(i).firstChildElement("start").attribute("startx");
		if (x1.endsWith("a")) {
			ln.prop_x1 = true;
			x1.chop(1);
		} else {
			ln.prop_x1 = false;
		}

		QString y1 = linePortsList.at(i).firstChildElement("start").attribute("starty");
		if (y1.endsWith("a")) {
			ln.prop_y1 = true;
			y1.chop(1);
		} else {
			ln.prop_y1 = false;
		}

		QString x2 = linePortsList.at(i).firstChildElement("end").attribute("endx");
		if (x2.endsWith("a")) {
			ln.prop_x2 = true;
			x2.chop(1);
		} else {
			ln.prop_x2 = false;
		}

		QString y2 = linePortsList.at(i).firstChildElement("end").attribute("endy");
		if (y2.endsWith("a")) {
			ln.prop_y2 = true;
			y2.chop(1);
		} else {
			ln.prop_y2 = false;
		}

		ln.line = QLineF(x1.toDouble() / static_cast<double>(width)
				, y1.toDouble() / static_cast<double>(height)
				, x2.toDouble() / static_cast<double>(width)
				, y2.toDouble() / static_cast<double>(height));
		ln.initWidth = width;
		ln.initHeight = height;
		linePorts << ln;
	}
}

void InterpreterElementImpl::init(QRectF &contents, QList<StatPoint> &pointPorts
		, QList<StatLine> &linePorts, LabelFactoryInterface &factory
		, QList<LabelInterface*> &titles
		, SdfRendererInterface *renderer, SdfRendererInterface *portRenderer
		, ElementRepoInterface *elementRepo)
{
	Q_UNUSED(elementRepo);
	if (mId.element() == "MetaEntityNode") {
		mGraphics.setContent(mEditorRepoApi->stringProperty(mId, "shape"));
		QDomDocument classDoc;
		QDomElement sdfElement = mGraphics.firstChildElement("graphics").firstChildElement("picture");
		classDoc.appendChild(classDoc.importNode(sdfElement, true));
		if (!classDoc.childNodes().isEmpty()) {
			mRenderer = renderer;
			mRenderer->load(classDoc);
		}

		int width = 0;
		int height = 0;
		if (!sdfElement.isNull()) {
			width = sdfElement.attribute("sizex").toInt();
			height = sdfElement.attribute("sizey").toInt();
		}

		QDomDocument portsDoc;
		QDomNode portsPicture = portsDoc.importNode(sdfElement, false);
		initPointPorts(pointPorts, portsDoc, portsPicture, width, height);
		initLinePorts(linePorts, portsDoc, portsPicture, width, height);
		portsDoc.appendChild(portsPicture);
		if (!portsDoc.childNodes().isEmpty()) {
			portRenderer->load(portsDoc);
		}

		contents.setWidth(width);
		contents.setHeight(height);
		initLabels(width, height, factory, titles);
	}
}

void InterpreterElementImpl::init(LabelFactoryInterface &factory, QList<LabelInterface*> &titles)
{
	if (mId.element() == "MetaEntityEdge") {
		QString labelText = mEditorRepoApi->stringProperty(mId, "labelText");
		if (!labelText.isEmpty()) {
			QString const labelType = mEditorRepoApi->stringProperty(mId, "labelType");
			LabelInterface* title = NULL;
			if (labelType == "Static text") {
				// This is a statical label, it does not need repository.
				title = factory.createTitle(0, 0, labelText, 0);
			} else {
				// It is a binded label, text for it will be taken from repository.
				title = factory.createTitle(0, 0, labelText, false, 0);
			}
			title->setBackground(QColor(Qt::white));
			title->setScaling(false, false);
			title->setFlags(0);
			title->setTextInteractionFlags(Qt::NoTextInteraction);
			titles.append(title);
			mEdgeLabels.append(EdgeLabel(labelText, labelType, title));
		}
	}
}

void InterpreterElementImpl::paint(QPainter *painter, QRectF &contents)
{
	if (mId.element() == "MetaEntityNode") {
		if(!mGraphics.childNodes().isEmpty()) {
			mRenderer->render(painter, contents);
		}
	}
}

QStringList InterpreterElementImpl::getListOfStr(QString const &labelText) const
{
	QStringList reformedList = labelText.split("##");
	QStringList list;
	int counter = 1;
	foreach (QString const &str, reformedList) {
		list.append(str);
		counter++;
	}

	return list;
}

QString InterpreterElementImpl::getResultStr(QStringList const &list, ElementRepoInterface *repo) const
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
		foreach (QString const &listElement, list) {
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

void InterpreterElementImpl::updateData(ElementRepoInterface *repo) const
{
	if (mId.element() == "MetaEntityEdge") {
		foreach (EdgeLabel edgeLabel, mEdgeLabels) {
			if (edgeLabel.labelType == "Static text") {
				// Static label
				return;
			}
			QStringList const list = getListOfStr(edgeLabel.labelText);
			QString const resultStr = getResultStr(list, repo);
			edgeLabel.title->setHtml(QString("<center>%1</center>").arg(resultStr).replace("\n", "<br>"));
		}
	}

	if (mId.element() == "MetaEntityNode") {
		foreach (NodeLabel const &nodeLabel, mNodeLabels) {
			if (nodeLabel.textBinded.isEmpty()) {
				// Static label
				Q_UNUSED(repo);
				return;
			}
			QStringList const list = getListOfStr(nodeLabel.textBinded);
			QString const resultStr = getResultStr(list, repo);
			nodeLabel.title->setHtml(QString(nodeLabel.center == "true"
					? "<center>%1</center>" : "<b>%1</b>").arg(resultStr).replace("\n", "<br>"));
		}
	}
}

bool InterpreterElementImpl::isNode() const
{
	return mId.element() == "MetaEntityNode";
}

bool InterpreterElementImpl::hasPorts() const
{
	if (mId.element() == "MetaEntityNode") {
		QDomDocument portsDoc;
		portsDoc.setContent(mEditorRepoApi->stringProperty(mId, "shape"));
		QDomNodeList const pointPorts = portsDoc.elementsByTagName("pointPort");
		QDomNodeList const linePorts = portsDoc.elementsByTagName("linePort");
		return !pointPorts.isEmpty() || !linePorts.isEmpty();
	}
	return false;
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
		QString const QtStyle = "Qt::" + mEditorRepoApi->stringProperty(mId, "lineType").replace(0, 1, mEditorRepoApi->stringProperty(mId, "lineType").at(0).toUpper());
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

	QDomElement const lineWidthElement = mGraphics.firstChildElement("lineWidth");
	if (lineWidthElement.isNull()) {
		return 1;
	}

	QString const lineWidth = lineWidthElement.attribute("width");
	if (lineWidth.isEmpty()) {
		return 0;
	}

	bool success = true;
	int const lineWidthInt = lineWidth.toInt(&success);
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

void InterpreterElementImpl::drawArrow(QPainter *painter, QString const &type) const
{
	if (mId.element() == "MetaEntityEdge") {
		QString style = "";
		foreach (Id const &edgeChild, mEditorRepoApi->children(mId)) {
			if (edgeChild.element() == "MetaEntityAssociation") {
				 style = mEditorRepoApi->stringProperty(edgeChild, type);
			}
		}

		if (style.isEmpty()) {
			style = "filled_arrow";
		}

		QBrush const oldBrush = painter->brush();
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
			static const QPointF points[] = {QPointF(-15, 30), QPointF(-10, 10), QPointF(0, 0), QPointF(10, 10), QPointF(15, 30), QPointF(0, 23), QPointF(-15, 30)};
			painter->drawPolyline(points, 7);
		}

		painter->setBrush(oldBrush);
	}
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

bool InterpreterElementImpl::hasContainerProperty(QString const &property) const
{
	QDomElement const propertiesElement =
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

QVector<int> InterpreterElementImpl::getSizeOfContainerProperty(QString const &property) const
{
	QVector<int> size(4, 0);
	QDomElement const propertiesElement =
			mGraphics.firstChildElement("logic").firstChildElement("container").firstChildElement("properties");
	if (propertiesElement.hasChildNodes()) {
		if (!propertiesElement.firstChildElement(property).isNull()) {
			QStringList const sizeStr = propertiesElement.firstChildElement(property).attribute("size").split(',');
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

bool InterpreterElementImpl::isPort() const
{
	return mEditorRepoApi->stringProperty(mId, "isPin") == "true";
}

bool InterpreterElementImpl::hasPin() const
{
	return mEditorRepoApi->stringProperty(mId, "isAction") == "true";
}

QList<double> InterpreterElementImpl::border() const
{
	QList<double> list;
	if (mEditorRepoApi->stringProperty(mId, "isAction") == "true") {
		list << 30 << 15 << 15 << 25;
	} else {
		list << 0 << 0 << 0 << 0;
	}
	return list;
}

void InterpreterElementImpl::updateRendererContent(QString const &shape)
{
	QDomDocument classDoc;
	mGraphics.setContent(shape);
	QDomElement sdfElement = mGraphics.firstChildElement("graphics").firstChildElement("picture");
	classDoc.appendChild(classDoc.importNode(sdfElement, true));
	if (!classDoc.childNodes().isEmpty()) {
		mRenderer->load(classDoc);
	}
}

QStringList InterpreterElementImpl::bonusContextMenuFields() const
{
	return QStringList();
}
