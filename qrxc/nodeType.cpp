#include <QtCore/QDebug>

#include "nodeType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

#include "diagram.h"
#include "xmlCompiler.h"
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
	qDeleteAll(mPorts);
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

bool NodeType::initPortTypes()
{
	return true;
}

bool NodeType::initGraphics()
{
	return initSdf() && initPorts() && initBooleanProperties() && initIcon();
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
	for (QDomElement portElement = portsElement.firstChildElement("pointPort")
			; !portElement.isNull()
			; portElement = portElement.nextSiblingElement("pointPort"))
	{
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
	for (QDomElement portElement = portsElement.firstChildElement("linePort")
			; !portElement.isNull()
			; portElement = portElement.nextSiblingElement("linePort"))
	{
		Port *linePort = new LinePort();
		if (!linePort->init(portElement, mWidth, mHeight)) {
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

bool NodeType::initIcon()
{
	QDomElement iconElement = mGraphics.firstChildElement("icon").firstChildElement("graphics");
	if (iconElement.isNull()) {
		iconElement = mGraphics;
	}
	mIsIconWidgetBased = isWidgetBased(iconElement);
	mIconDomElement = mIsIconWidgetBased
			? mGraphics.firstChildElement("widget-template")
			: iconElement.firstChildElement("picture");
	return true;
}

void NodeType::generateIcon()
{
	mDiagram->editor()->xmlCompiler()->addResource(
		"\t<file>generated/shapes/" + resourceName("Icon") + "</file>\n");

	OutFile out("generated/shapes/" + resourceName("Icon"));
	mIconDomElement.save(out(), 1);
}

void NodeType::generateCode(OutFile &out)
{
	generateSdf();
	generateIcon();

	QString const className = NameNormalizer::normalize(qualifiedName());
	bool hasSdf = false;
	bool hasWtf = false;

	out() << "\tclass " << className << " : public qReal::ElementImpl\n\t{\n"
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

	out () << "\t\tvoid init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}\n\n"
	<< "\t\tvoid init(QRectF &contents, PortFactoryInterface const &portFactory, QList<PortInterface *> &ports\n"
	<< "\t\t\t\t\t\t\t, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles\n"
	<< "\t\t\t\t\t\t\t, qReal::SdfRendererInterface *renderer, WidgetsHelperInterface *widgetsHelper,"
	<< " qReal::ElementRepoInterface *elementRepo)\n\t\t{\n";

	if (mPorts.empty()) {
		out() << "\t\t\tQ_UNUSED(portFactory);\n";
		out() << "\t\t\tQ_UNUSED(ports);\n";
	}

	if (mLabels.empty())
		out() << "\t\t\tQ_UNUSED(titles);\n"
			<<"\t\t\tQ_UNUSED(factory);\n";

	QFile sdfFile("generated/shapes/" + className + "Class.sdf");
	if (sdfFile.exists()) {
		out() << "\t\t\tmRenderer = renderer;\n"
				"\t\t\tmRenderer->load(QString(\":/generated/shapes/" << className << "Class.sdf\"));\n"
				<< "\t\t\tmRenderer->setElementRepo(elementRepo);\n";
		hasSdf = true;
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
		port->generateCode(out, mDiagram->editor()->getAllPortNames());
	}

	foreach (Label *label, mLabels) {
		label->generateCodeForConstructor(out);
	}

	out() << "\t\t}\n\n";

	out() << "\t\t qReal::ElementImpl *clone() { return NULL; }\n";

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
	<< "\t\tvoid drawEndArrow(QPainter *) const {}\n\n"

	<< "\t\tvoid updateData(qReal::ElementRepoInterface *repo) const\n\t\t{\n";

	if (hasSdf) {
		out() << "\t\t\tmRenderer->setElementRepo(repo);\n";
	}

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

	ContainerProperties *containerProperties = mContainerProperties ?
			mContainerProperties : new ContainerProperties;

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
	<< (mContainerProperties->isSortingContainer ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n";

	QStringList forestalling;
	foreach (int size, mContainerProperties->sizeOfForestalling) {
		forestalling << QString::number(size);
	}

	out() << "\t\tQVector<int> sizeOfForestalling() const\n\t\t{\n"
	<< "\t\t\tQVector<int> result;\n"
	<< "\t\t\tresult << " + forestalling[0] + " << " + forestalling[1] + " << " + forestalling[2]
			+ " << " + forestalling[3] + ";\n"
	<< ";\n\t\t\treturn result;\n"
	<< "\t\t}\n\n"

	<< "\t\tint sizeOfChildrenForestalling() const\n\t\t{\n"
	<< "\t\t\treturn " << QString::number(containerProperties->sizeOfChildrenForestalling) << ";\n"
	<< "\t\t}\n\n"

	<< "\t\tbool hasMovableChildren() const\n\t\t{\n"
	<< (containerProperties->hasMovableChildren ?  "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool minimizesToChildren() const\n\t\t{\n"
	<< (containerProperties->minimizesToChildren ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool maximizesChildren() const\n\t\t{\n"
	<< (containerProperties->maximizesChildren ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool isDividable() const\n\t\t{\n\t\t\treturn false;\n\t\t}\n\n"

	<< "\t\tQStringList fromPortTypes() const\n\t\t{\n\t\t\treturn QStringList(\"NonTyped\");\n\t\t}\n\n"

	<< "\t\tQStringList toPortTypes() const\n\t\t{\n\t\t\treturn QStringList(\"NonTyped\");\n\t\t}\n\n"

	<< "\t\tenums::linkShape::LinkShape shapeType() const\n\t\t{\n\t\t\treturn enums::linkShape::square;\n\t\t}\n\n"

	<< "\t\tbool isPort() const\n\t\t{\n"
	<< (mIsPin ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool hasPin() const\n\t\t{\n"
	<< (mIsHavePin ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool createChildrenFromMenu() const\n\t\t{\n"
	<< (mCreateChildrenFromMenu ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
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

	out() << "\t\tQString layout() const\n\t\t{\n";
	out() << "\t\t\treturn \"" << containerProperties->layout << "\";\n";
	out() << "\t\t}\n\n";

	out() << "\t\tQString layoutBinding() const\n\t\t{\n";
	out() << "\t\t\treturn \"" << containerProperties->layoutBinding << "\";\n";
	out() << "\t\t}\n\n";

	if (!mContainerProperties) {
		delete containerProperties;
	}

	out() << "\tprivate:\n";
	if (!mBonusContextMenuFields.empty()) {
		out() << "\t\tQStringList mBonusContextMenuFields;\n";
	}
	if (hasSdf) {
		out() << "\t\tqReal::SdfRendererInterface *mRenderer;\n";
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

bool NodeType::generatePorts(OutFile &out, bool isNotFirst)
{
	GraphicType::generateOneCase(out, isNotFirst);

	QSet<QString> portTypes;
	foreach (Port *port, mPorts) {
		portTypes.insert(port->type());
	}

	if (!portTypes.empty()) {
		out() << "\t\tresult ";
		foreach (QString const &type, portTypes) {
			out() << "<< \"" << type << "\"";
		}
		out() << ";\n";
	}

	out() << "\t}\n";
	return true;
}
