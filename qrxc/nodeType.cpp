#include <QtCore/QDebug>

#include "nodeType.h"
#include "diagram.h"
#include "xmlCompiler.h"
#include "../qrutils/outFile.h"
#include "pointPort.h"
#include "linePort.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "label.h"

using namespace utils;

NodeType::NodeType(Diagram *diagram)
	: GraphicType(diagram)
	, mIsPin(false)
	, mIsHavePin(false)
	, mIsResizeable(true)
{
}

NodeType::~NodeType()
{
	foreach (Port *port, mPorts) {
		delete port;
	}
}

Type* NodeType::clone() const
{
	NodeType *result = new NodeType(mDiagram);
	GraphicType::copyFields(result);
	foreach (Port *port, mPorts) {
		result->mPorts.append(port->clone());
	}
	result->mSdfDomElement = mSdfDomElement;
	result->mPortsDomElement = mPortsDomElement;
	result->mIsPin = mIsPin;
	result->mIsHavePin = mIsHavePin;
	result->mIsResizeable = mIsResizeable;
	return result;
}

bool NodeType::initAssociations()
{
	return true;
}

bool NodeType::initDividability()
{
	return true;
}

bool NodeType::initGraphics()
{
	return initSdf() && initPorts() && initBooleanProperties();
}

bool NodeType::initSdf()
{
	if (isWidgetBased(mGraphics)) {
		QDomElement wtfElement = mGraphics.firstChildElement("widget-template");
		if (!wtfElement.isNull()) {
			mSdfDomElement = wtfElement;
			QDomElement const rootElement = wtfElement.firstChildElement("Root");
			initSizeFromRoot(rootElement);
			mVisible = true;
		} else {
			mVisible = false;
		}
	} else {
		QDomElement const sdfElement = mGraphics.firstChildElement("picture");
		if (!sdfElement.isNull()) {
			mWidth = sdfElement.attribute("sizex").toInt();
			mHeight = sdfElement.attribute("sizey").toInt();
			mSdfDomElement = sdfElement;
			mVisible = true;
		} else {
			mVisible = false;
		}
	}
	return true;
}

void NodeType::initSizeFromRoot(QDomElement const &root)
{
	if (root.isNull()) {
		return;
	}
	QDomElement rootSubproperty = root.firstChild().toElement();
	while (!rootSubproperty.isNull()) {
		if (rootSubproperty.tagName() == "property" &&
			rootSubproperty.hasAttribute("propertyName") &&
			rootSubproperty.attribute("propertyName") == "geometry") {

			QString const ws = rootSubproperty.attribute("width");
			QString const hs = rootSubproperty.attribute("height");
			bool ok;
			int w = ws.toInt(&ok);
			if (ok) {
				mWidth = w;
			}
			int h = hs.toInt(&ok);
			if (ok) {
				mHeight = h;
			}
			return;
		}
		rootSubproperty = rootSubproperty.nextSibling().toElement();
	}
}

void NodeType::generateSdf() const
{
	mDiagram->editor()->xmlCompiler()->addResource(
		"\t<file>generated/shapes/" + resourceName("Class") + "</file>\n");

	OutFile out("generated/shapes/" + resourceName("Class"));
	mSdfDomElement.save(out(), 1);
}

bool NodeType::isWidgetBased(QDomElement const &graphics) const
{
	if (graphics.isNull()) {
		return false;
	}
	// TODO: move tag name to global constant
	return !graphics.firstChildElement("widget-template").isNull();
}

bool NodeType::initPorts()
{
	QDomElement portsElement = mGraphics.firstChildElement("ports");
	if (portsElement.isNull()) {
		return true;
	}
	mPortsDomElement = portsElement;
	initPointPorts(portsElement);
	initLinePorts(portsElement);

	return true;
}

bool NodeType::initPointPorts(QDomElement const &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("pointPort");
			!portElement.isNull();
			portElement = portElement.nextSiblingElement("pointPort")) {

		Port *pointPort = new PointPort();
		if (!pointPort->init(portElement, mWidth, mHeight)) {
			delete pointPort;
			return false;
		}
		mPorts.append(pointPort);
	}
	return true;
}

bool NodeType::initLinePorts(QDomElement const &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("linePort");
			!portElement.isNull();
			portElement = portElement.nextSiblingElement("linePort")) {

		Port *linePort = new LinePort();
		if (!linePort->init(portElement, mWidth, mHeight))
		{
			delete linePort;
			return false;
		}
		mPorts.append(linePort);
	}
	return true;
}

bool NodeType::initLabel(Label *label, QDomElement const &element, int const &count)
{
	return label->init(element, count, true, mWidth, mHeight);
}

bool NodeType::initBooleanProperties()
{
	mIsPin = false;
	mIsHavePin = false;
	mIsResizeable = true;
	QDomElement const element = mLogic.firstChildElement("pin");
	if (!element.isNull()) {
		mIsPin = true;
	}
	QDomElement const element1 = mLogic.firstChildElement("action");
	if (!element1.isNull()) {
		mIsHavePin = true;
	}
	QDomElement const element2 = mGraphics.firstChildElement("nonResizeable");
	if (!element2.isNull()) {
		mIsResizeable = false;
	}
	return true;
}

void NodeType::generatePorts() const
{
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>generated/shapes/" + resourceName("Ports") + "</file>\n");

	OutFile out("generated/shapes/" + resourceName("Ports"));
	out() << "<picture ";
	out() << "sizex=\"" << mWidth << "\" ";
	out() << "sizey=\"" << mHeight << "\" ";
	out() << ">\n";

	generatePointPorts(mPortsDomElement, out);
	generateLinePorts(mPortsDomElement, out);

	out() << "</picture>\n";
}

void NodeType::generatePointPorts(QDomElement const &portsElement, OutFile &out) const
{
	for (QDomElement portElement = portsElement.firstChildElement("pointPort");
			!portElement.isNull();
			portElement = portElement.nextSiblingElement("pointPort")) {

		out() << "\t<point stroke-width=\"11\" stroke-style=\"solid\" stroke=\"#c3dcc4\" ";
		out() << "x1=\""<<portElement.attribute("x") << "\" y1=\""<<portElement.attribute("y") << "\" ";
		out() << "/>\n";
		out() << "\t<point stroke-width=\"3\" stroke-style=\"solid\" stroke=\"#465945\" ";
		out() << "x1=\"" << portElement.attribute("x") << "\" y1=\"" << portElement.attribute("y") << "\" ";
		out() << "/>\n";
	}
}

void NodeType::generateLinePorts(QDomElement const &portsElement, OutFile &out) const
{
	for (QDomElement portElement = portsElement.firstChildElement("linePort");
			!portElement.isNull();
			portElement = portElement.nextSiblingElement("linePort")) {

		QDomElement portStartElement = portElement.firstChildElement("start");
		QDomElement portEndElement = portElement.firstChildElement("end");

		out() << "\t<line x1=\"" << portStartElement.attribute("startx") << "\" y1=\"" << portStartElement.attribute("starty") << "\" ";
		out() << "x2=\"" << portEndElement.attribute("endx") << "\" y2=\"" << portEndElement.attribute("endy") << "\" ";
		out() << "stroke-width=\"7\" stroke-style=\"solid\" stroke=\"#c3dcc4\" ";
		out() << "/>\n";

		out() << "\t<line x1=\"" << portStartElement.attribute("startx") << "\" y1=\""<<portStartElement.attribute("starty") << "\" ";
		out() << "x2=\""<<portEndElement.attribute("endx") << "\" y2=\"" << portEndElement.attribute("endy") << "\" ";
		out() << "stroke-width=\"1\" stroke-style=\"solid\" stroke=\"#465945\" ";
		out() << "/>\n";
	}
}

bool NodeType::hasPointPorts()
{
	foreach (Port *port, mPorts) {
		if (dynamic_cast<PointPort*>(port)) {
			return true;
		}
	}
	return false;
}

bool NodeType::hasLinePorts()
{
	foreach (Port *port, mPorts) {
		if (dynamic_cast<LinePort*>(port)) {
			return true;
		}
	}
	return false;
}

void NodeType::generateCode(OutFile &out)
{
	generateSdf();
	generatePorts();

	QString const className = NameNormalizer::normalize(qualifiedName());
	bool hasSdf = false;
	bool hasWtf = false;
	bool hasPorts = false;

	out() << "\tclass " << className << " : public ElementImpl\n\t{\n"
	<< "\tpublic:\n";

	if (!mBonusContextMenuFields.empty()) {
		out() << "\t\t" << className << "()\n\t\t{\n";
		out() << "\t\t\tmBonusContextMenuFields";
		foreach (QString const &elem, mBonusContextMenuFields) {
			out() << " << " << "\"" << elem << "\"";
		}
		out() << ";\n";
		out() << "\t\t}\n\n";
	}

	out () << "\t\tvoid init(ElementTitleFactoryInterface &, QList<ElementTitleInterface*> &) {}\n\n"
	<< "\t\tvoid init(QRectF &contents, QList<StatPoint> &pointPorts,\n"
	<< "\t\t\t\t\t\t\tQList<StatLine> &linePorts, ElementTitleFactoryInterface &factory,\n"
	<< "\t\t\t\t\t\t\tQList<ElementTitleInterface*> &titles, SdfRendererInterface *renderer,\n"
	<< "\t\t\t\t\t\t\tSdfRendererInterface *portRenderer, WidgetsHelperInterface *widgetsHelper)\n\t\t{\n";

	if (!hasPointPorts()) {
		out() << "\t\t\tQ_UNUSED(pointPorts);\n";
	}
	if (!hasLinePorts()) {
		out() << "\t\t\tQ_UNUSED(linePorts);\n";
	}
	if (mLabels.size() == 0) {
		out() << "\t\t\tQ_UNUSED(titles);\n"
		<<"\t\t\tQ_UNUSED(factory);\n";
	}

	QFile sdfFile("generated/shapes/" + className + "Class.sdf");
	if (sdfFile.exists()) {
		out() << "\t\t\tmRenderer = renderer;\n"
		"\t\t\tmRenderer->load(QString(\":/generated/shapes/" << className << "Class.sdf\"));\n";
		hasSdf = true;
	} else {
		out() << "\t\t\tQ_UNUSED(portRenderer);\n";
	}

	sdfFile.setFileName("generated/shapes/" + className + "Ports.sdf");
	if (sdfFile.exists()) {
		out() << "\t\t\tportRenderer->load(QString(\":/generated/shapes/" << className << "Ports.sdf\"));\n";
		hasPorts = true;
	}

	QFile wtfFile("generated/shapes/" + className + "Class.wtf");
	if (wtfFile.exists()) {
		out() << "\t\t\tQ_UNUSED(renderer)\n";
		out() << "\t\t\tmWidgetsHelper = widgetsHelper;\n"
		"\t\t\tmWidgetsHelper->initWidget(QString(\":/generated/shapes/" << className << "Class.wtf\"));\n";
		out() << "\t\t\tmPropertyEditors = mWidgetsHelper->propertyEditors();\n";
		hasWtf = true;
	} else {
		out() << "\t\t\tQ_UNUSED(widgetsHelper);\n";
	}

	out() << "\t\t\tcontents.setWidth(" << mWidth << ");\n"
	<< "\t\t\tcontents.setHeight(" << mHeight << ");\n";

	foreach (Port *port, mPorts) {
		port->generateCode(out);
	}

	foreach (Label *label, mLabels) {
		label->generateCodeForConstructor(out);
	}

	out() << "\t\t}\n\n";

	out() << "\t\t ElementImpl *clone() { return NULL; }\n";

	out() << "\t\t~" << className << "() {}\n\n"
	<< "\t\tvoid paint(QPainter *painter, QRectF &contents)\n\t\t{\n";

	if (hasSdf) {
		out() << "\t\t\tmRenderer->render(painter, contents);\n";
	} else {
		out() << "\t\t\tQ_UNUSED(painter);\n";
		out() << "\t\t\tQ_UNUSED(contents);\n";
	}

	out() << "\t\t}\n\n";

	out() << "\t\tQt::PenStyle getPenStyle() const { return Qt::SolidLine; }\n\n"
	<< "\t\tint getPenWidth() const { return 0; }\n\n"
	<< "\t\tQColor getPenColor() const { return QColor(); }\n\n"
	<< "\t\tvoid drawStartArrow(QPainter *) const {}\n"
	<< "\t\tvoid drawEndArrow(QPainter *) const {}\n"
	<< "\t\tbool hasPorts() const\n\t\t{\n";

	out() << (hasPorts ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"
	<< "\t\tvoid updateData(ElementRepoInterface *repo) const\n\t\t{\n";

	if (mLabels.isEmpty() && !hasWtf) {
		out() << "\t\t\tQ_UNUSED(repo);\n";
	}
	if (!mLabels.isEmpty()) {
		foreach (Label *label, mLabels) {
			label->generateCodeForUpdateData(out);
		}
	}
	if (hasWtf) {
		out() << "\t\t\tforeach (QString const &propertyName, mPropertyEditors.keys()) {\n";
		out() << "\t\t\t\tforeach (PropertyEditorInterface *propertyEditor, mPropertyEditors.values(propertyName)) {\n";
		out() << "\t\t\t\t\tpropertyEditor->setPropertyValue(repo->logicalProperty(propertyName));\n";
		out() << "\t\t\t\t}\n";
		out() << "\t\t\t}\n";
	}

	out() << "\t\t}\n\n"
	<< "\t\tbool isNode() const\n\t\t{\n"
	<< "\t\t\treturn true;\n"
	<< "\t\t}\n\n"

	<< "\t\tbool isResizeable() const\n\t\t{\n"
	<< "\t\t\treturn " << (mIsResizeable ? "true" : "false") << ";\n"
	<< "\t\t}\n\n"

	<< "\t\tbool isContainer() const\n\t\t{\n"
	<< (!mContains.empty() ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool isSortingContainer() const\n\t\t{\n"
	<< (mContainerProperties.isSortingContainer ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tint sizeOfForestalling() const\n\t\t{\n"
	<< "\t\t\treturn " + QString::number(mContainerProperties.sizeOfForestalling) + ";\n"
	<< "\t\t}\n\n"

	<< "\t\tint sizeOfChildrenForestalling() const\n\t\t{\n"
	<< "\t\t\treturn " << QString::number(mContainerProperties.sizeOfChildrenForestalling) << ";\n"
	<< "\t\t}\n\n"

	<< "\t\tbool hasMovableChildren() const\n\t\t{\n"
	<< (mContainerProperties.hasMovableChildren ?  "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool minimizesToChildren() const\n\t\t{\n"
	<< (mContainerProperties.minimizesToChildren ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool maximizesChildren() const\n\t\t{\n"
	<< (mContainerProperties.maximizesChildren ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool isDividable() const\n\t\t{\n\t\t\treturn false;\n\t\t}\n\n"

	<< "\t\tbool isPort() const\n\t\t{\n"
	<< (mIsPin ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool hasPin() const\n\t\t{\n"
	<< (mIsHavePin ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n";

	out() << "\t\tQList<double> border() const\n\t\t{\n"
	<< "\t\t\tQList<double> list;\n";
	if (mIsHavePin) {
		out() << "\t\t\tlist << 30 << 15 << 15 << 25;\n";
	} else {
		out() << "\t\t\tlist << 0 << 0 << 0 << 0;\n";
	}
	out() << "\t\t\treturn list;\n"
	<< "\t\t}\n\n";

	out() << "\t\tQStringList bonusContextMenuFields() const\n\t\t{\n" << "\t\t\treturn ";
	if (!mBonusContextMenuFields.empty()) {
		out() << "mBonusContextMenuFields;";
	} else {
		out() << "QStringList();";
	}
	out() << "\n\t\t}\n\n";

	out() << "\tprivate:\n";
	if (!mBonusContextMenuFields.empty()) {
		out() << "\t\tQStringList mBonusContextMenuFields;\n";
	}
	if (hasSdf) {
		out() << "\t\tSdfRendererInterface *mRenderer;\n";
	}
	if (hasWtf) {
		out() << "\t\tWidgetsHelperInterface *mWidgetsHelper;\n";
		out() << "\t\tQMap<QString, PropertyEditorInterface *> mPropertyEditors;\n	";
	}
	foreach (Label *label, mLabels) {
		label->generateCodeForFields(out);
	}
	out() << "\t};";
	out() << "\n\n";
}
