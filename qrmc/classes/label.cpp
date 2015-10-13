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

#include "label.h"
#include "../metaCompiler.h"

#include <QDebug>

using namespace qrmc;

bool Label::init(const QDomElement &element, int index, bool nodeLabel, int width, int height)
{
	initCoordinate(mX, element.attribute("x", "0"), width);
	initCoordinate(mY, element.attribute("y", "0"), height);

	scalingX = mX.isScalable() ? "true" : "false";
	scalingY = mY.isScalable() ? "true" : "false";

	mCenter = element.attribute("center", "false");
	mText = element.attribute("text");
	mTextBinded = element.attribute("textBinded");
	mReadOnly = element.attribute("readOnly", "false");
	mRotation = element.attribute("rotation", "0").toDouble();
	mIndex = index;
	mBackground = element.attribute("background", nodeLabel ? "transparent" : "white");
	if ((mText.isEmpty() && mTextBinded.isEmpty()) || (mReadOnly != "true" && mReadOnly != "false")) {
		qDebug() << "ERROR: can't parse label";
		return false;
	}
	return true;
}

void Label::initCoordinate(ScalableCoordinate &field, QString coordinate, int maxValue)
{
	if (coordinate.endsWith("a")) {
		coordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / maxValue, maxValue, true);
	} else if (coordinate.endsWith("%")) {
		coordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / 100, 100, false);
	} else {
		field = ScalableCoordinate(((qreal) coordinate.toInt()) / maxValue, maxValue, false);
	}
}

QString Label::generateInit(MetaCompiler *compiler, bool isNode) const
{
	QString result = isNode ? compiler->getTemplateUtils(nodeInitTag) : compiler->getTemplateUtils(edgeInitTag);
	QString name = mText.isEmpty() ? mTextBinded : mText;

	result.replace(labelXTag, mX.toString(false))
			.replace(labelYTag, mY.toString(false))
			.replace(xCoordIsScalable, scalingX)
			.replace(yCoordIsScalable, scalingY)
			.replace(labelReadonlyTag, mReadOnly)
			.replace(labelIndexTag, QString::number(mIndex))
			.replace(labelNameTag, "\"" + name + "\"")
			.replace(labelRotationTag, QString::number(mRotation));

	return result;
}

QString Label::generateUpdate(MetaCompiler *compiler) const
{
	if (mTextBinded.isEmpty()) // static label
		return (nodeIndent + "Q_UNUSED(repo)" + endline);

	QString result = compiler->getTemplateUtils(updateDataTag);
	QString name = generateCodeForUpdateData();

	return result.replace(updateRoleTag, name)
			.replace(labelIndexTag, QString::number(mIndex));
}

QStringList Label::getListOfStr(const QString &strToParse) const
{
	return strToParse.split("##");
}

QString Label::generateCodeForUpdateData() const
{
	QStringList list = getListOfStr(mTextBinded);

	QString resultStr;
	if (list.count() == 1) {
		if (list.first() == "name") {
			resultStr = "repo->name()";
		} else {
			resultStr = "repo->logicalProperty(\"" + list.first() + "\")";
		}
	} else {
		int counter = 1;
		foreach (const QString &listElement, list) {
			QString field;
			if (counter % 2 == 0) {
				if (listElement == "name") {
					field = "repo->name()";
				} else {
					field = "repo->logicalProperty(\"" + listElement + "\")";
				}
			} else {
				field = "QString::fromUtf8(\"" + listElement + "\")";
			}

			resultStr += " + " +  field;
			counter++;
		}
		resultStr = resultStr.mid(3);
	}
	return resultStr;
}

QString Label::generateDefinition(MetaCompiler *compiler) const
{
	QString result = compiler->getTemplateUtils(labelDefinitionTag);
	return result.replace(labelIndexTag, QString::number(mIndex));
}
