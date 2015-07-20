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

#include "shape.h"
#include "../utils/defs.h"
#include "../diagram.h"
#include "../metaCompiler.h"
#include "../editor.h"
#include "graphicType.h"
#include "../utils/nameNormalizer.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

using namespace qrmc;

Shape::Shape(const QString &shape) : mNode(nullptr)
{
	init(shape)	;
}

Shape::~Shape()
{
}

void Shape::setNode(GraphicType *node)
{
	mNode = node;
}

void Shape::init(const QString &shape)
{
	if (shape.isEmpty())
		return;

	QString error = "";
	int errorLine = 0;
	int errorCol = 0;
	QDomDocument doc;
	if (!doc.setContent(shape, false, &error, &errorLine, &errorCol)) {
		return;
	}

	QDomElement graphics = doc.firstChildElement("graphics");

	QDomElement picture = graphics.firstChildElement("picture");
	QTextStream out(&mPicture);
	picture.save(out, 4);

	mWidth = graphics.firstChildElement("picture").attribute("sizex", "88").toInt();
	mHeight = graphics.firstChildElement("picture").attribute("sizey", "88").toInt();

	initLabels(graphics);
	initPorts(graphics);
}

void Shape::initLabels(const QDomElement &graphics)
{
	int count = 1;
	for (QDomElement element = graphics.firstChildElement("labels").firstChildElement("label");
		!element.isNull();
		element = element.nextSiblingElement("label"))
	{
		Label *label = new Label();
		if (!label->init(element, count, true, mWidth, mHeight))
			delete label;
		else {
			mLabels.append(label);
			++count;
		}
	}
	return;

}

void Shape::initPorts(const QDomElement &graphics)
{
	QDomElement portsElement = graphics.firstChildElement("ports");
	if (portsElement.isNull()) {
		return;
	}
	initPointPorts(portsElement);
	initLinePorts(portsElement);

	return;
}

void Shape::initPointPorts(const QDomElement &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("pointPort");
		!portElement.isNull();
		portElement = portElement.nextSiblingElement("pointPort"))
	{
		Port *pointPort = new PointPort();
		if (!pointPort->init(portElement, mWidth, mHeight)) {
			delete pointPort;
			return;
		}
		mPorts.append(pointPort);
	}
	return;
}

void Shape::initLinePorts(const QDomElement &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("linePort");
		!portElement.isNull();
		portElement = portElement.nextSiblingElement("linePort"))
	{
		Port *linePort = new LinePort();
		if (!linePort->init(portElement, mWidth, mHeight))
		{
			delete linePort;
			return;
		}
		mPorts.append(linePort);
	}
	return;
}

void Shape::changeDir(QDir &dir) const
{
	if (!dir.exists(generatedDir)) {
		dir.mkdir(generatedDir);
	}
	dir.cd(generatedDir);
	QString editorName = mNode->diagram()->editor()->name()	;
	if (!dir.exists(editorName)) {
		dir.mkdir(editorName);
	}
	dir.cd(editorName);
	if (!dir.exists(generatedShapesDir)) {
		dir.mkdir(generatedShapesDir);
	}
	dir.cd(generatedShapesDir);
	if (!dir.exists(shapesDir)) {
		dir.mkdir(shapesDir);
	}
	dir.cd(shapesDir);
}

void Shape::generate(QString &classTemplate) const
{
	if (!mNode)
		return;

	generateSdf();

	MetaCompiler *compiler = mNode->diagram()->editor()->metaCompiler();
	QString unused;
	if (!hasPointPorts()) {
		unused += nodeIndent + "Q_UNUSED(pointPorts)" + endline;
	}
	if (!hasLabels()) {
		unused += nodeIndent + "Q_UNUSED(titles);" + endline + nodeIndent + "Q_UNUSED(factory)" + endline;
	}

	QString shapeRendererLine = hasPicture()
								? compiler->getTemplateUtils(nodeLoadShapeRendererTag)
								: "";
	QString portRendererLine = (hasLinePorts() || hasPointPorts())
								? compiler->getTemplateUtils(nodeLoadPortsRendererTag)
								: nodeIndent +  "mRenderer->setElementRepo(elementRepo);";
	QString nodeContentsLine = compiler->getTemplateUtils(nodeContentsTag)
							.replace(nodeWidthTag, QString::number(mWidth))
							.replace(nodeHeightTag, QString::number(mHeight));
	QString portsInitLine;
	for (Port *port : mPorts) {
		port->generatePortList(this->mNode->diagram()->editor()->getAllPortNames());
		portsInitLine += port->generateInit(compiler) + endline;
	}

	QString labelsInitLine;
	QString labelsUpdateLine;
	QString labelsDefinitionLine;

	foreach(Label *label, mLabels) {
		labelsInitLine += label->generateInit(compiler, true) + endline;
		labelsUpdateLine += label->generateUpdate(compiler) + endline;
		labelsDefinitionLine += label->generateDefinition(compiler) + endline;
	}
	if (mLabels.isEmpty()) { // no labels
		labelsUpdateLine = nodeIndent + "Q_UNUSED(repo)" + endline;
	}

	classTemplate.replace(nodeUnusedTag, unused)
			.replace(nodeLoadShapeRendererTag, shapeRendererLine)
			.replace(nodeLoadPortsRendererTag, portRendererLine)
			.replace(nodeContentsTag, nodeContentsLine)
			.replace(nodeInitPortsTag, portsInitLine)
			.replace(nodeInitTag, labelsInitLine)
			.replace(updateDataTag, labelsUpdateLine)
			.replace(labelDefinitionTag, labelsDefinitionLine);
}

QList<Port*> Shape::getPorts() const
{
	return mPorts;
}

void Shape::generateSdf() const
{
	if (!hasPicture()) {
		return;
	}

	QDir dir;
	changeDir(dir);

	const QString fileName = dir.absoluteFilePath(mNode->name() + "Class.sdf");
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return;
	}

	QTextStream out(&file);
	out << mPicture;
	file.close();
}

bool Shape::hasLabels() const
{
	return !mLabels.isEmpty();
}

bool Shape::hasPointPorts() const
{
	foreach (Port *port, mPorts){
		if (dynamic_cast<PointPort*>(port)) {
			return true;
		}
	}
	return false;
}

bool Shape::hasLinePorts() const
{
	foreach (Port *port, mPorts){
		if (dynamic_cast<LinePort*>(port)) {
			return true;
		}
	}
	return false;
}

bool Shape::hasPicture() const
{
	return !mPicture.isEmpty();
}

QString Shape::generateResourceLine(const QString &resourceTemplate) const
{
	QString result;

	if (!hasPicture()) {
		return result;
	}

	QString line = resourceTemplate;
	result += line.replace(fileNameTag, mNode->name() + "Class.sdf") + endline;

	return result;
}
