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

#pragma once

#include <QtWidgets/QFrame>

class QGraphicsView;

namespace twoDModel {
namespace view {

/// A widget for displaying distances on grid in centimeters.
class Ruler : public QFrame
{
	Q_OBJECT

	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
	explicit Ruler(QWidget *parent = 0);
	~Ruler();

	/// Returns the orientation of this ruler.
	Qt::Orientation orientation() const;

public slots:
	/// Returns the orientation of this ruler.
	void setOrientation(Qt::Orientation orientation);

	/// Reconfigures ruller to calculate distances in other metrics. The distance between grid lines in pixels
	/// stays the same, but values in centimeters modified.
	void setPixelsInCm(qreal pixelsInCm);

	/// Configures this ruller to work with the given graphics view.
	void setScene(QGraphicsView *scene);

private:
	void paintEvent(QPaintEvent *event) override;

	qreal relevantCoordinate(const QPointF &point) const;
	qreal relevantDimension(const QSizeF &size) const;
	qreal irrelevantDimension(const QSizeF &size) const;
	QPointF makePoint(qreal relevantCoordinate, qreal irrelevantCoordinate) const;
	QPointF drawingPoint(qreal relevantCoordinate, const QSizeF &textSize) const;
	QRectF textBoundingRect(const QString &text) const;

	Qt::Orientation mOrientation;
	qreal mPixelsInCm;
	QGraphicsView *mView;  // Doesn`t take owership
	QFont mFont;
};

}
}
