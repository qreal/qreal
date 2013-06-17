#include <QtCore>
#include "gridDrawer.h"

using namespace graphicsUtils;

GridDrawer::GridDrawer()
{
}

void GridDrawer::drawGrid(QPainter *painter, QRectF const &rect, int const indexGrid)
{
	qreal const sceneX = rect.x();
	qreal const sceneY = rect.y();

	int const indent = (300 / indexGrid) * indexGrid; // now enough

	int const startX = static_cast<int>(sceneX + 10) / indexGrid * indexGrid - indent;
	int const endX = static_cast<int>(sceneX + rect.width() - 10) / indexGrid * indexGrid + indent;
	int const startY = static_cast<int>(sceneY + 10) / indexGrid * indexGrid - indent;
	int const endY = static_cast<int>(sceneY + rect.height() - 10) / indexGrid * indexGrid + indent;
	for (int i = startX; i <= endX; i = i + indexGrid) {
		QLineF const line(i, startY, i, endY);
		painter->drawLine(line);
	}
	for (int i = startY; i <= endY; i = i + indexGrid) {
		QLineF const line(startX, i, endX, i);
		painter->drawLine(line);
	}
}
