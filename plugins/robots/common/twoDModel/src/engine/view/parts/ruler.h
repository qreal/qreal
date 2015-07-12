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
