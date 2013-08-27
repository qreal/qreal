#include "label.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

using namespace utils;

bool Label::init(QDomElement const &element, int index, bool nodeLabel, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);

	mCenter = element.attribute("center", "false");
	mText = element.attribute("text");
	mTextBinded = element.attribute("textBinded");
	mReadOnly = element.attribute("readOnly", "false");
	mRotation = element.attribute("rotation", "0").toDouble();

	if (mTextBinded.contains("##")) {
		mReadOnly = "true";
	}
	mIndex = index;
	mBackground = element.attribute("background", nodeLabel ? "transparent" : "white");
	mIsHard = element.attribute("hard", "false").toLower().trimmed() == "true";
	mIsPlainText = element.attribute("isPlainText", "false").toLower().trimmed() == "true";
	if ((mText.isEmpty() && mTextBinded.isEmpty()) || (mReadOnly != "true" && mReadOnly != "false")) {
		qDebug() << "ERROR: can't parse label";
		return false;
	}
	return true;
}

QString Label::titleName() const
{
	return "title_" + QString("%1").arg(mIndex);
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
				+ ", QString::fromUtf8(\"" + mText + "\"), " + QString::number(mRotation) + ");\n";
	}
	out() << "			" + titleName() + "->setBackground(Qt::" + mBackground + ");\n";

	QString const scalingX = mX.isScalable() ? "true" : "false";
	QString const scalingY = mY.isScalable() ? "true" : "false";
	out() << "			" + titleName() + "->setScaling(" + scalingX + ", " + scalingY + ");\n";
	out() << "			" + titleName() + "->setHard(" + (mIsHard ? "true" : "false") + ");\n";

	out()
		<< "			" + titleName() + "->setTextInteractionFlags(Qt::NoTextInteraction);\n"
		<< "			titles.append(" + titleName() + ");\n";
}

QStringList Label::getListOfStr(QString const &strToParse) const
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
		foreach (QString const &listElement, list) {
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


