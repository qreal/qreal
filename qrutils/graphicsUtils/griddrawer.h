#pragma once

#include <QtGui/QPainter>
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT GridDrawer
{
public:
	void drawGrid(QPainter *painter, const QRectF &rect, const int indexGrid);
	GridDrawer();
};
}
