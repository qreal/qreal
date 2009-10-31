#include "label.h"
#include "outFile.h"

#include <qDebug>

bool Label::init(QDomElement const &element, int width, int height)
{
	mX = ((qreal) element.attribute("x").toInt()) / width;
	mY = ((qreal) element.attribute("y").toInt()) / height;
	mText = element.attribute("text");
	mTextBinded = element.attribute("textBinded");
	if ((mText == "") && (mTextBinded == ""))
	{
		qDebug() << "Error: can't parse label";
		return false;
	}
	return true;
}

void Label::generateCode(OutFile &out)
{
	out() << QString("\t\t\tQString text = QString(\"%1\")").arg(mTextBinded);
}