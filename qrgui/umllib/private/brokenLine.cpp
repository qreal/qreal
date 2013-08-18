#include "brokenLine.h"

namespace qReal {

BrokenLine::BrokenLine(EdgeElement *edge)
	: LineHandler(edge)
{
}

void BrokenLine::moveEdge(QPointF const &pos, bool needAlign)
{
	QPolygonF line = mEdge->line();

	if (mDragType == EdgeElement::noPort) {
		mDragType = addPoint(mDragStartPoint);
	}

	line = mEdge->line();
	line[mDragType] = needAlign ? alignedPoint(pos) : pos;
	mEdge->setLine(line);

	mEdge->update();
}

void BrokenLine::improveAppearance()
{
	mEdge->delCloseLinePoints();
	mEdge->deleteLoops();
}

QPointF BrokenLine::alignedPoint(QPointF const &point) const
{
	QPointF result = mEdge->mapToScene(point);
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();

	int const coefX = static_cast<int>(result.x()) / indexGrid;
	int const coefY = static_cast<int>(result.y()) / indexGrid;

	result = QPointF(SceneGridHandler::alignedCoordinate(result.x(), coefX, indexGrid)
			, SceneGridHandler::alignedCoordinate(result.y(), coefY, indexGrid));

	return mEdge->mapFromScene(result);
}
}
