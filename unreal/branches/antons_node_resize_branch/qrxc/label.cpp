#include "label.h"
#include "../utils/outFile.h"

#include <QDebug>

using namespace utils;

bool Label::init(QDomElement const &element, int index, bool nodeLabel)
{
	mX = element.attribute("x", "0");
	mY = element.attribute("y", "0");
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
		out() << "			" + titleName() + " = new ElementTitle("
				+ mX + ", " + mY + ", \"" + mTextBinded + "\", " + mReadOnly + ");\n";
	} else {
		// Это статический лейбл, репозиторий ему не нужен
		out() << "			" + titleName() + " = new ElementTitle("
				+ mX + ", " + mY + ", \"" + mText + "\");\n";
	}
	out() << "			" + titleName() + "->setBackground(Qt::" + mBackground + ");\n";

	// TODO: вынести отсюда в родительский класс.
	out() << "			" + titleName() + "->setFlags(0);\n"
		<< "			" + titleName() + "->setTextInteractionFlags(Qt::NoTextInteraction);\n"
		<< "			titles.append(" + titleName() + ");\n";
}

void Label::generateCodeForUpdateData(OutFile &out)
{
	if (mTextBinded.isEmpty())
		return;  // Метка статическая.
	QString field;
	if (mTextBinded == "name")
		field = "dataIndex.data(Qt::DisplayRole).toString()";
	else
		field = "roleValueByName(\"" + mTextBinded + "\")";  // Кастомное свойство. Если есть желание забиндиться на ещё какое-нибудь из предефайненных, надо тут дописать.

	out() << "			" + titleName() + "->setHtml(QString(\"%1\").arg(" + field + "));\n";
}

void Label::generateCodeForFields(OutFile &out)
{
	out() << "		ElementTitle *" + titleName() + ";\n";
}
