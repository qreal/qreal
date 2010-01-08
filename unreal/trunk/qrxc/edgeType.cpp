#include "edgeType.h"
#include "association.h"
#include "../utils/outFile.h"
#include "xmlCompiler.h"
#include "diagram.h"
#include "editor.h"
#include "nameNormalizer.h"

#include <QDebug>

using namespace utils;

EdgeType::EdgeType(Diagram *diagram) : GraphicType(diagram)
{}

EdgeType::~EdgeType()
{
	foreach (Association *association, mAssociations)
		delete association;
}

Type* EdgeType::clone() const
{
	EdgeType *result = new EdgeType(mDiagram);
	GraphicType::copyFields(result);
	foreach (Association *association, mAssociations)
		result->mAssociations.append(new Association(*association));
	result->mBeginType = mBeginType;
	result->mEndType = mEndType;
	result->mLineType = mLineType;
	return result;
}

bool EdgeType::initAssociations()
{
	QDomElement associationsElement = mLogic.firstChildElement("associations");
	if (associationsElement.isNull())
	{
		return true;
	}
	mBeginType = associationsElement.attribute("beginType");
	mEndType = associationsElement.attribute("endType");
	if ((mBeginType == "") || (mEndType == ""))
	{
		qDebug() << "ERROR: can't parse associations";
		return false;
	}
	for (QDomElement element = associationsElement.firstChildElement("association"); !element.isNull();
		element = element.nextSiblingElement("association"))
	{
		Association *association = new Association();
		if (!association->init(element))
		{
			delete association;
			return false;
		}
		mAssociations.append(association);
	}
	return true;
}

bool EdgeType::initGraphics()
{
	mVisible = true;
	QDomElement lineTypeElement = mGraphics.firstChildElement("lineType");
	if (lineTypeElement.isNull())
	{
		mVisible = false;
		return true;
	}
	QString lineType = lineTypeElement.attribute("type");
	if (lineType == "")
	{
		qDebug() << "ERROR: no line type";
		return false;
	}
	else if (lineType == "noPan")
		lineType = "solidLine";
	mLineType = "Qt::" + lineType.replace(0, 1, lineType.at(0).toUpper());
	return true;
}

void EdgeType::generateGraphics() const
{
	QString sdfType = mLineType;
	sdfType.remove("Qt::");
	sdfType.remove("Line").toLower();

	OutFile out("generated/shapes/" + resourceName("Class"));
	out() << "<picture sizex=\"100\" sizey=\"60\" >\n" <<
		"\t<line fill=\"#000000\" stroke-style=\"" << sdfType << "\" stroke=\"#000000\" y1=\"0\" " <<
		"x1=\"0\" y2=\"60\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />\n" <<
		"</picture>";
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>" + resourceName("Class") + "</file>\n");
}

void EdgeType::generateCode(OutFile &out)
{
	generateGraphics();

	QString const className = NameNormalizer::normalize(qualifiedName());

	out() << "\tclass " << className << " : public EdgeElement {\n"
		<< "\tpublic:\n"
		<< "\t\t" << className << "() {\n";
		if (mLineType != "")
		{
			out() << "\t\t\tmPenStyle = " << mLineType <<  ";\n";
		}

		out() << "\t\t}\n\n"

		<< "\t\tvirtual ~" << className << "() {}\n\n"

		<< "\tprotected:\n"
		<< "\t\tvirtual void drawStartArrow(QPainter * painter) const {\n";

		generateEdgeStyle(mBeginType, out);

		out() << "\t\tvirtual void drawEndArrow(QPainter * painter) const {\n";

		generateEdgeStyle(mEndType, out);

		out()<< "\t};\n\n";
}

void EdgeType::generateEdgeStyle(QString const &styleString, OutFile &out)
{
	QString style = styleString;

	if (style.isEmpty())
		style = "filled_arrow";

	out() << "\t\t\tQBrush old = painter->brush();\n"
		"\t\t\tQBrush brush;\n"
		"\t\t\tbrush.setStyle(Qt::SolidPattern);\n";

	if (style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow")
		out() << "\t\t\tbrush.setColor(Qt::white);\n";

	if (style == "filled_arrow" || style == "filled_rhomb")
		out() << "\t\t\tbrush.setColor(Qt::black);\n";
	out() << "\t\t\tpainter->setBrush(brush);\n";

	if (style == "empty_arrow" || style == "filled_arrow")
		out() << "\t\t\tstatic const QPointF points[] = {\n"
		"\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(-5,10),\n\t\t\t\tQPointF(5,10)\n\t\t\t};\n"
		"\t\t\tpainter->drawPolygon(points, 3);\n";

	if (style == "empty_rhomb" || style == "filled_rhomb")
		out() << "\t\t\tstatic const QPointF points[] = {\n"
		"\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(-5,10),\n\t\t\t\tQPointF(0,20),\n\t\t\t\tQPointF(5,10)\n\t\t\t"
		"};\n"
		"\t\t\tpainter->drawPolygon(points, 4);\n";

	if (style == "open_arrow")
		out() << "\t\t\tstatic const QPointF points[] = {\n"
		"\t\t\t\tQPointF(-5,10),\n\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(5,10)\n\t\t\t};\n"
		"\t\t\tpainter->drawPolyline(points, 3);\n";

	if (style == "complex_arrow")
		out() << "\t\t\tstatic const QPointF points[] = {"
		"\n\t\t\t\tQPointF(-15,30),\n\t\t\t\tQPointF(-10,10),"
		"\n\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(10,10),"
		"\n\t\t\t\tQPointF(15,30),\n\t\t\t\tQPointF(0,23),\n\t\t\t\tQPointF(-15,30)\n\t\t\t};\n"
		"\t\t\tpainter->drawPolyline(points, 7);\n";
	out() << "\t\t\tpainter->setBrush(old);\n\t\t}\n\n";
}
