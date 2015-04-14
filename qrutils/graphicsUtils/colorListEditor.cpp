/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
