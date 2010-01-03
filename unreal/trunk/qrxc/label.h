#pragma once

#include <QDomElement>

namespace utils {
	class OutFile;
}

class Label
{
public:
	bool init(QDomElement const &element, int index);
	void generateCodeForConstructor(utils::OutFile &out);
	void generateCodeForUpdateData(utils::OutFile &out);
	void generateCodeForFields(utils::OutFile &out);
private:
	QString titleName() const;

	QString mX;
	QString mY;
	int mIndex;
	QString mText;
	QString mTextBinded;
	QString mReadOnly;
};
