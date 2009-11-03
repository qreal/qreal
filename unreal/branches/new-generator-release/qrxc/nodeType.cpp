#include "nodeType.h"
#include "diagram.h"
#include "xmlCompiler.h"
#include "outFile.h"
#include "pointPort.h"
#include "linePort.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "label.h"

#include <QDebug>

NodeType::NodeType(Diagram *diagram) : GraphicType(diagram)
{}

NodeType::~NodeType()
{
	foreach (Port *port, mPorts)
	{
		delete port;
	}
}

bool NodeType::initAssociations()
{
	return true;
}

void NodeType::addKernelParent()
{
	if ((mName != "Kernel Element") && (mName != "Named Element"))
	{
		mParents.append("Named Element");
	}
}

bool NodeType::initGraphics()
{
	if (!initSdf())
	{
		return false;
	}
	if (!initPorts())
	{
		return false;
	}
	return true;
}

bool NodeType::initSdf()
{
	QDomElement sdfElement = mElement.firstChildElement("picture");
	if (sdfElement.isNull())
	{
		mVisible = (mName == "NamedElement");
	}
	else
	{
		mWidth = sdfElement.attribute("sizex").toInt();
		mHeight = sdfElement.attribute("sizey").toInt();
		QString name = NameNormalizer::normalize(mName);
		QString resourceName = name + "Class.sdf";
		mDiagram->editor()->xmlCompiler()->addResource("\t<file>" + resourceName + "</file>\n");
		OutFile out("generated/shapes/" + resourceName);
		sdfElement.save(out(), 1);
		mVisible = true;
	}
	return true;
}

bool NodeType::initPorts()
{
	QDomElement portsElement = mElement.firstChildElement("ports");
	if (portsElement.isNull())
	{
		return true;
	}
	QString const name = NameNormalizer::normalize(mName);
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>" + name + "Ports.sdf" + "</file>\n");
	OutFile out("generated/shapes/" + name + "Ports.sdf");
	out() << "<picture ";
	out() << "sizex=\"" << mWidth << "\" ";
	out() << "sizey=\"" << mHeight << "\" ";
	out() << ">\n";

	if (!initPointPorts(portsElement, out))
	{
		qDebug() << "Error: can't parse point ports";
		return false;
	}
	if (!initLinePorts(portsElement, out))
	{
		qDebug() << "Error: can't parse line ports";
		return false;
	}
	out() << "</picture>\n";
	return true;
}

bool NodeType::initPointPorts(QDomElement const &portsElement, OutFile &out)
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

		Port *pointPort = new PointPort();
		if (!pointPort->init(portElement, mWidth, mHeight))
		{
			delete pointPort;
			return false;
		}
		mPorts.append(pointPort);
	}
	return true;
}

bool NodeType::initLinePorts(QDomElement const &portsElement, OutFile &out)
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

void NodeType::generateCode(OutFile &out)
{
	QString const className = NameNormalizer::normalize(mName);

	out() << "\tclass " << className << " : public NodeElement {\n"
		<< "\tpublic:\n"
		<< "\t\t" << className << "() {\n";

	bool hasSdf = false;
	bool hasPorts = false;

	QFile sdffile("generated/shapes/" + className + "Class.sdf");
	if (sdffile.exists()) {
		out() << "\t\t\tmRenderer.load(QString(\":/" << className << "Class.sdf\"));\n";
		hasSdf = true;
	}

	sdffile.setFileName("generated/shapes/" + className + "Ports.sdf");
	if (sdffile.exists()) {
		out() << "\t\t\tmPortRenderer.load(QString(\":/" << className << "Ports.sdf\"));\n";
		hasPorts = true;
	}

	out() << "\t\t\tmContents.setWidth(" << mWidth << ");\n"
		<< "\t\t\tmContents.setHeight(" << mHeight << ");\n"
		<< "\t\t\tmTitle.setFlags(0);\n"
		<< "\t\t\tmTitle.setTextInteractionFlags(Qt::NoTextInteraction);\n"
		<< "\t\t\tmTitle.setParentItem(this);\n";

	// True horror:
	if ((className == "Class_method") || (className == "Class_field")) {
		out() << "\tmDocVis.setParentItem(this);\n"
			<< "\tmDocType.setParentItem(this);\n";
	}

	foreach (Port *port, mPorts)
	{
		port->generateCode(out);
	}

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
	{
		label->generateCode(out);
	}
	if (mLabels.size() > 0)  // потом разобраться с надписями
	{
		out() << ";\n";
		out() << "\t\t\tmTitle.setHtml(text);\n";	
	}

	out() << "\t\t\tupdate();\n" << "\t\t}\n\n";

	out() << "\tprivate:\n";
	if (hasSdf)
		out() << "\t\tSdfRenderer mRenderer;\n";
	if (hasPorts)
		out() << "\t\tSdfRenderer mPortRenderer;\n";
	out() << "\t};";
	out() << "\n\n";
}