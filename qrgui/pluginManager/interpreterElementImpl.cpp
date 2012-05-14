#include "interpreterElementImpl.h"
#include "../../qrutils/outFile.h"

using namespace qReal;
using namespace utils;

InterpreterElementImpl::InterpreterElementImpl(qrRepo::RepoApi *repo, Id id)
	: mEditorRepoApi(repo), mId(id)
{}
//TODO:
void InterpreterElementImpl::init(QRectF &contents, QList<StatPoint> &pointPorts,
								  QList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,
								  QList<ElementTitleInterface*> &titles,
								  SdfRendererInterface *renderer, SdfRendererInterface *portRenderer)
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityNode") {
		mGraphics.setContent(mEditorRepoApi->stringProperty(mId, "shape"));
		QDomNodeList pointPortsList = mGraphics.firstChildElement("graphics").firstChildElement("ports").elementsByTagName("pointPort");
		QDomNodeList linePortsList = mGraphics.firstChildElement("graphics").firstChildElement("ports").elementsByTagName("linePort");

//		QString const labelText = (mEditorRepoApi->stringProperty(mId, "labelText"));
//		if (labelText.isEmpty()) {
//			Q_UNUSED(titles);
//			Q_UNUSED(factory);
//		}

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
		}
		for (int i = 0; i < linePortsList.size(); i + 2) {
			QDomElement lineElement1 = portsDoc.createElement("line");
			lineElement1.setAttribute("x1", linePortsList.at(i).toElement().attribute("startx"));
			lineElement1.setAttribute("y1", linePortsList.at(i).toElement().attribute("starty"));
			lineElement1.setAttribute("x2", linePortsList.at(i+1).toElement().attribute("endx"));
			lineElement1.setAttribute("y2", linePortsList.at(i+1).toElement().attribute("endy"));
			lineElement1.setAttribute("stroke-width", 7);
			lineElement1.setAttribute("stroke-style", "solid");
			lineElement1.setAttribute("stroke", "#c3dcc4");
			portsPicture.appendChild(lineElement1);
			QDomElement lineElement2 = portsDoc.createElement("line");
			lineElement2.setAttribute("x1", linePortsList.at(i).toElement().attribute("startx"));
			lineElement2.setAttribute("y1", linePortsList.at(i).toElement().attribute("starty"));
			lineElement2.setAttribute("x2", linePortsList.at(i+1).toElement().attribute("endx"));
			lineElement2.setAttribute("y2", linePortsList.at(i+1).toElement().attribute("endy"));
			lineElement2.setAttribute("stroke-width", 1);
			lineElement2.setAttribute("stroke-style", "solid");
			lineElement2.setAttribute("stroke", "#465945");
			portsPicture.appendChild(lineElement2);
		}

		portsDoc.appendChild(portsPicture);
//		OutFile out1("ololoPorts.txt");
//		portsDoc.save(out1(), 1);
		if(!portsDoc.childNodes().isEmpty()) {
			portRenderer->load(portsDoc);
		}

		contents.setWidth(width);
		contents.setHeight(height);

//		for (QDomElement label = mGraphics.firstChildElement("labels").firstChildElement("label");
//			!label.isNull();
//			label = label.nextSiblingElement("label"))
//		{
//			QString const labelType = mEditorRepoApi->stringProperty(mId, "labelType");
//			if (labelType == "Static text")
//				label.setAttribute("text", labelText);
//			else if (labelType == "Dynamic text")
//				label.setAttribute("textBinded", labelText);

//		}
//		foreach (Label *label, mLabels)
//			label->generateCodeForConstructor(out);   !!!
	}
}
//TODO:
void InterpreterElementImpl:: init(ElementTitleFactoryInterface &factory,
				  QList<ElementTitleInterface*> &titles)
{
		if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge") {
			if (!(mEditorRepoApi->stringProperty(mId, "labelText")).isEmpty()) {
				//mLabels[0]->generateCodeForConstructor(out);
			} else {
				Q_UNUSED(titles);
				Q_UNUSED(factory);
			}
		}
}

void InterpreterElementImpl::paint(QPainter *painter, QRectF &contents)
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityNode") {
		if(!mGraphics.childNodes().isEmpty()) {
			mRenderer->render(painter, contents);
		}
	}
}
//TODO:
void InterpreterElementImpl::updateData(ElementRepoInterface *repo) const
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge") {
		if ((mEditorRepoApi->stringProperty(mId, "labelText")).isEmpty()) {
			Q_UNUSED(repo);
		} else {
			//mLabels[0]->generateCodeForUpdateData(out);
		}
	}

	if (mEditorRepoApi->typeName(mId) == "MetaEntityNode") {

	}
}

bool InterpreterElementImpl::isNode()
{
	return mEditorRepoApi->typeName(mId) == "MetaEntityNode";
}

bool InterpreterElementImpl::hasPorts()
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityNode") {
		QDomDocument portsDoc;
		portsDoc.setContent(mEditorRepoApi->stringProperty(mId, "shape"));
		QDomNodeList pointPorts = portsDoc.elementsByTagName("pointPort");
		QDomNodeList linePorts = portsDoc.elementsByTagName("linePort");
		if (!pointPorts.isEmpty() || !linePorts.isEmpty())
			return true;
		else
			return false;
	}
	return false;
}

bool InterpreterElementImpl::isResizeable()
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityNode") {
		if (mEditorRepoApi->stringProperty(mId, "isResizeable") == "false")
			return false;
		else
			return true;
	}
	return true;
}

Qt::PenStyle InterpreterElementImpl::getPenStyle()
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge") {
		QString QtStyle = "Qt::" + mEditorRepoApi->stringProperty(mId, "lineType").replace(0, 1, mEditorRepoApi->stringProperty(mId, "lineType").at(0).toUpper());
		if (QtStyle != "") {
			if (QtStyle == "Qt::NoPen")
				return Qt::NoPen;
			if (QtStyle == "Qt::SolidLine")
				return Qt::SolidLine;
			if (QtStyle == "Qt::DashLine")
				return Qt::DashLine;
			if (QtStyle == "Qt::DotLine")
				return Qt::DotLine;
			if (QtStyle == "Qt::DashDotLine")
				return Qt::DashDotLine;
			if (QtStyle == "Qt::DashDotDotLine")
				return Qt::DashDotDotLine;
			if (QtStyle == "Qt::CustomDashLine")
				return Qt::CustomDashLine;
		}
	}
	return Qt::SolidLine;
}

int InterpreterElementImpl::getPenWidth()
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge") {
		QDomElement lineWidthElement = mGraphics.firstChildElement("lineWidth");
		if (lineWidthElement.isNull()) {
			return 1;
		}
		else {
			QString lineWidth = lineWidthElement.attribute("width");
			if (lineWidth.isEmpty())
			{
				qDebug() << "ERROR: no width of line";
				return false;
			}
			else {
				bool success = true;
				int lineWidthInt = lineWidth.toInt(&success);
				if (!success) {
					qDebug() << "ERROR: line width is not a number";
					return false;
				}
				else if (lineWidthInt <= 0) {
					qDebug() << "ERROR: line width is negative number";
					return false;
				}
				else
					return lineWidthInt;
			}
		}
	}
	return 0;
}

QColor InterpreterElementImpl::getPenColor()
{
	QColor lineColor;
	if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge")
		return QColor(lineColor.red(), lineColor.green(), lineColor.blue());
	return QColor();
}

void InterpreterElementImpl::drawStartArrow(QPainter *painter) const
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge") {
		QString style = "";
		foreach (Id edgeChild, mEditorRepoApi->children(mId)) {
			if(mEditorRepoApi->typeName(edgeChild) == "MetaEntityAssociation") {
				 style = mEditorRepoApi->stringProperty(edgeChild, "beginType");
			}
		}
		if (style.isEmpty())
			style = "filled_arrow";
		QBrush old = painter->brush();
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);

		if (style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow")
			brush.setColor(Qt::white);

		if (style == "filled_arrow" || style == "filled_rhomb")
			brush.setColor(Qt::black);
		painter->setBrush(brush);

		if (style == "empty_arrow" || style == "filled_arrow") {
			static const QPointF points[] = {QPointF(0,0), QPointF(-5,10), QPointF(5,10)};
			painter->drawPolygon(points, 3);
		}

		if (style == "empty_rhomb" || style == "filled_rhomb") {
			static const QPointF points[] = {QPointF(0,0), QPointF(-5,10), QPointF(0,20), QPointF(5,10)};
			painter->drawPolygon(points, 4);
		}

		if (style == "open_arrow") {
			static const QPointF points[] = {QPointF(-5,10), QPointF(0,0), QPointF(5,10)};
			painter->drawPolyline(points, 3);
		}

		if (style == "complex_arrow") {
			static const QPointF points[] = {QPointF(-15,30), QPointF(-10,10), QPointF(0,0), QPointF(10,10), QPointF(15,30), QPointF(0,23), QPointF(-15,30)};
			painter->drawPolyline(points, 7);
		}
		painter->setBrush(old);
	}
}

void InterpreterElementImpl::drawEndArrow(QPainter *painter) const
{
	if (mEditorRepoApi->typeName(mId) == "MetaEntityEdge") {
		QString style = "";
		foreach (Id edgeChild, mEditorRepoApi->children(mId)) {
			if(mEditorRepoApi->typeName(edgeChild) == "MetaEntityAssociation") {
				 style = mEditorRepoApi->stringProperty(edgeChild, "endType");
			}
		}
		if (style.isEmpty())
			style = "filled_arrow";
		QBrush old = painter->brush();
		QBrush brush;
		brush.setStyle(Qt::SolidPattern);

		if (style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow")
			brush.setColor(Qt::white);

		if (style == "filled_arrow" || style == "filled_rhomb")
			brush.setColor(Qt::black);
		painter->setBrush(brush);

		if (style == "empty_arrow" || style == "filled_arrow") {
			static const QPointF points[] = {QPointF(0,0), QPointF(-5,10), QPointF(5,10)};
			painter->drawPolygon(points, 3);
		}

		if (style == "empty_rhomb" || style == "filled_rhomb") {
			static const QPointF points[] = {QPointF(0,0), QPointF(-5,10), QPointF(0,20), QPointF(5,10)};
			painter->drawPolygon(points, 4);
		}

		if (style == "open_arrow") {
			static const QPointF points[] = {QPointF(-5,10), QPointF(0,0), QPointF(5,10)};
			painter->drawPolyline(points, 3);
		}

		if (style == "complex_arrow") {
			static const QPointF points[] = {QPointF(-15,30), QPointF(-10,10), QPointF(0,0), QPointF(10,10), QPointF(15,30), QPointF(0,23), QPointF(-15,30)};
			painter->drawPolyline(points, 7);
		}
		painter->setBrush(old);
	}
}

bool InterpreterElementImpl::isContainer()
{
	return mEditorRepoApi->typeName(mId) == "Container";
}

bool InterpreterElementImpl::isSortingContainer()
{
	return true; //(mEditorRepoApi->stringProperty(mId, "sortContainer") == "true");
}

int InterpreterElementImpl::sizeOfForestalling()
{
	return mEditorRepoApi->stringProperty(mId, "forestallingSize").toInt();
}

int InterpreterElementImpl::sizeOfChildrenForestalling()
{
	return mEditorRepoApi->stringProperty(mId, "childrenForestallingSize").toInt();
}

bool InterpreterElementImpl::hasMovableChildren()
{
	return (mEditorRepoApi->stringProperty(mId, "banChildrenMove") == "false");
}

bool InterpreterElementImpl::minimizesToChildren()
{
	return (mEditorRepoApi->stringProperty(mId, "minimizeToChildren") == "true");

}

bool InterpreterElementImpl::maximizesChildren()
{
	return (mEditorRepoApi->stringProperty(mId, "maximizeChildren") == "true");
}

bool InterpreterElementImpl::isPort()
{
	return (mEditorRepoApi->stringProperty(mId, "isPin") == "true");
}

bool InterpreterElementImpl::hasPin()
{
	return (mEditorRepoApi->stringProperty(mId, "isAction") == "true");
}

QList<double> InterpreterElementImpl::border()
{
	QList<double> list;
	if (mEditorRepoApi->stringProperty(mId, "isAction") == "true")
		list << 30 << 15 << 15 << 25;
	else
		list << 0 << 0 << 0 << 0;
	return list;
}
//TODO:
QStringList InterpreterElementImpl::bonusContextMenuFields()
{
	return QStringList();
}
