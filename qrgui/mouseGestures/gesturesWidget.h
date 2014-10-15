#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QString>

#include <qrkernel/ids.h>

namespace Ui {
	class GesturesWidget;
}

namespace qReal {
namespace gestures {

class GesturesWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GesturesWidget(QWidget *parent = 0);
	~GesturesWidget();

	void draw(QString const &paths);
	Id currentElement() const;
	void setElements(QList<QPair<QString, Id> > const &elements);

signals:
	void currentElementChanged();

private:
	Ui::GesturesWidget *mUi;
	int mCurrentPointNumber;
	static int coord(int previous, int next, int part);
};

}
}
