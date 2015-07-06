/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#pragma once

#include <QtWidgets/QComboBox>
#include <QtGui/QColor>

#include <qrutils/utilsDeclSpec.h>

namespace qReal {
namespace ui {

/// Provides a minimalistic color picker providing switching between a fixed set of colors
/// (in contrast to complicated QColorDialog able to pick any one).
class QRUTILS_EXPORT ColorListEditor : public QComboBox
{
	Q_OBJECT

public:
	/// @param minimalistic If true then only colors will be shown; otherwise color will be written by text.
	explicit ColorListEditor(QWidget *widget = 0, bool minimalistic = false);

signals:
	/// Emitted when user picked another color.
	void colorChanged(const QColor &color);

public:
	/// Sets colors user can switch between. In compact mode @arg translatedColorList will be ignored.
	void setColorList(const QStringList &colorList, const QStringList &translatedColorList = QStringList());

	/// Returns the currently selected by user color.
	QColor color() const;

	/// Returns the color value in the given combobox cell.
	QColor colorByIndex(int index) const;

	/// Sets the color that will be shown as selected.
	void setColor(const QColor &color);

private:
	void populateList();

	bool mMinimalistic;
	QStringList mColorList;
	QStringList mTranslatedColorList;
};

}
}
