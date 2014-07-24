#pragma once

#include <QtXml/QDomElement>
#include <QtCore/QStringList>

#include <qrutils/scalableItem.h>
#include "graphicType.h"

namespace utils {
class OutFile;
}

/// Class representing text label on a shape.
class Label : public utils::ScalableItem
{
public:
	/// Initialize label with data from XML and some external data.
	/// @param element XML node with label description.
	/// @param index Index of this label in a list of labels of parent element.
	/// @param nodeLabel True, if this label is for node, false if it is for edge.
	/// @param width Width of parent node or edge.
	/// @param height Height of parent node or edge.
	bool init(QDomElement const &element, int index, bool nodeLabel, int width, int height);

	void generateCodeForConstructor(utils::OutFile &out);
	void generateCodeForUpdateData(utils::OutFile &out);
	void generateCodeForFields(utils::OutFile &out);
	// Default copy constructor ok.
	Label* clone();
	void changeIndex(int i);

private:
	QString titleName() const;

	/// Split the string into substrings wherever "##" occurs and return the list of those strings.
	QStringList getListOfStr(QString const &strToParse) const;

	utils::ScalableCoordinate mX;
	utils::ScalableCoordinate mY;

	int mIndex;
	QString mText;
	QString mTextBinded;
	QString mReadOnly;
	QString mBackground;
	QString mCenter;
	bool mIsHard;
	bool mIsPlainText;
	qreal mRotation;
};
