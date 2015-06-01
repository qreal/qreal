/* Copyright 2007-2015 QReal Research Group
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

#include "edgeType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

#include "association.h"
#include "xmlCompiler.h"
#include "diagram.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "label.h"

using namespace utils;

EdgeType::EdgeType(Diagram *diagram) : GraphicType(diagram)
{
	mIsDividable = "false";
}

EdgeType::~EdgeType()
{
	qDeleteAll(mAssociations);
}


bool EdgeType::copyPorts(NodeType *parent)
{
	Q_UNUSED(parent)	return false;
}

Type* EdgeType::clone() const
{
	EdgeType *result = new EdgeType(mDiagram);
	GraphicType::copyFields(result);
	foreach (Association *association, mAssociations) {
		result->mAssociations.append(new Association(*association));
	}
	result->mBeginType = mBeginType;
	result->mEndType = mEndType;
	result->mLineType = mLineType;
	result->mShapeType = mShapeType;
	result->mFromPorts = mFromPorts;
	result->mToPorts = mToPorts;
	return result;
}

bool EdgeType::copyPictures(GraphicType *parent)
{
	EdgeType *pictureParent = dynamic_cast<EdgeType*>(parent);
	if (pictureParent != nullptr) {
		for (auto association : pictureParent->mAssociations) {
			mAssociations.append(association->clone());
		}

		mLineType = pictureParent->mLineType;
		mShapeType = pictureParent->mShapeType;
		mLineColor = pictureParent->mLineColor;
		mLineWidth = pictureParent->mLineWidth;
		mBeginType = pictureParent->mBeginType;
		mEndType = pictureParent->mEndType;
		mIsDividable = pictureParent->mIsDividable;
		mFromPorts = pictureParent->mFromPorts;
		mToPorts = pictureParent->mToPorts;
		mVisible = true;
		return true;
	}

	return false;
}

bool EdgeType::initAssociations()
{
	QDomElement associationsElement = mLogic.firstChildElement("associations");
	if (associationsElement.isNull()) {
		return true;
	}

	mBeginType = associationsElement.attribute("beginType");
	mEndType = associationsElement.attribute("endType");
	if (mBeginType.isEmpty() || mEndType.isEmpty()) {
		qDebug() << "ERROR: can't parse associations";
		return false;
	}

	for (QDomElement element = associationsElement.firstChildElement("association");
		!element.isNull();
		element = element.nextSiblingElement("association"))
	{
		Association *association = new Association();
		if (!association->init(element)) {
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

	QDomElement shapeType = mGraphics.firstChildElement("shape");
	if (shapeType.isNull()) {
		mShapeType = "square";
	} else {
		mShapeType = shapeType.attribute("type", "square");
	}

	QDomElement lineTypeElement = mGraphics.firstChildElement("lineType");
	if (lineTypeElement.isNull()) {
		mVisible = false;
		return true;
	}

	/* code for setting the width of the edges */
	QDomElement lineWidthElement = mGraphics.firstChildElement("lineWidth");
	if (lineWidthElement.isNull()) {
		mLineWidth = 1;
	} else {
		QString lineWidth = lineWidthElement.attribute("width");
		if (lineWidth.isEmpty()) {
			qDebug() << "ERROR: no width of line";
			return false;
		} else {
			bool success = true;
			int lineWidthInt = lineWidth.toInt(&success);
			if (!success) {
				qDebug() << "ERROR: line width is not a number";
				return false;
			} else if (lineWidthInt <= 0) {
				qDebug() << "ERROR: line width is negative number";
				return false;
			} else {
				mLineWidth = lineWidthInt;
			}
		}
	}

	/* code for setting the color of the edges */
	QDomElement lineColorElement = mGraphics.firstChildElement("lineColor");
	if (lineColorElement.isNull()) {
		mLineColor = Qt::black;
	}
	else {
		QString lineColor = lineColorElement.attribute("color");
		if (lineColor.isEmpty()) {
			qDebug() << "ERROR: no color of line";
			return false;
		} else {
			QColor color = QColor(lineColor);
			if (!color.isValid()) {
				qDebug() << "ERROR: invalid color name of line";
				return false;
			} else {
				mLineColor = color;
			}
		}
	}

	QString lineType = lineTypeElement.attribute("type");
	if (lineType.isEmpty()) {
		qDebug() << "ERROR: no line type";
		return false;
	} else if (lineType == "noPan") {
		lineType = "solidLine";
	}

	mLineType = "Qt::" + lineType.replace(0, 1, lineType.at(0).toUpper());

	return true;
}

bool EdgeType::initDividability()
{
	QDomElement dividabilityElement = mLogic.firstChildElement("dividability");

	mIsDividable = "false";
	if (dividabilityElement.isNull()) {
		return true;
	}

	QString isDividable = dividabilityElement.attribute("isDividable");
	if (isDividable != "true" && isDividable != "false") {
		qDebug() << "ERROR: can't parse dividability";
		return false;
	}

	mIsDividable = isDividable;
	return true;
}

bool EdgeType::initPortTypes()
{
	initPortTypes(mLogic.firstChildElement("fromPorts"), mFromPorts);
	initPortTypes(mLogic.firstChildElement("toPorts"), mToPorts);
	return true;
}

void EdgeType::initPortTypes(const QDomElement &portsElement, QStringList &ports)
{
	ports << "NonTyped";
	if (portsElement.isNull()) {
		return;
	}

	QDomNodeList portNodes = portsElement.elementsByTagName("port");
	for (int i = 0; i < portNodes.size(); i++) {
		QDomElement elem = portNodes.at(i).toElement();
		if (!elem.isNull()) {
			ports << elem.attribute("type");
		}
	}
	ports.removeDuplicates();
}

bool EdgeType::initLabel(Label *label, const QDomElement &element, const int &count)
{
	return label->init(element, count, false, mWidth, mHeight);
}

void EdgeType::generateGraphics() const
{
	QString sdfType = mLineType;
	sdfType.remove("Qt::");
	sdfType.remove("Line").toLower();

	OutFile out("generated/shapes/" + resourceName("Class"));
	out() << "<picture sizex=\"100\" sizey=\"60\" >\n"
			<< "\t<line fill=\""<< mLineColor.name() << "\" stroke-style=\"" << sdfType << "\" stroke=\""
			<< mLineColor.name() <<"\" y1=\"0\" "
			<< "x1=\"0\" y2=\"60\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />\n" << "</picture>";
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>generated/shapes/" + resourceName("Class") + "</file>\n");
}

void EdgeType::generateCode(OutFile &out)
{
	generateGraphics();

	const QString className = NameNormalizer::normalize(qualifiedName());

	out() << "\tclass " << className << " : public qReal::ElementImpl {\n"
	<< "\tpublic:\n";

	if (!mBonusContextMenuFields.empty()) {
		out() << "\t\t" << className << "() {\n";
		out() << "\t\t\tmBonusContextMenuFields";
		foreach (QString elem, mBonusContextMenuFields) {
			out() << " << " << "\"" << elem << "\"";
		}
		out() << ";\n";
		out() << "\t\t}\n\n";
	}

	out() << "\t\tvoid init(QRectF &, const PortFactoryInterface &, QList<PortInterface *> &,\n"
	<< "\t\t\t\t\t\t\t\t\t\t\tqReal::LabelFactoryInterface &, QList<qReal::LabelInterface *> &,\n"
	<< "\t\t\t\t\t\t\t\t\t\t\tqReal::SdfRendererInterface *, qReal::ElementRepoInterface *) {}\n\n"
	<< "\t\tvoid init(qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles)\n\t\t{\n";

	if (!mLabels.isEmpty()) {
		mLabels[0]->generateCodeForConstructor(out);
	} else {
		out() << "\t\t\tQ_UNUSED(titles);\n" << "\t\t\tQ_UNUSED(factory);\n";
	}

	out() << "\t\t}\n\n"
	<< "\t\tvirtual ~" << className << "() {}\n\n"
	<< "\t\tqReal::ElementImpl *clone() { return nullptr; }\n"
	<< "\t\tvoid paint(QPainter *, QRectF &){}\n"
	<< "\t\tbool isNode() const { return false; }\n"
	<< "\t\tbool isResizeable() const { return true; }\n"
	<< "\t\tbool isContainer() const { return false; }\n"
	<< "\t\tbool isDividable() const { return " << mIsDividable << "; }\n"
	<< "\t\tbool isSortingContainer() const { return false; }\n"
	<< "\t\tQVector<int> sizeOfForestalling() const { return QVector<int>(4, 0); }\n"
	<< "\t\tint sizeOfChildrenForestalling() const { return 0; }\n"
	<< "\t\tbool hasMovableChildren() const { return false; }\n"
	<< "\t\tbool minimizesToChildren() const { return false; }\n"
	<< "\t\tbool maximizesChildren() const { return false; }\n"

	<< "\t\tQStringList fromPortTypes() const\n\t\t{\n\t\t\t";
	generatePorts(out, mFromPorts);

	out() << "\t\tQStringList toPortTypes() const\n\t\t{\n\t\t\t";
	generatePorts(out, mToPorts);

	out() << "\t\tenums::linkShape::LinkShape shapeType() const\n\t\t{\n"
	<< "\t\t\treturn enums::linkShape::" << mShapeType << ";\n\t\t}\n";

	out() << "\t\tbool createChildrenFromMenu() const { return false; }\n"
	<< "\t\tQList<double> border() const\n\t\t{\n"
	<< "\t\t\tQList<double> list;\n"
	<< "\t\t\tlist << 0 << 0 << 0 << 0;\n"
	<< "\t\t\treturn list;\n"
	<< "\t\t}\n"
	<< "\t\tint getPenWidth() const { return " << mLineWidth << "; }\n"
	<< "\t\tQColor getPenColor() const { return QColor("
	<< mLineColor.red() << ","
	<< mLineColor.green() << ","
	<< mLineColor.blue()
	<< "); }\n"
	<< "\t\tQt::PenStyle getPenStyle() const { ";

	if (mLineType != "") {
		out() << "return " << mLineType << "; }\n";
	} else {
		out() << "return Qt::SolidLine; }\n";
	}

	out() << "\t\tQStringList bonusContextMenuFields() const\n\t\t{\n" << "\t\t\treturn ";

	if (!mBonusContextMenuFields.empty()) {
		out() << "mBonusContextMenuFields;";
	} else {
		out() << "QStringList();";
	}

	out() << "\n\t\t}\n\n";

	out() << "\tprotected:\n"
	<< "\t\tvirtual void drawStartArrow(QPainter * painter) const\n\t\t{\n";

	generateEdgeStyle(mBeginType, out);

	out() << "\t\tvirtual void drawEndArrow(QPainter * painter) const\n\t\t{\n";

	generateEdgeStyle(mEndType, out);

	out() << "\t\tvoid updateData(qReal::ElementRepoInterface *repo) const\n\t\t{\n";

	if (mLabels.isEmpty()) {
		out() << "\t\t\tQ_UNUSED(repo);\n";
	} else {
		mLabels[0]->generateCodeForUpdateData(out);
	}

	out() << "\t\t}\n\n";
	out() << "\tprivate:\n";

	if (!mBonusContextMenuFields.empty()) {
		out() << "\t\tQStringList mBonusContextMenuFields;\n";
	}

	if (!mLabels.isEmpty()) {
		mLabels[0]->generateCodeForFields(out);
	}

	out() << "\t};\n\n";
}

void EdgeType::generateEdgeStyle(const QString &styleString, OutFile &out)
{
	QString style = styleString;

	if (style.isEmpty()) {
		style = "filled_arrow";
	}

	QStringList bpmnEdges;
	bpmnEdges << "signal" << "timer" << "message" << "error" << "escalation" << "cancel" << "compensation"
			<< "conditional" << "multiple" << "parallel_multiple" << "message_noninterrupting"
			<< "timer_noninterrupting" << "escalation_noninterrupting" << "conditional_noninterrupting"
			<< "signal_noninterrupting" << "multiple_noninterrupting" << "parallel_multiple_noninterrupting";

	out() << "\t\t\tQBrush old = painter->brush();\n"
	"\t\t\tQBrush brush;\n";
	out() << "\t\t\tbrush.setStyle(Qt::SolidPattern);\n";

	if (style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow" || style == "empty_circle"
			|| bpmnEdges.contains(style)) {
		out() << "\t\t\tbrush.setColor(Qt::white);\n";
	}

	if (style == "filled_arrow" || style == "filled_rhomb") {
		out() << "\t\t\tbrush.setColor(Qt::black);\n";
	}
	out() << "\t\t\tpainter->setBrush(brush);\n";

	if (style == "empty_arrow" || style == "filled_arrow") {
		out() << "\t\t\tstatic const QPointF points[] = {\n"
		"\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(-5,10),\n\t\t\t\tQPointF(5,10)\n\t\t\t};\n"
		"\t\t\tpainter->drawPolygon(points, 3);\n";
	}

	if (style == "empty_rhomb" || style == "filled_rhomb") {
		out() << "\t\t\tstatic const QPointF points[] = {\n"
		"\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(-5,10),\n\t\t\t\tQPointF(0,20),\n\t\t\t\tQPointF(5,10)\n\t\t\t"
		"};\n"
		"\t\t\tpainter->drawPolygon(points, 4);\n";
	}

	if (style == "open_arrow") {
		out() << "\t\t\tstatic const QPointF points[] = {\n"
		"\t\t\t\tQPointF(-5,10),\n\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(5,10)\n\t\t\t};\n"
		"\t\t\tpainter->drawPolyline(points, 3);\n";
	}

	if (style == "complex_arrow") {
		out() << "\t\t\tstatic const QPointF points[] = {"
		"\n\t\t\t\tQPointF(-15,30),\n\t\t\t\tQPointF(-10,10),"
		"\n\t\t\t\tQPointF(0,0),\n\t\t\t\tQPointF(10,10),"
		"\n\t\t\t\tQPointF(15,30),\n\t\t\t\tQPointF(0,23),\n\t\t\t\tQPointF(-15,30)\n\t\t\t};\n"
		"\t\t\tpainter->drawPolyline(points, 7);\n";
	}

	if (style == "crossed_line") {
		out() << "\t\t\tQPen oldPen = painter->pen();\n"
		"\t\t\tQPen newPen = oldPen;\n"
		"\t\t\tnewPen.setWidth(2);\n"
		"\t\t\tpainter->setPen(newPen);\n"
		"\t\t\tpainter->drawLine(5, 5, -5, 15);\n"
		"\t\t\tpainter->setPen(oldPen);\n";
	}

	if (style == "empty_circle") {
		out() << "\t\t\tpainter->drawEllipse(-5, 0, 10, 10);\n";
	}

	if (bpmnEdges.contains(style)) {
		QStringList parts = style.split('_');
		out() << "\t\t\tpainter->save();\n";
		if (parts.last() == "noninterrupting") {
			out() << "\t\t\tQPen dashPen = painter->pen();\n"
					 "\t\t\tdashPen.setStyle(Qt::DashLine);\n"
					 "\t\t\tpainter->setPen(dashPen);\n";
			parts.removeLast();
			style = parts.join('_');
		}

		out() << "\t\t\tpainter->drawEllipse(-20, 0, 40, 40);\n"
				"\t\t\tpainter->drawEllipse(-15, 5, 30, 30);\n";

		out() << "\t\t\tQPen solidPen = painter->pen();\n"
				 "\t\t\tsolidPen.setStyle(Qt::SolidLine);\n"
				 "\t\t\tpainter->setPen(solidPen);\n";

		if (style == "signal") {
			out() << "\t\t\tstatic const QPointF points[] = {"
					 "\n\t\t\t\tQPointF(0, 10),\n\t\t\t\tQPointF(-10, 27),"
					 "\n\t\t\t\tQPointF(10, 27),\n\t\t\t\tQPointF(0, 10)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points, 4);\n";
		}

		if (style == "timer") {
			out() << "\t\t\tpainter->drawEllipse(-10, 10, 20, 20);\n"
					 "\t\t\tpainter->drawLine(0, 20, 0, 15);\n"
					 "\t\t\tpainter->drawLine(0, 20, 10, 20);\n";
		}

		if (style == "message") {
			out() << "\t\t\tpainter->drawRect(-10, 15, 20, 11);\n"
					 "\t\t\tpainter->drawLine(-9, 16, -1, 19);\n"
					 "\t\t\tpainter->drawLine(9, 16, 1, 19);\n";
		}

		if (style == "error") {
			out() << "\t\t\tstatic const QPointF points[] = {"
					 "\n\t\t\t\tQPointF(-10, 28),\n\t\t\t\tQPointF(-4, 10),"
					 "\n\t\t\t\tQPointF(3, 20),\n\t\t\t\tQPointF(10, 12),"
					 "\n\t\t\t\tQPointF(4, 30),\n\t\t\t\tQPointF(-3, 20),"
					 "\n\t\t\t\tQPointF(-10, 28)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points, 7);\n";
		}

		if (style == "escalation") {
			out() << "\t\t\tstatic const QPointF points[] = {"
					 "\n\t\t\t\tQPointF(0, 22),\n\t\t\t\tQPointF(-6, 30),"
					 "\n\t\t\t\tQPointF(0, 10),\n\t\t\t\tQPointF(6, 30),"
					 "\n\t\t\t\tQPointF(0, 22)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points, 5);\n";
		}

		if (style == "cancel") {
			out() << "\t\t\tstatic const QPointF points[] = {"
					 "\n\t\t\t\tQPointF(-12, 12),\n\t\t\t\tQPointF(-8, 8),"
					 "\n\t\t\t\tQPointF(0, 17),\n\t\t\t\tQPointF(8, 8),"
					 "\n\t\t\t\tQPointF(12, 12),\n\t\t\t\tQPointF(3, 20),"
					 "\n\t\t\t\tQPointF(12, 28),\n\t\t\t\tQPointF(8, 32),"
					 "\n\t\t\t\tQPointF(0, 23),\n\t\t\t\tQPointF(-8, 32),"
					 "\n\t\t\t\tQPointF(-12, 28),\n\t\t\t\tQPointF(-3, 20),"
					 "\n\t\t\t\tQPointF(-12, 12)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points, 13);\n";
		}

		if (style == "compensation") {
			out() << "\t\t\tstatic const QPointF points1[] = {"
					 "\n\t\t\t\tQPointF(-12, 20),\n\t\t\t\tQPointF(-2, 10),"
					 "\n\t\t\t\tQPointF(-2, 30),\n\t\t\t\tQPointF(-12, 20)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points1, 4);\n";
			out() << "\t\t\tstatic const QPointF points2[] = {"
					 "\n\t\t\t\tQPointF(-2, 20),\n\t\t\t\tQPointF(8, 10),"
					 "\n\t\t\t\tQPointF(8, 30),\n\t\t\t\tQPointF(-2, 20)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points2, 4);\n";
		}

		if (style == "conditional") {
			out() << "\t\t\tpainter->drawRect(-8, 10, 16, 20);\n"
					 "\t\t\tpainter->drawLine(-6, 14, 6, 14);\n"
					 "\t\t\tpainter->drawLine(-6, 20, 6, 20);\n"
					 "\t\t\tpainter->drawLine(-6, 26, 6, 26);\n";
		}

		if (style == "multiple") {
			out() << "\t\t\tstatic const QPointF points[] = {"
					 "\n\t\t\t\tQPointF(0, 11),\n\t\t\t\tQPointF(10, 17),"
					 "\n\t\t\t\tQPointF(7, 29),\n\t\t\t\tQPointF(-7, 29),"
					 "\n\t\t\t\tQPointF(-10, 17),\n\t\t\t\tQPointF(0, 11)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points, 6);\n";
		}

		if (style == "parallel_multiple") {
			out() << "\t\t\tstatic const QPointF points[] = {"
					 "\n\t\t\t\tQPointF(-12, 17),\n\t\t\t\tQPointF(-3, 17),"
					 "\n\t\t\t\tQPointF(-3, 8),\n\t\t\t\tQPointF(3, 8),"
					 "\n\t\t\t\tQPointF(3, 17),\n\t\t\t\tQPointF(12, 17),"
					 "\n\t\t\t\tQPointF(12, 23),\n\t\t\t\tQPointF(3, 23),"
					 "\n\t\t\t\tQPointF(3, 32),\n\t\t\t\tQPointF(-3, 32),"
					 "\n\t\t\t\tQPointF(-3, 23),\n\t\t\t\tQPointF(-12, 23),"
					 "\n\t\t\t\tQPointF(-12, 17)\n\t\t\t};\n"
					 "\t\t\tpainter->drawPolyline(points, 13);\n";
		}

		out() << "\t\t\tpainter->restore();\n";
	}

	out() << "\t\t\tpainter->setBrush(old);\n\t\t}\n\n";
}

void EdgeType::generatePorts(OutFile &out, const QStringList &portTypes)
{
	out() << "QStringList result;\n"
		  << "\t\t\tresult";

	foreach (const QString &type, portTypes) {
		out() << " << \"" << type << "\"";
	}

	out() << ";\n\t\t\treturn result;\n\t\t}\n";
}
