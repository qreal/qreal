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

#include "pointPort.h"
#include "../utils/defs.h"
#include "../metaCompiler.h"

using namespace qrmc;

bool PointPort::init(const QDomElement &element, int width, int height)
{
	mX = static_cast<qreal>(element.attribute("x").toInt()) / width;
	mY = static_cast<qreal>(element.attribute("y").toInt()) / height;
	mWidth = width;
	mHeight = height;
	return true;
}

Port* PointPort::clone() const
{
	PointPort *result = new PointPort();
	result->mX = mX;
	result->mY = mY;
	return result;
}

QString PointPort::generateSdf(MetaCompiler *compiler) const
{
	QString result = compiler->getTemplateUtils(pointPortTag);
	result.replace(pointXTag, QString::number(mX * mWidth)).replace(pointYTag, QString::number(mY * mHeight));
	return result;
}

QString PointPort::generateInit(MetaCompiler *compiler) const
{
	QString result = compiler->getTemplateUtils(nodePointPortInitTag);
	result.replace(pointXTag, QString::number(mX)).replace(pointYTag, QString::number(mY));
	return result;

}

void PointPort::generatePortList(const QStringList &portTypes)
{
	if (!portTypes.contains(mType)) {
		mType = "NonTyped";
	}
}
