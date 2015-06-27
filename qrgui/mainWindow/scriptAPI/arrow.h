#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QWidget>

namespace qReal {
namespace gui {

/// An implementation of arrow on the screen for pointing some widgets in help mode. Has animated fade-away effect.
/// @see qReal::HintAPI.
class Arrow : public QWidget
{
	Q_OBJECT

public:
	Arrow(const QPoint &sourcePoint, const QPoint &destPoint, int lifeTime, QWidget *parent);

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
