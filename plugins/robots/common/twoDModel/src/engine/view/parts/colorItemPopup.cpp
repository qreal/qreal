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

#include "colorItemPopup.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>

#include <qrutils/widgets/colorListEditor.h>

#include "src/engine/items/colorFieldItem.h"

using namespace twoDModel::view;

ColorItemPopup::ColorItemPopup(const QPen &pen, graphicsUtils::AbstractScene &scene, QWidget *parent)
	: ItemPopup(scene, parent)
	, mLastColor(pen.color())
	, mLastThickness(pen.width())
{
	initWidget();
}

ColorItemPopup::~ColorItemPopup()
{
}

QColor ColorItemPopup::lastColor() const
{
	return mLastColor;
}

int ColorItemPopup::lastThickness() const
{
	return mLastThickness;
}

bool ColorItemPopup::suits(QGraphicsItem *item)
{
	/// @todo: Make wall not inheriting from ColorFieldItem.
	return dynamic_cast<items::ColorFieldItem *>(item) != nullptr;
}

bool ColorItemPopup::attachTo(const QList<QGraphicsItem *> &items)
{
	ItemPopup::attachTo(items);

	// Subsequent setting values to editors will cause theese values loss. Saving it here.
	const QColor lastColorBackup = mLastColor;
	const int lastThicknessBackup = mLastThickness;

	blockSignals(true);
	mSpinBox->blockSignals(true);

	mColorPicker->setColor(dominantPropertyValue("color").value<QColor>());
	mBrushPicker->setVisible(hasProperty("filled"));
	mBrushPicker->setChecked(dominantPropertyValue("filled").toBool());
	mSpinBox->setValue(dominantPropertyValue("thickness").toInt());

	// Restoring values that really were picked by user.
	mLastColor = lastColorBackup;
	mLastThickness = lastThicknessBackup;

	blockSignals(false);
	mSpinBox->blockSignals(false);

	return true;
}

void twoDModel::view::ColorItemPopup::initWidget()
{
	QVBoxLayout * const layout = new QVBoxLayout(this);
	QHBoxLayout * const firstRow = new QHBoxLayout;
	firstRow->addWidget(initColorPicker());
	firstRow->addWidget(initBrushPicker());
	layout->addLayout(firstRow);
	layout->addWidget(initSpinBox());

	updateDueToLayout();
}

QWidget *ColorItemPopup::initColorPicker()
{
	qReal::ui::ColorListEditor * const editor = new qReal::ui::ColorListEditor(this, true);
	editor->setToolTip(tr("Color"));
	const QStringList colorList = { "Black", "Blue", "Green", "Yellow", "White", "Red" };
	editor->setColorList(colorList);
	editor->setFocusPolicy(Qt::NoFocus);
	connect(editor, &qReal::ui::ColorListEditor::colorChanged, [=](const QColor &color) {
		setPropertyMassively("color", color);
		if (mLastColor != color) {
			mLastColor = color;
			emit userPenChanged(pen());
		}
	});

	mColorPicker = editor;
	return editor;
}

QWidget *ColorItemPopup::initBrushPicker()
{
	QCheckBox * const editor = new QCheckBox(this);
	editor->setFocusPolicy(Qt::NoFocus);
	mBrushPicker = editor;
	connect(mColorPicker, &qReal::ui::ColorListEditor::colorChanged, this, &ColorItemPopup::setBrushPickerColor);
	setBrushPickerColor(mColorPicker->color());

	connect(editor, &QCheckBox::toggled, [this, editor](bool checked) {
		editor->setToolTip(checked ? tr("Disable filling") : tr("Enable filling"));
		setPropertyMassively("filled", checked);
		if (mLastFilled != checked) {
			mLastFilled = checked;
			emit isFilledChanged(mLastFilled);
		}
	});

	return editor;
}

QWidget *ColorItemPopup::initSpinBox()
{
	QSpinBox * const spinBox = new QSpinBox(this);
	spinBox->setRange(1, 30);
	spinBox->setToolTip(tr("Thickness"));
	QPalette spinBoxPalette;
	spinBoxPalette.setColor(QPalette::Window, Qt::transparent);
	spinBoxPalette.setColor(QPalette::Base, Qt::transparent);
	spinBox->setPalette(spinBoxPalette);
	connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) {
		setPropertyMassively("thickness", value);
		if (mLastThickness != value) {
			mLastThickness = value;
			emit userPenChanged(pen());
		}
	});

	mSpinBox = spinBox;
	return spinBox;
}

void ColorItemPopup::setBrushPickerColor(const QColor &color)
{
	mBrushPicker->setStyleSheet(QString(
			"QCheckBox { spacing: 0 }"
			"QCheckBox::indicator { width: 12px; height: 12px; }"
			"QCheckBox::indicator::checked { background: %1; border: 1px solid %1; border-radius: 6px; }"
			"QCheckBox::indicator::unchecked { background: white; border: 1px solid %1; border-radius: 6px; }"
	).arg(color.name()));
}

QPen ColorItemPopup::pen() const
{
	QPen pen(mLastColor);
	pen.setWidth(mLastThickness);
	return pen;
}
