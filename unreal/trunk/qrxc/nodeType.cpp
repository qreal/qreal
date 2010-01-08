#include "nodeType.h"
#include "diagram.h"
#include "xmlCompiler.h"
#include "../utils/outFile.h"
#include "pointPort.h"
#include "linePort.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "label.h"

#include <QDebug>

using namespace utils;

NodeType::NodeType(Diagram *diagram) : GraphicType(diagram)
{}

NodeType::~NodeType()
{
	foreach (Port *port, mPorts)
		delete port;
}

Type* NodeType::clone() const
{
	NodeType *result = new NodeType(mDiagram);
	GraphicType::copyFields(result);
	foreach (Port *port, mPorts)
		result->mPorts.append(port->clone());
	result->mSdfDomElement = mSdfDomElement;
	result->mPortsDomElement = mPortsDomElement;
	return result;
}

bool NodeType::initAssociations()
{
	return true;
}

bool NodeType::initGraphics()
{
	if (!initSdf())
		return false;
	if (!initPorts())
		return false;
	return true;
}

bool NodeType::initSdf()
{
	QDomElement sdfElement = mGraphics.firstChildElement("picture");
	if (!sdfElement.isNull()) {
		mWidth = sdfElement.attribute("sizex").toInt();
		mHeight = sdfElement.attribute("sizey").toInt();
		mSdfDomElement = sdfElement;
		mVisible = true;
	} else
		mVisible = false;
	return true;
}

void NodeType::generateSdf() const
{
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>" + resourceName("Class") + "</file>\n");

	OutFile out("generated/shapes/" + resourceName("Class"));
	mSdfDomElement.save(out(), 1);
}

bool NodeType::initPorts()
{
	QDomElement portsElement = mGraphics.firstChildElement("ports");
	if (portsElement.isNull())
		return true;
	mPortsDomElement = portsElement;
	initPointPorts(portsElement);
	initLinePorts(portsElement);

	return true;
}

bool NodeType::initPointPorts(QDomElement const &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("pointPort"); !portElement.isNull();
		portElement = portElement.nextSiblingElement("pointPort"))
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
	for (QDomElement portElement = portsElement.firstChildElement("linePort"); !portElement.isNull();
		portElement = portElement.nextSiblingElement("linePort"))
	{
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

void NodeType::generatePorts() const
{
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>" + resourceName("Ports") + "</file>\n");

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
	for (QDomElement portElement = portsElement.firstChildElement("pointPort"); !portElement.isNull();
		portElement = portElement.nextSiblingElement("pointPort"))
	{
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
	for (QDomElement portElement = portsElement.firstChildElement("linePort"); !portElement.isNull();
		portElement = portElement.nextSiblingElement("linePort"))
	{
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

void NodeType::generateCode(OutFile &out)
{
	generateSdf();
	generatePorts();

	QString const className = NameNormalizer::normalize(qualifiedName());

	out() << "\tclass " << className << " : public NodeElement {\n"
		<< "\tpublic:\n"
		<< "\t\t" << className << "() {\n";

	bool hasSdf = false;
	bool hasPorts = false;

	QFile sdfFile("generated/shapes/" + className + "Class.sdf");
	if (sdfFile.exists()) {
		out() << "\t\t\tmRenderer.load(QString(\":/" << className << "Class.sdf\"));\n";
		hasSdf = true;
	}

	sdfFile.setFileName("generated/shapes/" + className + "Ports.sdf");
	if (sdfFile.exists()) {
		out() << "\t\t\tmPortRenderer.load(QString(\":/" << className << "Ports.sdf\"));\n";
		hasPorts = true;
	}

	out() << "\t\t\tmContents.setWidth(" << mWidth << ");\n"
		<< "\t\t\tmContents.setHeight(" << mHeight << ");\n";

	foreach (Port *port, mPorts)
		port->generateCode(out);

	foreach (Label *label, mLabels)
		label->generateCodeForConstructor(out);

	out() << "\t\t}\n\n"
		<< "\t\t~" << className << "() {}\n\n"
		<< "\t\tvoid paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)\n\t\t{\n";

	if (hasSdf)
		out() << "\t\t\tmRenderer.render(painter, mContents);\n";

	if (hasPorts)
		out() << "\t\t\tNodeElement::paint(painter, style, widget, &mPortRenderer);\n";
	else
		out() << "\t\t\tNodeElement::paint(painter, style, widget, NULL);\n";

	out() << "\t\t}\n\n"
		<< "\t\tvoid updateData()\n\t\t{\n"
		<< "\t\t\tNodeElement::updateData();\n";

	foreach (Label *label, mLabels)
		label->generateCodeForUpdateData(out);

	out() << "\t\t\tupdate();\n" << "\t\t}\n\n";

	out() << "\tprivate:\n";
	if (hasSdf)
		out() << "\t\tSdfRenderer mRenderer;\n";
	if (hasPorts)
		out() << "\t\tSdfRenderer mPortRenderer;\n";
	foreach (Label *label, mLabels)
		label->generateCodeForFields(out);
	out() << "\t};";
	out() << "\n\n";
}
