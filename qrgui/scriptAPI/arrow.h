#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QWidget>


namespace qReal {
namespace gui {

class Arrow : public QWidget
{
	Q_OBJECT

public:
	Arrow(QPoint const &sourcePoint, QPoint const &destPoint, QWidget *parent);

protected:
	void paintEvent(QPaintEvent *);

private:
	QPoint mSourcePoint;
	QPoint mDestPoint;
};

}
}
