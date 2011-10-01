#include <QtGui>
#include "colorlisteditor.h"
using namespace graphicsUtils;

ColorListEditor::ColorListEditor(QWidget *widget) : QComboBox(widget)
{
}

void ColorListEditor::setColorList(QStringList const &colorList)
{
    mColorList = colorList;
    populateList();
}

QColor ColorListEditor::color() const
{
	return qVariantValue<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void ColorListEditor::setColor(QColor color)
{
	setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorListEditor::populateList()
{
        QStringList colorNames = mColorList;
	for (int i = 0; i < colorNames.size(); ++i) {
		QColor color(colorNames[i]);
		insertItem(i, colorNames[i]);
		setItemData(i, color, Qt::DecorationRole);
	}
}
