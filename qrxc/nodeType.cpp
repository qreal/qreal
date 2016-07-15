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

#include "nodeType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

#include "diagram.h"
#include "xmlCompiler.h"
#include "pointPort.h"
#include "linePort.h"
#include "circularPort.h"
#include "editor.h"
#include "nameNormalizer.h"
#include "label.h"

using namespace utils;

NodeType::NodeType(Diagram *diagram)
		: GraphicType(diagram)
		, mIsResizeable(true)
{
}

NodeType::~NodeType()
{
	qDeleteAll(mPointPorts);
	qDeleteAll(mLinePorts);
	qDeleteAll(mCircularPorts);
}

Type* NodeType::clone() const
{
	NodeType *result = new NodeType(mDiagram);
	GraphicType::copyFields(result);

	for (Port *port : mPointPorts) {
		result->mPointPorts << port->clone();
	}

	for (Port *port : mLinePorts) {
		result->mLinePorts << port->clone();
	}

	for (Port *port : mCircularPorts) {
		result->mCircularPorts << port->clone();
	}

	result->mSdfDomElement = mSdfDomElement;
	result->mPortsDomElement = mPortsDomElement;
	result->mIsResizeable = mIsResizeable;
	return result;
}

bool NodeType::copyPictures(GraphicType *parent)
{
	const NodeType * const nodeParent = dynamic_cast<NodeType*>(parent);
	if (nodeParent != nullptr) {
		if (mSdfDomElement.isNull()) {
			/// @todo Support this.
			if (!nodeParent->mSdfDomElement.isNull()) {
				qDebug() << name()
						<< ": Inheriting pictures for an element without <picture> tag is not currently supported";
			}
		} else {
			mWidth = qMax(mWidth, nodeParent->mWidth);
			mHeight = qMax(mHeight, nodeParent->mHeight);

			for (QDomNode sdfPrimitive = nodeParent->mSdfDomElement.firstChild();
					!sdfPrimitive.isNull();
					sdfPrimitive = sdfPrimitive.nextSibling())
			{
				mSdfDomElement.appendChild(sdfPrimitive.cloneNode());
			}

			mVisible = mVisible || nodeParent->mVisible;
		}

		return true;
	}

	return false;
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
	return initSdf() && initPorts() && initBooleanProperties();
}

bool NodeType::initSdf()
{
	QDomElement sdfElement = mGraphics.firstChildElement("picture");
	if (!sdfElement.isNull()) {
		mWidth = sdfElement.attribute("sizex").toInt();
		mHeight = sdfElement.attribute("sizey").toInt();
		mSdfDomElement = sdfElement;
		mVisible = true;
	} else {
		mVisible = false;
	}

	return true;
}

void NodeType::generateSdf() const
{
	mDiagram->editor()->xmlCompiler()->addResource("\t<file>generated/shapes/" + resourceName("Class") + "</file>\n");

	OutFile out("generated/shapes/" + resourceName("Class"));
	mSdfDomElement.save(out(), 1);
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
	initCircularPorts(portsElement);

	return true;
}

bool NodeType::initPointPorts(const QDomElement &portsElement)
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

		mPointPorts << pointPort;
	}

	return true;
}

bool NodeType::initLinePorts(const QDomElement &portsElement)
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

		mLinePorts << linePort;
	}

	return true;
}

bool NodeType::initCircularPorts(const QDomElement &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("circularPort");
			!portElement.isNull();
			portElement = portElement.nextSiblingElement("circularPort"))
	{
		Port *circularPort = new CircularPort();
		if (!circularPort->init(portElement, mWidth, mHeight)) {
			delete circularPort;
			return false;
		}

		mCircularPorts.append(circularPort);
	}

	return true;
}

bool NodeType::initLabel(Label *label, const QDomElement &element, const int &count)
{
	return label->init(element, count, true, mWidth, mHeight);
}

bool NodeType::initBooleanProperties()
{
	mIsResizeable = true;

	const QDomElement element2 = mGraphics.firstChildElement("nonResizeable");
	if (!element2.isNull()) {
		mIsResizeable = false;
	}

	return true;
}

void NodeType::generateCode(OutFile &out)
{
	generateSdf();

	const QString className = NameNormalizer::normalize(qualifiedName());

	out() << "\tclass " << className << " : public qReal::NodeElementType\n\t{\n"
			<< "\tpublic:\n";

	out() << "\t\texplicit " << className << "(qReal::Metamodel &metamodel)\n"
			<< "\t\t\t: NodeElementType(metamodel)\n"
			<< "\t\t{\n";

	generateCommonData(out);

	out() << "\t\t\tloadSdf(utils::xmlUtils::loadDocument(\":/generated/shapes/"
			+ className + "Class.sdf\").documentElement());\n"
			<< "\t\t\tsetSize(QSizeF(" + QString::number(mWidth) + ", " + QString::number(mHeight) + "));\n"
			<< "\t\t\tinitProperties();\n";

	generateMouseGesture(out);

	for (Port * const pointPort : mPointPorts) {
		out() << "\t\t\taddPointPort(";
		pointPort->generateCode(out, mDiagram->editor()->getAllPortNames());
		out() << ");\n";
	}

	for (Port * const linePort : mLinePorts) {
		out() << "\t\t\taddLinePort(";
		linePort->generateCode(out, mDiagram->editor()->getAllPortNames());
		out() << ");\n";
	}

	for (Port * const circlePort : mCircularPorts) {
		out() << "\t\t\taddCircularPort(";
		circlePort->generateCode(out, mDiagram->editor()->getAllPortNames());
		out() << ");\n";
	}

	out() << "\t\t\tsetResizable(" << boolToString(mIsResizeable) << ");\n"
			<< "\t\t\tsetContainer(" << boolToString(!mContains.empty()) << ");\n"
			<< "\t\t\tsetSortingContainer(" << boolToString(mContainerProperties.isSortingContainer) << ");\n";

	QStringList forestalling;
	for (int size : mContainerProperties.sizeOfForestalling) {
		forestalling << QString::number(size);
	}

	out() << "\t\t\tsetSizeOfForestalling({" << forestalling.join(", ") << "});\n"
			<< "\t\t\tsetSizeOfChildrenForestalling("
			<< QString::number(mContainerProperties.sizeOfChildrenForestalling) << ");\n"
			<< "\t\t\tsetChildrenMovable(" << boolToString(mContainerProperties.hasMovableChildren) << ");\n"
			<< "\t\t\tsetMinimizesToChildren(" << boolToString(mContainerProperties.minimizesToChildren) << ");\n"
			<< "\t\t\tsetMaximizesChildren(" << boolToString(mContainerProperties.maximizesChildren) << ");\n"
			<< "\t\t\tsetCreateChildrenFromMenu(" << boolToString(mCreateChildrenFromMenu) << ");\n"
			/// @todo: borders are strange things available only in qrmc. Do we need it?
			<< "\t\t\tsetBorder({});\n"
			<< "\t\t}\n\n";

	generatePropertyData(out);

	out() << "\t};";
	out() << "\n\n";
}

QList<Port *> NodeType::ports() const
{
	return mPointPorts + mLinePorts + mCircularPorts;
}

bool NodeType::copyPorts(NodeType* parent)
{
	for (Port * const port : parent->mPointPorts) {
		mPointPorts << port->clone();
	}

	for (Port * const port : parent->mLinePorts) {
		mLinePorts << port->clone();
	}

	for (Port * const port : parent->mCircularPorts) {
		mCircularPorts << port->clone();
	}

	return !parent->mPointPorts.isEmpty() || !parent->mLinePorts.isEmpty() || !parent->mCircularPorts.isEmpty();
}

void NodeType::generateMouseGesture(OutFile &out)
{
	QString gesturePath = path();

	const QString output = "\t\t\tsetMouseGesture(";
	out() << output;
	if (gesturePath.length() > maxLineLength - output.length()) {
		out() << "\"" << gesturePath.left(maxLineLength - output.length());
		gesturePath.remove(0, maxLineLength - output.length());
		const QString prefix = "\t\t\t\"";
		do {
			out() << "\"\n" << prefix << gesturePath.left(maxLineLength);
			gesturePath.remove(0, maxLineLength);
		} while (gesturePath.length() > maxLineLength);

		if (gesturePath.length() > 0) {
			out() << "\"\n" << prefix << gesturePath;
		}

	} else {
		out() << "\"" << gesturePath;
	}

	out() << "\");\n";
}
