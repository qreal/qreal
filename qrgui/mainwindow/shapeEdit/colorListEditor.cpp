#include <QtWidgets>

#include "colorListEditor.h"

ColorListEditor::ColorListEditor(QWidget *widget) : QComboBox(widget)
{
	populateList();
}

QColor ColorListEditor::color() const
{
	return itemData(currentIndex(), Qt::DecorationRole).value<QColor>();
}

void ColorListEditor::setColor(QColor color)
{
	setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorListEditor::populateList()
{
	QStringList colorNames = QColor::colorNames();
	for (int i = 0; i < colorNames.size(); ++i) {
		QColor color(colorNames[i]);
		insertItem(i, colorNames[i]);
		setItemData(i, color, Qt::DecorationRole);
	}
}
