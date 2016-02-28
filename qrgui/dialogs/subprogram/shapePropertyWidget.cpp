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

const QRectF contents = QRectF(0, 0, 50, 50);

ShapePropertyWidget::ShapePropertyWidget(QWidget *parent)
	: QWidget(parent)
{
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
}

void ShapePropertyWidget::setShape(const QString &shape)
{
	mShape = shape;
	repaint();
}

void ShapePropertyWidget::drawShape(QPainter *painter, const QRectF &bounds)
{
	QDomDocument dynamicShape;
	dynamicShape.setContent(mShape);
	mRenderer.load(dynamicShape);
	mRenderer.render(painter, bounds);
}

void ShapePropertyWidget::paintEvent(QPaintEvent *event)
{
	mWidthOfGrid = SettingsManager::value("GridWidth").toDouble() / 100;
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, mWidthOfGrid));

	const QRectF r = QRectF(rect());
	const int indexGrid = SettingsManager::value("IndexGrid").toInt();
	mGridDrawer.drawGrid(&painter, r, indexGrid);


	drawShape(&painter, contents);
}

