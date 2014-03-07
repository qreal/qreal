#include "umllib/private/lineFactory.h"

#include "umllib/private/brokenLine.h"
#include "umllib/private/squareLine.h"
#include "umllib/private/curveLine.h"

using namespace qReal;

LineFactory::LineFactory(EdgeElement *edge)
		: mEdge(edge)
{
}

LineHandler * LineFactory::createHandler(linkShape::LinkShape const type) const
{
	switch(type) {
	case linkShape::broken:
		return new BrokenLine(mEdge);
	case linkShape::curve:
		return new CurveLine(mEdge);
	default:
		return new SquareLine(mEdge);
	}
}

QMenu * LineFactory::shapeTypeMenu() const
{
	QMenu * const menu = new QMenu();

	QAction * const brokenLine = menu->addAction(tr("Broken"));
	connect(brokenLine, SIGNAL(triggered()), this, SLOT(setBrokenLine()));

	QAction * const squareLine = menu->addAction(tr("Square"));
	connect(squareLine, SIGNAL(triggered()), this, SLOT(setSquareLine()));

	QAction * const curveLine = menu->addAction(tr("Curve"));
	connect(curveLine, SIGNAL(triggered()), this, SLOT(setCurveLine()));

	return menu;
}

QString LineFactory::shapeToString(linkShape::LinkShape const shapeType)
{
	switch (shapeType) {
	case linkShape::broken:
		return "broken";
	case linkShape::curve:
		return "curve";
	default:
		return "square";
	}
}

linkShape::LinkShape LineFactory::stringToShape(QString const &string)
{
	if (string == "broken") {
		return linkShape::broken;
	} else if (string == "square") {
		return linkShape::square;
	} else if (string == "curve") {
		return linkShape::curve;
	} else {
		return linkShape::unset;
	}
}

void LineFactory::setSquareLine() const
{
	mEdge->changeShapeType(linkShape::square);
}

void LineFactory::setBrokenLine() const
{
	mEdge->changeShapeType(linkShape::broken);
}

void LineFactory::setCurveLine() const
{
	mEdge->changeShapeType(linkShape::curve);
}
