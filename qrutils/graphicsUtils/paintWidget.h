#pragma once

#include <QtWidgets/QWidget>

#include "painterInterface.h"
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{
class QRUTILS_EXPORT PaintWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PaintWidget(QWidget *parent = 0);
	void setPainter(PainterInterface *painter);
	
protected:
	void paintEvent(QPaintEvent *);

private:
	QList<PainterInterface*> mPainters;
	
};
}
