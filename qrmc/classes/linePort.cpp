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

#include "linePort.h"
#include "../utils/defs.h"
#include "../metaCompiler.h"

#include <QtCore/QDebug>

using namespace qrmc;

bool LinePort::init(const QDomElement &element, int width, int height)
{
	mWidth = width;
	mHeight = height;
	mType = element.attribute("type", "NonTyped");
	QDomElement portStartElement = element.firstChildElement("start");
	QDomElement portEndElement = element.firstChildElement("end");

	initCoordinate(mStartX, portStartElement.attribute("startx"), width);
	initCoordinate(mStartY, portStartElement.attribute("starty"), height);
	initCoordinate(mEndX, portEndElement.attribute("endx"), width);
	initCoordinate(mEndY, portEndElement.attribute("endy"), height);
	return true;
}

void LinePort::initCoordinate(ScalableCoordinate &field, QString coordinate, int maxValue)
{
	if (coordinate.endsWith("a"))
	{
		coordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / maxValue, maxValue, true);
	}
	else if (coordinate.endsWith("%"))
	{
		coordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / 100, 100, false);
	}
	else
	{
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / maxValue, maxValue, false);
	}
}

Port *LinePort::clone() const
{
	LinePort *result = new LinePort();
	result->mEndX = mEndX;
	result->mEndY = mEndY;
	result->mStartX = mStartX;
	result->mStartY = mStartY;
	return result;
}

QString LinePort::generate(const QString &lineTemplate, bool isScaled) const
{
	QString result = lineTemplate;
	result.replace(startXTag, mStartX.toString(isScaled)).replace(startYTag, mStartY.toString(isScaled))
		.replace(endXTag, mEndX.toString(isScaled)).replace(endYTag, mEndY.toString(isScaled));
	return result;
}

QString LinePort::generateSdf(MetaCompiler *compiler) const
{
	QString linePortLine = compiler->getTemplateUtils(linePortTag);
	return generate(linePortLine, true);
}

QString LinePort::generateInit(MetaCompiler *compiler) const
{
	QString linePortLine = compiler->getTemplateUtils(nodeLinePortInitTag);
	QString result = generate(linePortLine, false);

	result.replace(startXScalabilityTag, mStartX.getScalability())
			.replace(startYScalabilityTag, mStartY.getScalability())
			.replace(endXScalabilityTag, mEndX.getScalability())
			.replace(endYScalabilityTag, mEndY.getScalability());

	return result;
}

void LinePort::generatePortList(const QStringList &portTypes)
{
	if (!portTypes.contains(mType)) {
		mType = "NonTyped";
	}
}
