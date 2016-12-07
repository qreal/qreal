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

#include "roleType.h"
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
	qDeleteAll(mRoles);
}


bool EdgeType::copyPorts(NodeType *parent)
{
	Q_UNUSED(parent)
	return false;
}

Type *EdgeType::clone() const
{
	EdgeType *result = new EdgeType(mDiagram);
	GraphicType::copyFields(result);
	for (RoleType * const role : mRoles) {
		result->mRoles.append(role->clone());
	}
	result->mBeginArrowType = mBeginArrowType;
	result->mBeginRoleName = mBeginRoleName;
	result->mEndArrowType = mEndArrowType;
	result->mEndRoleName = mEndRoleName;
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
		for (auto role : pictureParent->mRoles) {
			mRoles.append(role->clone());
		}

		mLineType = pictureParent->mLineType;
		mShapeType = pictureParent->mShapeType;
		mLineColor = pictureParent->mLineColor;
		mLineWidth = pictureParent->mLineWidth;

		mBeginArrowType = pictureParent->mBeginArrowType;
		mBeginRoleName = pictureParent->mBeginRoleName;
		mEndArrowType = pictureParent->mEndArrowType;
		mEndRoleName = pictureParent->mEndRoleName;
		mIsDividable = pictureParent->mIsDividable;
		mFromPorts = pictureParent->mFromPorts;
		mToPorts = pictureParent->mToPorts;
		mVisible = true;
		return true;
	}

	return false;
}

bool EdgeType::initRoles()
{
	QDomElement beginRoleElement = mLogic.firstChildElement("beginRole");
	QDomElement endRoleElement = mLogic.firstChildElement("endRole");

	mBeginRoleName = beginRoleElement.attribute("role");
	mEndRoleName = endRoleElement.attribute("role");
	QList<Type*> allExistingTypes = mDiagram->types().values();

	for (auto element : allExistingTypes) {
		QString name = element->displayedName();

		if (name == mBeginRoleName || name == mEndRoleName) {
			RoleType *temp = dynamic_cast<RoleType *> (element->clone());
			mRoles.append(temp);
		}
	}

	for (auto role : mRoles) {
		if (role->name() == mBeginRoleName && role->name() == mEndRoleName) {
			mBeginArrowType = role->typeOfArrow();
			mEndArrowType = role->typeOfArrow();
		} else if (role->name() == mBeginRoleName) {
			mBeginArrowType = role->typeOfArrow();
		} else if (role->name() == mEndRoleName) {
			mEndArrowType = role->typeOfArrow();
		}
	}

	return true;
}

bool EdgeType::initRoleProperties()
{
	for (RoleType *role : mRoles) {
		for (Property *property : role->getPropertiesOfRole()) {
			addProperty(property, role->name());
		}
	}

	return true;
}

QString EdgeType::propertyName(Property *property, const QString &roleName)
{
	for (RoleType* const role : mRoles) {
		if (role->name() == roleName) {
			for (Property* const currentProperty : role->getPropertiesOfRole()) {
				if (currentProperty->name() == property->name()) {
					return role->name() + "!" + property->name();
				}
			}
		}
	}

	return "";
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

void EdgeType::generateCode(OutFile &out)
{
	const QString className = NameNormalizer::normalize(qualifiedName());

	out() << "\tclass " << className << " : public qReal::EdgeElementType \n\t{\n"
	<< "\tpublic:\n"

	<< "\t\texplicit " << className << "(qReal::Metamodel &metamodel)\n"
			<< "\t\t\t: EdgeElementType(metamodel)\n"
			<< "\t\t{\n";

	generateCommonData(out);
	generatePorts(out, mFromPorts, "From");
	generatePorts(out, mToPorts, "To");
	out() << "\t\t\tsetShapeType(qReal::LinkShape::" << mShapeType << ");\n"
			<< "\t\t\tsetPenWidth(" << mLineWidth << ");\n"
			<< "\t\t\tsetPenColor(QColor("
					<< mLineColor.red() << ","
					<< mLineColor.green() << ","
					<< mLineColor.blue()
			<< "));\n"
			<< "\t\t\tsetPenStyle(" << (mLineType.isEmpty() ? "Qt::SolidLine" : mLineType) << ");\n"
			<< "\t\t\tsetDividable(" << mIsDividable << ")\n;"
			<< "\t\t\tinitProperties();\n"
			<< "\t\t}\n\n"

	<< "\t\tvirtual ~" << className << "() {}\n\n";

	out() << "\t\tvoid drawStartArrow(QPainter * painter) const override\n\t\t{\n";
	generateEdgeStyle(mBeginArrowType, out);

	out() << "\t\tvoid drawEndArrow(QPainter * painter) const override\n\t\t{\n";
	generateEdgeStyle(mEndArrowType, out);

	out() << "\tprivate:\n";
	generatePropertyData(out);

	out() << "\t};\n\n";
}

void EdgeType::generateLabels(OutFile &out) const
{
	for (Label *label : mLabels) {
		if (label->location() == "beginRole") {
			label->setRoleName(mBeginRoleName);
		} else if(label->location() == "endRole") {
			label->setRoleName(mEndRoleName);
		}

		label->generateCodeForConstructor(out);
	}
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

void EdgeType::generatePorts(OutFile &out, const QStringList &portTypes, const QString &direction)
{
	out() << "\t\t\tset" << direction << "PortTypes({";
	for (const QString &type : portTypes) {
		out() << "\"" << type << "\", ";
	}

	out() << "});\n";
}
