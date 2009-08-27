/** @file pathCreator.h
 * 	@brief Класс, отвечающий за красивое размещение связей на диаграмме.
 * */
#pragma once

#include "uml_edgeelement.h"

class GrNode;

class PathCreator
{
public:
	PathCreator(UML::EdgeElement* elem, QPointF const &stP, QPointF const &endP);
	QList<QPointF> createPath();

private:
	static const int defaultStep = 5;

	int step;
	UML::EdgeElement* edge;
	QList<QPointF> road;
	QPainterPath bounds;
	QList<float> graph_x;
	QList<float> graph_y;
	QPointF startPoint;
	QPointF endPoint;
	QList<GrNode> dPaths;

	bool findWay();
	void cleanWay();
	bool deletePoints();
	bool deleteCorners();
	bool canChangeCorner(QPointF const &a, QPointF const &b, QPointF const &c, bool vert) const;
	void load();

	friend class GrNode;
};

enum Direction
{
	d_up,
	d_right,
	d_down,
	d_left,
};

class GrNode
{
public:
	GrNode(const GrNode &n)
	{
		bounds = n.bounds;
		parent = n.parent;
		length = n.length;
		pos = n.pos;
	}

	GrNode(PathCreator* d, int x, int y, float L)
	{
		bounds = d;
		pos.setX(x);
		pos.setY(y);
		length = L;
	}

	GrNode(QPoint const &p)
	{
		pos = p;
	}

	bool operator == (const GrNode& a)
	{
		return pos == a.pos;
	}

	GrNode nextPoint(Direction d);

	float getLength()
	{
		return length;
	}

	QPoint getPos()
	{
		return pos;
	}

	QPoint getParent()
	{
		return parent;
	}

	void setParent(QPoint const &parent)
	{
		this->parent = parent;
	}

private:
	QPoint parent;
	QPoint pos;
	float length;
	PathCreator* bounds;
};
