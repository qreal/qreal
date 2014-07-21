#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QIconEngine>

/// This class get the gesture in QString. Call method pixmap
/// returns the icon as a pixmap with the required size, mode, and state.
/// The default implementation creates a new pixmap and calls paint() to fill it.
class GesturePainter : public QIconEngine
{
public:
	GesturePainter(QString const &gesture, QColor backgroungColor, QColor gestColor, qreal height);
	GesturePainter(QString const &gesture);
	/// Metod "paint" draw gesture on QIcon.
	void paint(QPainter * painter, const QRect & rect, QIcon::Mode mode, QIcon::State state) override;
	QIconEngine *clone() const override;
private:
	QString mGesture;
	QColor mGestColor;
	QColor mBackgroundColor;
	qreal mHeight;
	static short const minBoarder = -1000;
	static short const mGestWidth = 2;
	/// It constant is difference painter's and pixmap's coordinates.
	qreal const mFrame = 10;
};
