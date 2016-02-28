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

#include <qrkernel/settingsManager.h>
#include <QtGui/QPainter>

using namespace qReal;

const int minWidth = 355;

ShapePropertyWidget::ShapePropertyWidget(QWidget *parent)
	: QWidget(parent)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
}

void ShapePropertyWidget::initShapes(const QStringList &shapesList)
{
	int width = (shapesList.count() * 75 < minWidth) ? minWidth : shapesList.count() * 75;
	setFixedSize(width, 75);
	qreal startX = 0.0;
	int index = 0;
	for (const QString &shape: shapesList) {
		ShapeWidget *shapeWidget = new ShapeWidget(index, this);
		shapeWidget->setGeometry(startX, 0, 0, 0);
		mShapes << shapeWidget;
		shapeWidget->setShape(shape);
		shapeWidget->show();
		startX += 75.0;
		index++;

		connect(shapeWidget, &ShapeWidget::clicked, this, &ShapePropertyWidget::shapeClicked);
	}
}

void ShapePropertyWidget::paintEvent(QPaintEvent *)
{
	mWidthOfGrid = SettingsManager::value("GridWidth").toDouble() / 100;
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, mWidthOfGrid));

	const QRectF r = QRectF(rect());
	const int indexGrid = SettingsManager::value("IndexGrid").toInt();
	mGridDrawer.drawGrid(&painter, r, indexGrid);
}

void ShapePropertyWidget::shapeClicked()
{
	const int newSelectedIndex = dynamic_cast<ShapeWidget *>(sender())->getIndex();
	if (mSelectedShapeIndex >= mShapes.count()) {
		mSelectedShapeIndex = newSelectedIndex;
		return;
	}
	if (mSelectedShapeIndex != newSelectedIndex) {
		mShapes.at(mSelectedShapeIndex)->removeSelection();
		mSelectedShapeIndex = newSelectedIndex;
	}
}
