#pragma once

#include <QtGui/QPainter>

#include "../utilsDeclSpec.h"

namespace graphicsUtils {

class QRUTILS_EXPORT GridDrawer
{
public:
	GridDrawer();
	void drawGrid(QPainter *painter, QRectF const &rect, int const indexGrid);
};

}
