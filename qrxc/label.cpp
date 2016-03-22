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
		qWarning() << "ERROR: can't parse label";
		return false;
	}
	return true;
}

Label *Label::clone()
{
	Label *returnLabel = new Label();
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

QString Label::labelName() const
{
	return "label_" + QString("%1").arg(mIndex);
}

void Label::changeIndex(int i)
{
	mIndex = i;
}

void Label::generateCodeForConstructor(OutFile &out) const
{
	if (mText.isEmpty()) {
		// It is binded label, text for it will be fetched from repo.
		out() << QString("\t\t\tqReal::LabelProperties %1(%2, %3, %4, \"%5\", %6, %7);\n").arg(labelName()
						, QString::number(mIndex)
						, QString::number(mX.value())
						, QString::number(mY.value())
						, mTextBinded, mReadOnly
						, QString::number(mRotation));
	} else {
		// It is a static label, text for it is fixed.
		out() << QString("\t\t\tqReal::LabelProperties %1(%2, %3, %4, QObject::tr(\"%5\"), %6);\n").arg(labelName()
				, QString::number(mIndex)
				, QString::number(mX.value())
				, QString::number(mY.value())
				, mText
				, QString::number(mRotation));
	}

	out() << QString("\t\t\t%1.setBackground(Qt::%2);\n").arg(labelName(), mBackground);

	const QString scalingX = mX.isScalable() ? "true" : "false";
	const QString scalingY = mY.isScalable() ? "true" : "false";
	out() << QString("\t\t\t%1.setScalingX(%2);\n").arg(labelName(), scalingX);
	out() << QString("\t\t\t%1.setScalingY(%2);\n").arg(labelName(), scalingY);
	out() << QString("\t\t\t%1.setHard(%2);\n").arg(labelName(), mIsHard ? "true" : "false");
	out() << QString("\t\t\t%1.setPlainTextMode(%2);\n").arg(labelName(), mIsPlainText ? "true" : "false");

	if (!mPrefix.isEmpty()) {
		out() << QString("\t\t\t%1.setPrefix(QObject::tr(\"%2\"));\n").arg(labelName(), mPrefix);
	}

	if (!mSuffix.isEmpty()) {
		out() << QString("\t\t\t%1.setSuffix(QObject::tr(\"%2\"));\n").arg(labelName(), mSuffix);
	}

	out() << QString("\t\t\taddLabel(%1);\n").arg(labelName());
}
