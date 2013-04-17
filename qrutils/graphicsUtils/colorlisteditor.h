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
        void setColorList(QStringList const &ColorList);
	QColor color() const;
	void setColor(QColor c);

private:
	QColor mColor;
        QStringList mColorList;
	void populateList();
};
}
