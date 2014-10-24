#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QWidget>

namespace qReal {

namespace gui {

/// Represents graphical arrow. Allows draw navigated arrows with opacity animation (for a lifeTime).
/// Class HintAPI provides interface to draw those arrows, directed on any object in GUI of a system.
class Arrow : public QWidget
{
	Q_OBJECT

public:
	Arrow(QPoint const &sourcePoint, QPoint const &destPoint, int const lifeTime, QWidget *parent);

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void disappear();

private:
	QPoint mSourcePoint;
	QPoint mDestPoint;

	int mDuration;
};

}
}
