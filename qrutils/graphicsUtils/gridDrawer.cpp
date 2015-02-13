#include <QtCore/QLineF>

#include "gridDrawer.h"

using namespace graphicsUtils;

GridDrawer::GridDrawer()
{
}

void GridDrawer::drawGrid(QPainter *painter, const QRectF &rect, const int indexGrid)
{
	const int left = static_cast<int>(rect.left());
	const int right = static_cast<int>(rect.right());
	const int top = static_cast<int>(rect.top());
	const int bottom = static_cast<int>(rect.bottom());

	const int startX = left / indexGrid * indexGrid;
	const int startY = top / indexGrid * indexGrid;

	for (int i = startX; i <= right; i += indexGrid) {
		painter->drawLine(i, top, i, bottom);
	}

	for (int i = startY; i <= bottom; i += indexGrid) {
		painter->drawLine(left, i, right, i);
	}
}
