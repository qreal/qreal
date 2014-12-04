#pragma once

#include <QtWidgets/QGraphicsView>
#include <QtGui/QKeyEvent>

#include "../utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT AbstractView : public QGraphicsView
{
	Q_OBJECT
public:
	AbstractView(QWidget *parent = 0);

public slots:
	void zoomIn();
	void zoomOut();

signals:
	void deleteItem();

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
};
}
