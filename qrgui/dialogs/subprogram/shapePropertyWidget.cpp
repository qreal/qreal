/* Copyright 2015-2016 Kogutich Denis
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

#include "shapePropertyWidget.h"

#include <QtGui/QPainter>

#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace gui;

const int minWidth = 355;

ShapePropertyWidget::ShapePropertyWidget(QWidget *parent)
	: QWidget(parent)
	, mSelectedShapeIndex(-1)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
}

void ShapePropertyWidget::initShapes(const QStringList &shapes, const QString &currentShape, bool background)
{
	qreal x = 0.0;
	int index = 0;
	bool findedCurrentShape = false;
	if (!background) {
		/// @todo: Robots images should not be used here.
		const QString defaultShape = "images/subprogramRobotsBlock.png";
		addShape(index, x, defaultShape, currentShape, findedCurrentShape);
	}

	for (const QString &shape : shapes) {
		addShape(index, x, shape, currentShape, findedCurrentShape);
	}

	const int width = (index * 75 < minWidth) ? minWidth : index * 75;
	setFixedSize(width, 75);
}

QString ShapePropertyWidget::selectedShape() const
{
	return mShapes.isEmpty() || mSelectedShapeIndex == -1 ? QString() : mShapes.at(mSelectedShapeIndex)->shape();
}

void ShapePropertyWidget::selectShape(int index)
{
	if (!mShapes.isEmpty() && mShapes.size() > index) {
		mShapes.at(index)->addSelection();
		mSelectedShapeIndex = index;
	}
}

void ShapePropertyWidget::paintEvent(QPaintEvent *)
{
	mWidthOfGrid = SettingsManager::value("GridWidth").toDouble() / 100;
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, mWidthOfGrid));

	const int indexGrid = SettingsManager::value("IndexGrid").toInt();
	mGridDrawer.drawGrid(&painter, rect(), indexGrid);
}

void ShapePropertyWidget::shapeClicked()
{
	const int newSelectedIndex = dynamic_cast<ShapeWidget *>(sender())->index();

	if (mSelectedShapeIndex != newSelectedIndex) {
		if (mSelectedShapeIndex != -1) {
			mShapes.at(mSelectedShapeIndex)->removeSelection();
		}

		mSelectedShapeIndex = newSelectedIndex;
	}
}

void ShapePropertyWidget::addShape(int &index, qreal &x, const QString &shape, const QString &currentShape
		, bool &foundCurrentShape)
{
	ShapeWidget *shapeWidget = new ShapeWidget(index, this);
	shapeWidget->setGeometry(static_cast<int>(x), 0, 0, 0);
	mShapes << shapeWidget;
	shapeWidget->setShape(shape);

	if (!foundCurrentShape && shape == currentShape) {
		shapeWidget->addSelection();
		mSelectedShapeIndex = index;
		foundCurrentShape = true;
	}

	shapeWidget->show();
	connect(shapeWidget, &ShapeWidget::clicked, this, &ShapePropertyWidget::shapeClicked);
	++index;
	x += 75.0;
}
