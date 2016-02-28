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
	for (QDomElement portElement = portsElement.firstChildElement("pointPort");
			!portElement.isNull();
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

bool NodeType::initLinePorts(const QDomElement &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("linePort");
			!portElement.isNull();
			portElement = portElement.nextSiblingElement("linePort"))
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
	bool hasSdf = false;

	out() << "\tclass " << className << " : public qReal::ElementType\n\t{\n"
			<< "\tpublic:\n";

	if (!mBonusContextMenuFields.empty()) {
		out() << "\t\t" << className << "()\n\t\t{\n";
		out() << "\t\t\tmBonusContextMenuFields";
		foreach (QString elem, mBonusContextMenuFields) {
			out() << " << " << "\"" << elem << "\"";
		}
		out() << ";\n";
		out() << "\t\t}\n\n";
	}

	out () << "\t\tvoid init(qReal::LabelFactoryInterface &, QList<qReal::LabelInterface*> &) {}\n\n"
	<< "\t\tvoid init(QRectF &contents, const PortFactoryInterface &portFactory, QList<PortInterface *> &ports\n"
	<< "\t\t\t\t\t\t\t, qReal::LabelFactoryInterface &factory, QList<qReal::LabelInterface*> &titles\n"
	<< "\t\t\t\t\t\t\t, qReal::SdfRendererInterface *renderer, qReal::ElementRepoInterface *elementRepo)\n\t\t{\n";

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

	out() << "\t\t\tcontents.setWidth(" << mWidth << ");\n"
	<< "\t\t\tcontents.setHeight(" << mHeight << ");\n";

	foreach (Port *port, mPorts) {
		port->generateCode(out, mDiagram->editor()->getAllPortNames());
	}

	foreach (Label *label, mLabels) {
		label->generateCodeForConstructor(out);
	}

	out() << "\t\t}\n\n";

	out() << "\t\t qReal::ElementType *clone() { return nullptr; }\n";

	out() << "\t\t~" << className << "() {}\n\n"
	<< "\t\tvoid paint(QPainter *painter, QRectF &contents)\n\t\t{\n";

	if (hasSdf) {
		out() << "\t\t\tmRenderer->render(painter, contents);\n";
	}

	out() << "\t\t}\n\n";

	out() << "\t\tQt::PenStyle getPenStyle() const { return Qt::SolidLine; }\n\n"
	<< "\t\tint getPenWidth() const { return 0; }\n\n"
	<< "\t\tQColor getPenColor() const { return QColor(); }\n\n"
	<< "\t\tvoid drawStartArrow(QPainter *) const {}\n"
	<< "\t\tvoid drawEndArrow(QPainter *) const {}\n\n"

	<< "\t\tvoid updateData(qReal::ElementRepoInterface *repo) const\n\t\t{\n"
	<< "\t\t\tmRenderer->setElementRepo(repo);\n";

	if (mLabels.isEmpty()) {
		out() << "\t\t\tQ_UNUSED(repo);\n";
	} else {
		foreach (Label *label, mLabels) {
			label->generateCodeForUpdateData(out);
		}
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

	<< "\t\tbool isDividable() const\n\t\t{\n\t\t\treturn false;\n\t\t}\n\n"

	<< "\t\tQStringList fromPortTypes() const\n\t\t{\n\t\t\treturn QStringList(\"NonTyped\");\n\t\t}\n\n"

	<< "\t\tQStringList toPortTypes() const\n\t\t{\n\t\t\treturn QStringList(\"NonTyped\");\n\t\t}\n\n"

	<< "\t\tenums::linkShape::LinkShape shapeType() const\n\t\t{\n\t\t\treturn enums::linkShape::square;\n\t\t}\n\n"

	<< "\t\tbool createChildrenFromMenu() const\n\t\t{\n"
	<< (mCreateChildrenFromMenu ? "\t\t\treturn true;\n" : "\t\t\treturn false;\n")
	<< "\t\t}\n\n";

	out() << "\t\tQList<double> border() const\n\t\t{\n"
	<< "\t\t\tQList<double> list;\n";

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
		out() << "\t\tqReal::SdfRendererInterface *mRenderer;\n";
	}

	foreach (Label *label, mLabels) {
		label->generateCodeForFields(out);
	}

	out() << "\t};";
	out() << "\n\n";
}

QList<Port *> NodeType::ports() const
{
	return mPorts;
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
		foreach (const QString &type, portTypes) {
			out() << "<< \"" << type << "\"";
		}
		out() << ";\n";
	}

	out() << "\t}\n";
	return true;
}

bool NodeType::copyPorts(NodeType* parent)
{
	for (Port *port: parent->mPorts) {
		mPorts.append(port->clone());
	}

	return !parent->mPorts.isEmpty();
}
