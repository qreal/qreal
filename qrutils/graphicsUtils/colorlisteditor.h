#pragma once
#include <QtWidgets/QComboBox>
#include "../utilsDeclSpec.h"

class QColor;
class QWidget;

namespace graphicsUtils
{
class QRUTILS_EXPORT ColorListEditor : public QComboBox
{
	Q_OBJECT
public:
	ColorListEditor(QWidget *widget = 0);

public:
	void setColorList(QStringList const &colorList, QStringList const &translatedColorList = QStringList());
	QColor color() const;
	QColor colorByIndex(int index) const;
	void setColor(QColor c);

private:
	QColor mColor;
	QStringList mColorList;
	QStringList mTranslatedColorList;
	void populateList();
};
}
