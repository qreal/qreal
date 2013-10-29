#pragma once

#include <QtWidgets/QComboBox>
#include <QtGui/QColor>

#include <qrutils/utilsDeclSpec.h>

namespace graphicsUtils {

class QRUTILS_EXPORT ColorListEditor : public QComboBox
{
	Q_OBJECT

public:
	explicit ColorListEditor(QWidget *widget = 0);

public:
	void setColorList(QStringList const &colorList, QStringList const &translatedColorList = QStringList());
	QColor color() const;
	QColor colorByIndex(int index) const;
	void setColor(QColor const &color);

private:
	void populateList();

	QColor mColor;
	QStringList mColorList;
	QStringList mTranslatedColorList;
};

}
