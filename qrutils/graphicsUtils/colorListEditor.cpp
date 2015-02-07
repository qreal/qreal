#include "colorListEditor.h"

using namespace graphicsUtils;

ColorListEditor::ColorListEditor(QWidget *widget)
	: QComboBox(widget)
{
}

void ColorListEditor::setColorList(const QStringList &colorList, const QStringList &translatedColorList)
{
	mColorList = colorList;
	mTranslatedColorList = (translatedColorList == QStringList()) ? colorList : translatedColorList;
	populateList();
}

QColor ColorListEditor::color() const
{
	return itemData(currentIndex(), Qt::DecorationRole).value<QColor>();
}

QColor ColorListEditor::colorByIndex(int index) const
{
	return itemData(index, Qt::DecorationRole).value<QColor>();
}

void ColorListEditor::setColor(const QColor &color)
{
	setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorListEditor::populateList()
{
	QStringList colorNames = mColorList;
	for (int i = 0; i < colorNames.size(); ++i) {
		const QColor color(colorNames[i]);
		insertItem(i, mTranslatedColorList[i]);
		setItemData(i, color, Qt::DecorationRole);
	}
}
