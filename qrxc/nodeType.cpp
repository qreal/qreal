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
	} else
		mVisible = false;
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
			<< "\t\t{\n"
			<< "\t\t}\n\n";

	out() << "\t\tQString sdfFile() const override { return \":/generated/shapes/" + className + "Class.sdf\"; }\n\n";

	out() << "\t\tQSizeF size() const override { return QSizeF(" + QString::number(mWidth)
			+ ", " + QString::number(mHeight) + "); }\n\n";

	generateLabels(out);

	out() << "\t\tQList<qReal::PointPortInfo> pointPorts() const override\n\t\t{\n";
	out() << "\t\t\treturn {\n";
	for (Port * const pointPort : mPointPorts) {
		out() << "\t\t\t";
		pointPort->generateCode(out, mDiagram->editor()->getAllPortNames());
		out() << ",\n";
	}

	out() << "\t\t\t};\n\t\t}\n\n";

	out() << "\t\tQList<qReal::LinePortInfo> linePorts() const override\n\t\t{\n";
	out() << "\t\t\treturn {\n";
	for (Port * const linePort : mLinePorts) {
		out() << "\t\t\t\t";
		linePort->generateCode(out, mDiagram->editor()->getAllPortNames());
		out() << ",\n";
	}

	out() << "\t\t\t};\n\t\t}\n\n";

	out() << "\t\tbool isResizeable() const\n\t\t{\n"
	<< "\t\t\treturn " << (mIsResizeable ? "true" : "false") << ";\n"
	<< "\t\t}\n\n"

	<< "\t\tbool isContainer() const\n\t\t{\n"
	<< (!mContains.empty() ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n"

	<< "\t\tbool isSortingContainer() const\n\t\t{\n"
	<< (mContainerProperties.isSortingContainer ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n";

	QStringList forestalling;
	foreach (int size, mContainerProperties.sizeOfForestalling) {
		forestalling << QString::number(size);
	}

	out() << "\t\tQVector<int> sizeOfForestalling() const\n\t\t{\n"
	<< "\t\t\tQVector<int> result;\n"
	<< "\t\t\tresult << " + forestalling[0] + " << " + forestalling[1] + " << " + forestalling[2]
			+ " << " + forestalling[3] + ";\n"
	<< ";\n\t\t\treturn result;\n"
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

	<< "\t\tbool createChildrenFromMenu() const\n\t\t{\n"
	<< (mCreateChildrenFromMenu ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n";

	out() << "\t\tQList<double> border() const\n\t\t{\n"
	<< "\t\t\tQList<double> list;\n";

	out() << "\t\t\treturn list;\n"
	<< "\t\t}\n\n";

	out() << "\t};";
	out() << "\n\n";
}

QList<Port *> NodeType::ports() const
{
	return mPointPorts + mLinePorts;
}

bool NodeType::generatePorts(OutFile &out, bool isNotFirst)
{
	GraphicType::generateOneCase(out, isNotFirst);

	QSet<QString> portTypes;
	for (const Port *port : mPointPorts) {
		portTypes.insert(port->type());
	}

	for (const Port *port : mLinePorts) {
		portTypes.insert(port->type());
	}

	if (!portTypes.empty()) {
		out() << "\t\tresult ";
		for (const QString &type : portTypes) {
			out() << "<< \"" << type << "\"";
		}

		out() << ";\n";
	}

	out() << "\t}\n";
	return true;
}

bool NodeType::copyPorts(NodeType* parent)
{
	for (Port * const port : parent->mPointPorts) {
		mPointPorts << port->clone();
	}

	for (Port * const port : parent->mLinePorts) {
		mLinePorts << port->clone();
	}

	return !parent->mPointPorts.isEmpty() || !parent->mLinePorts.isEmpty();
}
