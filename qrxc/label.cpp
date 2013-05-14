#include "label.h"
#include "../qrutils/outFile.h"

#include <QDebug>

using namespace utils;

bool Label::init(QDomElement const &element, int index, bool nodeLabel, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);

	mCenter = element.attribute("center", "false");
	mText = element.attribute("text");
	mTextBinded = element.attribute("textBinded");
	mReadOnly = element.attribute("readOnly", "false");
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
		// Это бинденный лейбл, текст для него будет браться из репозитория
		out() << "			" + titleName() + " = factory.createTitle("
				+ QString::number(mX.value()) + ", " + QString::number(mY.value()) + ", \"" + mTextBinded + "\", " + mReadOnly + ");\n";
	} else {
		// Это статический лейбл, репозиторий ему не нужен
		out() << "			" + titleName() + " = factory.createTitle("
				+ QString::number(mX.value()) + ", " + QString::number(mY.value()) + ", QString::fromUtf8(\"" + mText + "\"));\n";
	}
	out() << "			" + titleName() + "->setBackground(Qt::" + mBackground + ");\n";

	QString const scalingX = mX.isScalable() ? "true" : "false";
	QString const scalingY = mY.isScalable() ? "true" : "false";
	out() << "			" + titleName() + "->setScaling(" + scalingX + ", " + scalingY + ");\n";
	out() << "			" + titleName() + "->setHard(" + (mIsHard ? "true" : "false") + ");\n";

	// TODO: вынести отсюда в родительский класс.
	out() << "			" + titleName() + "->setFlags(0);\n"
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
		out() << "\t\t\t" + titleName() + "->setHtml(QString(\""
			+ (mCenter == "true" ? "<center>%1</center>" : "<b>%1</b>") + "\").arg(" + resultStr + ").replace(\"\\n\", \"<br>\"));\n";
	}
}

void Label::generateCodeForFields(OutFile &out)
{
	out() << "		ElementTitleInterface *" + titleName() + ";\n";
}


