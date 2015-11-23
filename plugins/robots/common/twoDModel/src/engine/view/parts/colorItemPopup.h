/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include <qrutils/graphicsUtils/itemPopup.h>

class QSpinBox;
class QCheckBox;

namespace qReal {
namespace ui {
class ColorListEditor;
}
}

namespace twoDModel {
namespace view {

/// A popup for configuring ColorFieldItem on 2D model scene.
/// Contains item`s color picker and spin box for setting item`s thickness.
class ColorItemPopup : public graphicsUtils::ItemPopup
{
	Q_OBJECT

public:
	ColorItemPopup(const QPen &pen, graphicsUtils::AbstractScene &scene, QWidget *parent = 0);
	~ColorItemPopup();

	/// Returns last color selected by user.
	QColor lastColor() const;

	/// Returns last pen thickness entered by user.
	int lastThickness() const;

signals:
	/// Emitted when pen configured by user changed.
	void userPenChanged(const QPen &pen);

	/// Emitted when brush configured by user changed.
	void isFilledChanged(bool isFilled);

private slots:
	void setBrushPickerColor(const QColor &color);

private:
	bool suits(QGraphicsItem *item) override;
	bool attachTo(const QList<QGraphicsItem *> &items) override;

	void initWidget();
	QWidget *initColorPicker();
	QWidget *initBrushPicker();
	QWidget *initSpinBox();

	QPen pen() const;

	qReal::ui::ColorListEditor *mColorPicker;  // Takes ownership
	QCheckBox *mBrushPicker;  // Takes ownership
	QSpinBox *mSpinBox;  // Takes ownership
	QColor mLastColor;
	bool mLastFilled;
	int mLastThickness;
};

}
}
