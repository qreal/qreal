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
	void setColorList(const QStringList &colorList, const QStringList &translatedColorList = QStringList());
	QColor color() const;
	QColor colorByIndex(int index) const;
	void setColor(const QColor &color);

private:
	void populateList();

	QColor mColor;
	QStringList mColorList;
	QStringList mTranslatedColorList;
};

}
