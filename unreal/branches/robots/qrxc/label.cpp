#include "label.h"
#include "../utils/outFile.h"

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
	mIndex = index;
	mBackground = element.attribute("background", nodeLabel ? "transparent" : "white");
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

	const QString scalingX = mX.isScalable() ? "true" : "false";
	const QString scalingY = mY.isScalable() ? "true" : "false";
	out() << "			" + titleName() + "->setScaling(" + scalingX + ", " + scalingY + ");\n";

	// TODO: вынести отсюда в родительский класс.
	out() << "			" + titleName() + "->setFlags(0);\n"
		<< "			" + titleName() + "->setTextInteractionFlags(Qt::NoTextInteraction);\n"
		<< "			titles.append(" + titleName() + ");\n";
}

void Label::generateCodeForUpdateData(OutFile &out)
{
	if (mTextBinded.isEmpty()){
		// Метка статическая.
		out() << "\t\t\tQ_UNUSED(repo);\n";
		return;
	}
	QString field;
	if (mTextBinded == "name")
		field = "repo->name()";
	else
		// Кастомное свойство. Если есть желание забиндиться на ещё какое-нибудь из предефайненных, надо тут дописать.
		field = "repo->logicalProperty(\"" + mTextBinded + "\")";
	out() << "\t\t\t" + titleName() + "->setHtml(QString(\""
		+ (mCenter == "true" ? "<center>%1</center>" : "%1") + "\").arg(" + field + "));\n";
}

void Label::generateCodeForFields(OutFile &out)
{
	out() << "		ElementTitleInterface *" + titleName() + ";\n";
}
