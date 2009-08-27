#include "pathCreator.h"

#include <QGraphicsScene>
#include <QQueue>
#include <math.h>

#include "uml_nodeelement.h"

static Direction convert(int const &i)
{
	switch(i)
	{
	case 0: return d_up;
	case 1: return d_right;
	case 2: return d_down;
	case 3: return d_left;
	default: return d_up;
	}
}

// -----------------------------------------------------------------------------

PathCreator::PathCreator(UML::EdgeElement* elem, QPointF const &stP, QPointF const &endP)
{
	step = defaultStep;
	edge = elem;
	startPoint = stP;
	endPoint = endP;
}

void PathCreator::load()
{
	const double deltaStep = 1/1000;
	road.clear();
	graph_x.clear();
	graph_y.clear();
	dPaths.clear();
	QPainterPath p;
	bounds = p;
	QList<float> gx, gy;

	foreach (QGraphicsItem* item, edge->scene()->items()) {
		UML::NodeElement* elem = dynamic_cast<UML::NodeElement*>(item);

		if (elem == NULL) {
			continue;
		}

		QRectF rect= elem->boundingRect();

		rect = elem->mapRectToItem(edge, rect);

		if (rect.contains(startPoint) || rect.contains(endPoint))
			continue;

		rect.adjust(-step + deltaStep, -step + deltaStep, step - deltaStep, step - deltaStep);
		bounds.addRect(rect);
		rect.adjust(-deltaStep, -deltaStep, deltaStep, deltaStep);
		gx << rect.left() << rect.center().x() << rect.right();
		gy << rect.bottom() << rect.center().y() << rect.top();
	}

	gx << startPoint.x() << endPoint.x();
	gy << startPoint.y() << endPoint.y();

	graph_x = gx;
	graph_y = gy;

	qSort(graph_x.begin(), graph_x.end());
	qSort(graph_y.begin(), graph_y.end());

	bounds.setFillRule(Qt::WindingFill);
}

bool PathCreator::deleteCorners()
{
	if (road.size() == 3)
		return false;

	QPointF point;
	bool vert = true;

	for (int i = 1; i < road.size() - 1; ++i) {
		if (road.at(i - 1).x() == road.at(i).x()) {
			point.setX(road.at(i + 1).x());
			point.setY(road.at(i - 1).y());
			vert = false;
		}
		else {
			point.setX(road.at(i - 1).x());
			point.setY(road.at(i + 1).y());
			vert = true;
		}

		if(canChangeCorner(road.at(i - 1), point, road.at(i + 1),vert)) {
			road.replace(i,point);
			return true;
		}
	}
	return false;
}

bool PathCreator::canChangeCorner(QPointF const &a, QPointF const &b, QPointF const &c, bool vert) const
{
	int ax = graph_x.indexOf(a.x());
	int ay = graph_y.indexOf(a.y());
	int bx = graph_x.indexOf(b.x());
	int by = graph_y.indexOf(b.y());
	int cx = graph_x.indexOf(c.x());
	int cy = graph_y.indexOf(c.y());
	int stV = 0, endV = 0, stH = 0, endH = 0;
	if(vert) {
		stV = (ay>by) ? by : ay;
		endV = (ay>by) ? ay : by;
		stH = (bx>cx) ? cx : bx;
		endH = (bx>cx) ? bx : cx;
	}
	else {
		stV = (by > cy) ? cy : by;
		endV = (by > cy) ? by : cy;
		stH = (ax > bx) ? bx : ax;
		endH = (ax > bx) ? ax : bx;
	}

	for (int i = stH; i <= endH; ++i)
		if (bounds.contains(QPointF(graph_x.at(i), graph_y.at(by))))
			return false;

	for (int i = stV; i <= endV; ++i)
		if (bounds.contains(QPointF(graph_x.at(bx), graph_y.at(i))))
			return false;

	return true;
}

bool PathCreator::deletePoints()
{
	for (int i = 1; i < road.size() - 1; ++i) {
		if ((road.at(i - 1).x() == road.at(i + 1).x())
			|| (road.at(i - 1).y() == road.at(i + 1).y()))
		{
			road.removeAt(i);
			return true;
		}
	}
	return false;
}

bool PathCreator::findWay()
{
	QQueue<GrNode> M1;
	int maxX = graph_x.size();
	int maxY = graph_y.size();

	// TODO: Тогда уж std::vector или QList. В нашем веке массивы через указатели
	// задавать как-то не очень.
	int **graph = new int*[maxX];
	for (int i = 0; i < maxX; ++i)
		graph[i] = new int[maxY];

	for (int i = 0; i < maxX; ++i)
		for (int j = 0; j < maxY; ++j)
			graph[i][j] = 2;

	int x = graph_x.indexOf(startPoint.x());
	int y = graph_y.indexOf(startPoint.y());
	M1.enqueue(GrNode(this, x, y, 0));

	x = graph_x.indexOf(endPoint.x());
	y = graph_y.indexOf(endPoint.y());

	GrNode endNode(QPoint(x, y));

	while (!M1.isEmpty()) {
		GrNode gN = M1.dequeue();
		graph[gN.getPos().x()][gN.getPos().y()] = 0;
		for (int j = 0; j < 4; ++j) {
			GrNode gNode = gN.nextPoint(convert(j));
			if (gNode.getLength() != -1) {
				if(graph[gNode.getPos().x()][gNode.getPos().y()] == 2) {
					graph[gNode.getPos().x()][gNode.getPos().y()] = 1;
					gNode.setParent(gN.getPos());
					M1.enqueue(gNode);
				}
			}
			if (gN == endNode)
				break;
		}
		dPaths << gN;
	}

	x = graph_x.indexOf(endPoint.x());
	y = graph_y.indexOf(endPoint.y());

	for (int i = 0; i < maxX; ++i)
		delete[] graph[i];

	delete[] graph;

	if (dPaths.contains(GrNode(QPoint(x,y))))
		return true;
	return false;
}

void PathCreator::cleanWay()
{
	bool a = true, b = true;
	while(b)
	{
		while(a)
			a = deletePoints();
		a = true;
		b = deleteCorners();
	}
}

QList<QPointF> PathCreator::createPath()
{
	load();
	if (findWay())
	{
		int x = graph_x.indexOf(endPoint.x());
		int y = graph_y.indexOf(endPoint.y());
		GrNode gN = dPaths.at(dPaths.indexOf(QPoint(x,y)));

		road << endPoint;
		QPointF p = endPoint;
		QPoint pI;

		x = graph_x.indexOf(startPoint.x());
		y = graph_y.indexOf(startPoint.y());

		QPoint stP(x,y);

		do
		{
			pI = gN.getParent();
			gN = dPaths.at(dPaths.indexOf(GrNode(pI)));
			p.setX(graph_x.at(pI.x()));
			p.setY(graph_y.at(pI.y()));
			road << p;
		} while (pI != stP);

		cleanWay();
	} else
		road.clear();

	return road;
}

GrNode GrNode::nextPoint(Direction d)
{
	int x = pos.x();
	int y = pos.y();
	bool v_Direction = true;
	switch(d)
	{
		case d_left:
			v_Direction = false;
			if (x > 0)
				x--;
			else
				return GrNode(bounds, x, y, -1);
			break;
		case d_right:
			v_Direction = false;
			if (x < bounds->graph_x.size() - 1)
				x++;
			else
				return GrNode(bounds,x,y,-1);
			break;
		case d_up:
			v_Direction = true;
			if (y > 0)
				y--;
			else
				return GrNode(bounds, x, y, -1);
			break;
		case d_down:
			v_Direction = true;
			if (y < bounds->graph_y.size() - 1)
				y++;
			else
				return GrNode(bounds,x,y,-1);
			break;
	}

	if (bounds->bounds.contains(QPointF(bounds->graph_x.at(x), bounds->graph_y.at(y))))
		return GrNode(bounds, x, y, -1);

	float path_length = -1;

	if (v_Direction)
		path_length = fabs(bounds->graph_y.at(pos.y()) - bounds->graph_y.at(y));
	else
		path_length = fabs(bounds->graph_x.at(pos.x()) - bounds->graph_x.at(x));

	return GrNode(bounds, x, y, path_length + length);
}
