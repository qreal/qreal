#pragma once

#include <QDomElement>

namespace utils {
	class OutFile;
}

class Label
{
public:
	bool init(QDomElement const &element, int width, int height);
	void generateCode(utils::OutFile &out);

private:
	qreal mX;
	qreal mY;
	QString mText;
	QString mTextBinded;
};
