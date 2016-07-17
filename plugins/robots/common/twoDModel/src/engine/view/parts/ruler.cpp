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

#include "ruler.h"

#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QtMath>

#include <qrkernel/settingsManager.h>

using namespace twoDModel::view;

const qreal gap = 5;  // The gap between the ruler borders and text on it.
const int frequency = 150;  // The text on the ruler will not be met more often than once per this number of pixels.

Ruler::Ruler(QWidget *parent)
	: QFrame(parent)
	, mOrientation(Qt::Horizontal)
	, mPixelsInCm(1.0)
{
	mFont.setPixelSize(8);
}

Ruler::~Ruler()
{
}

Qt::Orientation Ruler::orientation() const
{
	return mOrientation;
}

void Ruler::setOrientation(Qt::Orientation orientation)
{
	mOrientation = orientation;

	const QString theLongestText = "-123.45";  // The longest text that ruler must place into itself.
	const QSizeF theLargestSize = textBoundingRect(theLongestText).size();
	if (orientation == Qt::Horizontal) {
		setFixedHeight(theLargestSize.height() + 2 * gap);
	} else {
		setFixedWidth(theLargestSize.width() + 2 * gap);
	}
}

void Ruler::setPixelsInCm(qreal pixelsInCm)
{
	mPixelsInCm = pixelsInCm;
}

void Ruler::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
	QPainter painter(this);
	painter.setFont(mFont);
	const int gridSize = qReal::SettingsManager::value("2dGridCellSize").toInt();
	const int shift = qMax(frequency / gridSize, 1);
	const QRectF sceneRect = mView->mapToScene(mView->viewport()->geometry()).boundingRect();
	const int firstCell = qCeil(relevantCoordinate(sceneRect.topLeft())) / gridSize;
	// This will let us always draw 0 near zero line for more persistent coordinates scrolling.
	// Without making first cell being multiple of shift the first marker will be always upon the first
	// line and that looks horrible when user scrolls the scene.
	const int realFirstCell = firstCell / shift * shift * gridSize;

	for (int coordinate = realFirstCell
			; coordinate < relevantCoordinate(sceneRect.bottomRight())
			; coordinate += shift * gridSize)
	{
		const QString text = QString::number(coordinate / mPixelsInCm);
		const QRectF boundingRect = textBoundingRect(text);
		const qreal relevantPosition = relevantCoordinate(mView->mapFromScene(makePoint(coordinate, 0)));
		const QPointF position = drawingPoint(relevantPosition, boundingRect.size());
		const QPointF alignment = makePoint(relevantDimension(boundingRect.size()) / 2, 0);

		painter.drawText(position - boundingRect.topLeft() - alignment, text);
	}
}

qreal Ruler::relevantCoordinate(const QPointF &point) const
{
	return orientation() == Qt::Horizontal ? point.x() : point.y();
}

qreal Ruler::relevantDimension(const QSizeF &size) const
{
	return orientation() == Qt::Horizontal ? size.width() : size.height();
}

qreal Ruler::irrelevantDimension(const QSizeF &size) const
{
	return orientation() == Qt::Horizontal ? size.height() : size.width();
}

QPointF Ruler::makePoint(qreal relevantCoordinate, qreal irrelevantCoordinate) const
{
	return orientation() == Qt::Horizontal
			? QPointF(relevantCoordinate, irrelevantCoordinate)
			: QPointF(irrelevantCoordinate, relevantCoordinate);
}

QPointF Ruler::drawingPoint(qreal relevantCoordinate, const QSizeF &textSize) const
{
	return makePoint(relevantCoordinate, (irrelevantDimension(size()) - irrelevantDimension(textSize)) / 2);
}

QRectF Ruler::textBoundingRect(const QString &text) const
{
	return QFontMetrics(mFont).boundingRect(text);
}

void Ruler::setScene(QGraphicsView *scene)
{
	mView = scene;
}
