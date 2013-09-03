#include <QtCore/QLineF>

#include "gridDrawer.h"

using namespace graphicsUtils;

GridDrawer::GridDrawer()
{
}

void GridDrawer::drawGrid(QPainter *painter, QRectF const &rect, int const indexGrid)
{
	int const left = static_cast<int>(rect.left());
	int const right = static_cast<int>(rect.right());
	int const top = static_cast<int>(rect.top());
	int const bottom = static_cast<int>(rect.bottom());

	int const startX = left / indexGrid * indexGrid;
	int const startY = top / indexGrid * indexGrid;

	for (int i = startX; i <= right; i += indexGrid) {
		painter->drawLine(i, top, i, bottom);
	}

	for (int i = startY; i <= bottom; i += indexGrid) {
		painter->drawLine(left, i, right, i);
	}
}
