/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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
#include "qrmc/metaCompiler.h"

#include <QtCore/QDebug>

using namespace qrmc;

bool Label::init(const QDomElement &element, int index, bool nodeLabel, int width, int height)
{
	initCoordinate(mX, element.attribute("x", "0"), width);
	initCoordinate(mY, element.attribute("y", "0"), height);

	mScalingX = mX.isScalable() ? "true" : "false";
	mScalingY = mY.isScalable() ? "true" : "false";

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

void Label::initCoordinate(ScalableCoordinate &field, const QString &coordinate, int maxValue)
{
	QString actualCoordinate = coordinate;
	if (actualCoordinate.endsWith("a")) {
		actualCoordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(static_cast<qreal>(actualCoordinate.toInt()) / maxValue, maxValue, true);
	} else if (actualCoordinate.endsWith("%")) {
		actualCoordinate.remove(coordinate.length() - 1, 1);
		field = ScalableCoordinate(static_cast<qreal>(actualCoordinate.toInt()) / 100, 100, false);
	} else {
		field = ScalableCoordinate(static_cast<qreal>(actualCoordinate.toInt()) / maxValue, maxValue, false);
	}
}

QString Label::generateInit(const MetaCompiler &compiler, bool isNode) const
{
	QString result = isNode ? compiler.getTemplateUtils(nodeInitTag) : compiler.getTemplateUtils(edgeInitTag);
	const QString name = mText.isEmpty() ? mTextBinded : mText;

	result.replace(labelXTag, mX.toString(false))
			.replace(labelYTag, mY.toString(false))
			.replace(xCoordIsScalable, mScalingX)
			.replace(yCoordIsScalable, mScalingY)
			.replace(labelReadonlyTag, mReadOnly)
			.replace(labelIndexTag, QString::number(mIndex))
			.replace(labelNameTag, "\"" + name + "\"")
			.replace(labelRotationTag, QString::number(mRotation));

	return result;
}

QString Label::generateUpdate(const MetaCompiler &compiler) const
{
	if (mTextBinded.isEmpty()) {
		// static label
		return (nodeIndent + "Q_UNUSED(repo)" + endline);
	}

	QString result = compiler.getTemplateUtils(updateDataTag);
	const QString name = generateCodeForUpdateData();

	return result.replace(updateRoleTag, name)
			.replace(labelIndexTag, QString::number(mIndex));
}

QStringList Label::getListOfStr(const QString &strToParse) const
{
	return strToParse.split("##");
}

QString Label::generateCodeForUpdateData() const
{
	const QStringList list = getListOfStr(mTextBinded);

	QString resultStr;
	if (list.count() == 1) {
		if (list.first() == "name") {
			resultStr = "repo->name()";
		} else {
			resultStr = "repo->logicalProperty(\"" + list.first() + "\")";
		}
	} else {
		int counter = 1;
		for (const QString &listElement : list) {
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

QString Label::generateDefinition(const MetaCompiler &compiler) const
{
	QString result = compiler.getTemplateUtils(labelDefinitionTag);
	return result.replace(labelIndexTag, QString::number(mIndex));
}
