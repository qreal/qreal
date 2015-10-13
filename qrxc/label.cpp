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

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

using namespace utils;

bool Label::init(const QDomElement &element, int index, bool nodeLabel, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);

	mCenter = element.attribute("center", "false");
	mText = element.attribute("text");
	mTextBinded = element.attribute("textBinded");
	mPrefix = element.attribute("prefix");
	mSuffix = element.attribute("suffix");
	mReadOnly = element.attribute("readOnly", "false");
	mRotation = element.attribute("rotation", "0").toDouble();

	if (mTextBinded.contains("##")) {
		mReadOnly = "true";
	}
	mIndex = index;
	mBackground = element.attribute("background", nodeLabel && mTextBinded.isEmpty() ? "transparent" : "white");
	mIsHard = element.attribute("hard", "false").toLower().trimmed() == "true";
	mIsPlainText = element.attribute("isPlainText", "false").toLower().trimmed() == "true";
	if ((mText.isEmpty() && mTextBinded.isEmpty()) || (mReadOnly != "true" && mReadOnly != "false")) {
		qDebug() << "ERROR: can't parse label";
		return false;
	}
	return true;
}

Label* Label::clone()
{
	Label* returnLabel = new Label();
	returnLabel->mX = mX;
	returnLabel->mY = mY;

	returnLabel->mCenter = mCenter;
	returnLabel->mText = mText;
	returnLabel->mTextBinded = mTextBinded;
	returnLabel->mReadOnly = mReadOnly;
	returnLabel->mRotation = mRotation;

	returnLabel->mIndex = mIndex;
	returnLabel->mBackground = mBackground;
	returnLabel->mIsHard = mIsHard;
	returnLabel->mIsPlainText = mIsPlainText;
	return returnLabel;
}

QString Label::titleName() const
{
	return "title_" + QString("%1").arg(mIndex);
}

void Label::changeIndex(int i)
{
	mIndex = i;
}

void Label::generateCodeForConstructor(OutFile &out)
{
	if (mText.isEmpty()) {
		// It is binded label, text for it will be fetched from repo.
		out() << "			" + titleName() + " = factory.createLabel(" + QString::number(mIndex) + ", "
				+ QString::number(mX.value()) + ", " + QString::number(mY.value())
				+ ", \"" + mTextBinded + "\", " + mReadOnly + ", " + QString::number(mRotation) + ");\n";
	} else {
		// It is a static label, text for it is fixed.
		out() << "			" + titleName() + " = factory.createLabel(" + QString::number(mIndex) + ", "
				+ QString::number(mX.value()) + ", " + QString::number(mY.value())
				+ ", QObject::tr(\"" + mText + "\"), " + QString::number(mRotation) + ");\n";
	}
	out() << "			" + titleName() + "->setBackground(Qt::" + mBackground + ");\n";

	const QString scalingX = mX.isScalable() ? "true" : "false";
	const QString scalingY = mY.isScalable() ? "true" : "false";
	out() << "			" + titleName() + "->setScaling(" + scalingX + ", " + scalingY + ");\n";
	out() << "			" + titleName() + "->setHard(" + (mIsHard ? "true" : "false") + ");\n";

	if (!mPrefix.isEmpty()) {
		out() << "			" + titleName() + "->setPrefix(QObject::tr(\"" + mPrefix + "\"));\n";
	}

	if (!mSuffix.isEmpty()) {
		out() << "			" + titleName() + "->setSuffix(QObject::tr(\"" + mSuffix + "\"));\n";
	}

	out()
		<< "			" + titleName() + "->setTextInteractionFlags(Qt::NoTextInteraction);\n"
		<< "			titles.append(" + titleName() + ");\n";
}

QStringList Label::getListOfStr(const QString &strToParse) const
{
	return strToParse.split("##");
}

void Label::generateCodeForUpdateData(OutFile &out)
{
	if (mTextBinded.isEmpty()) {
		// Static label
		out() << "\t\t\tQ_UNUSED(repo);\n";
		return;
	}

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
				field = "QObject::tr(\"" + listElement + "\")";
			}

			resultStr += " + " +  field;
			counter++;
		}
		resultStr = resultStr.mid(3);
	}
	if (mIsPlainText) {
		out() << QString("\t\t\t%1->setPlainText(%2);\n")
				.arg(titleName(), resultStr);
	} else {
		out() << "\t\t\t" + titleName() + "->setTextFromRepo("
			 + resultStr + ");\n";
	}
}

void Label::generateCodeForFields(OutFile &out)
{
	out() << "		qReal::LabelInterface *" + titleName() + ";\n";
}
