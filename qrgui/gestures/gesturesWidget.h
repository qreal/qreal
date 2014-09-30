#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QString>

#include "gesturesPainterInterface.h"

namespace Ui {
	class GesturesWidget;
}

namespace qReal {
namespace gestures {

class GesturesWidget : public QWidget, public GesturesPainterInterface
{
	Q_OBJECT

public:
	explicit GesturesWidget(QWidget *parent = 0);
	~GesturesWidget();

	void draw(QString const &paths) override;
	Id currentElement() const override;
	void setElements(QList<QPair<QString, Id> > const &elements) override;
	void paintEvent(QPaintEvent *) override;

signals:
	void currentElementChanged();

private:
	Ui::GesturesWidget *mUi;
	int mCurrentPointNumber;
	QString mIconPath;
	static int coord(int previous, int next, int part);
};

}
}
