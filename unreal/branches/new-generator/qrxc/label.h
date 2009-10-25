#pragma once

#include <QDomElement>

class OutFile;

class Label
{
public:
	bool init(QDomElement const &element, int width, int height);
	void generateCode(OutFile &out);

private:
	qreal mX;
	qreal mY;
	QString mText;
	QString mTextBinded;
};
