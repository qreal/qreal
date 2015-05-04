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

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpinBox>

#include <qrutils/graphicsUtils/colorListEditor.h>

#include "src/engine/items/wallItem.h"

using namespace twoDModel::view;

ColorItemPopup::ColorItemPopup(graphicsUtils::AbstractScene &scene, QWidget *parent)
	: ItemPopup(scene, parent)
{
	initWidget();
}

ColorItemPopup::~ColorItemPopup()
{
}

bool ColorItemPopup::suits(QGraphicsItem *item)
{
	/// @todo: Make wall not inheriting from ColorFieldItem.
	return dynamic_cast<items::ColorFieldItem *>(item) != nullptr
			&& dynamic_cast<items::WallItem *>(item) == nullptr;
}

void ColorItemPopup::attachTo(const QList<QGraphicsItem *> &items)
{
	ItemPopup::attachTo(items);
	mColorPicker->setColor(dominantPropertyValue("color").value<QColor>());
	mSpinBox->setValue(dominantPropertyValue("thickness").toInt());
}

void twoDModel::view::ColorItemPopup::initWidget()
{
	QVBoxLayout * const layout = new QVBoxLayout(this);
	layout->addWidget(initColorPicker());
	layout->addWidget(initSpinBox());

	updateDueToLayout();
}

QWidget *ColorItemPopup::initColorPicker()
{
	graphicsUtils::ColorListEditor * const editor = new graphicsUtils::ColorListEditor(this, true);
	const QStringList colorList = { "Black", "Blue", "Green", "Yellow", "Red" };
	editor->setColorList(colorList);
	editor->setFocusPolicy(Qt::NoFocus);
	connect(editor, &graphicsUtils::ColorListEditor::colorChanged, [=](const QColor &color) {
		setPropertyMassively("color", color);
	});

	mColorPicker = editor;
	return editor;
}

QWidget *ColorItemPopup::initSpinBox()
{
	QSpinBox * const spinBox = new QSpinBox(this);
	QPalette spinBoxPalette;
	spinBoxPalette.setColor(QPalette::Window, Qt::transparent);
	spinBoxPalette.setColor(QPalette::Base, Qt::transparent);
	spinBox->setPalette(spinBoxPalette);
	connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value) {
		setPropertyMassively("thickness", value);
	});

	mSpinBox = spinBox;
	return spinBox;
}
