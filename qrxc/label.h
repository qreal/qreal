#pragma once

#include <QDomElement>
#include <QStringList>
#include "scalableItem.h"

namespace utils {
	class OutFile;
}

class Label : public ScalableItem
{
public:
	bool init(QDomElement const &element, int index, bool nodeLabel, int width, int height);
	void generateCodeForConstructor(utils::OutFile &out);
	void generateCodeForUpdateData(utils::OutFile &out);
	void generateCodeForFields(utils::OutFile &out);
	// Default copy constructor ok.
private:
	QString titleName() const;
	QStringList getListOfStr(QString strToParse);
	QStringList getReformedList(QStringList list);

	ScalableCoordinate mX;
	ScalableCoordinate mY;
	int mIndex;
	QString mText;
	QString mTextBinded;
	QString mReadOnly;
	QString mBackground;
	QString mCenter;
};
