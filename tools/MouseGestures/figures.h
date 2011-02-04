#pragma once
#include <QList>
#include <QPoint>
#include <QDomElement>

static const QString x1Key = "x1";
static const QString x2Key = "x2";
static const QString y1Key = "y1";
static const QString y2Key = "y2";
static const double pi = 3.141592;
static const int pointsOnEllipse = 64;

class Figure
{
public:
	Figure(QDomElement const & object)
	{
		bool isValid;
		mX1 = object.attribute(x1Key, "").toInt(&isValid, 10);
		mX2 = object.attribute(x2Key, "").toInt(&isValid, 10);
		mY1 = object.attribute(y1Key, "").toInt(&isValid, 10);
		mY2 = object.attribute(y2Key, "").toInt(&isValid, 10);
	}
	virtual QList<QPoint> getCurve() = 0;

protected:
	int mX1;
	int mY1;
	int mX2;
	int mY2;
};

class Line : Figure
{
public:
	Line(QDomElement const & object) : Figure(object) {}

	QList<QPoint> getCurve()
	{
		QPoint point1(mX1, mY1);
		QPoint point2(mX2, mY2);
		QList<QPoint> component;
		component.push_back(point1);
		component.push_back(point2);
		return component;
	}
};

class Rectangle : Figure
{
public:
	Rectangle(QDomElement const & object) : Figure(object) {}

	QList<QPoint> getCurve()
	{
		QList<QPoint> component;
		component.push_back(QPoint(mX1, mY1));
		component.push_back(QPoint(mX2, mY1));
		component.push_back(QPoint(mX2, mY2));
		component.push_back(QPoint(mX1, mY2));
		component.push_back(QPoint(mX1, mY1));
		return component;
	}
};

class Ellipse : Figure
{
public:
	Ellipse(QDomElement const & object) : Figure(object) {}

	QList<QPoint> getCurve()
	{
		QPoint point1(mX1, mY1);
		QPoint point2(mX2, mY2);
		QList<QPoint> ellipse;
		QPoint centre = (point1 + point2) / 2;
		int diam = static_cast<int>(sqrt(pow(mX1 - mX2, 2) + pow(mY1 - mY2, 2)));
		for (int i = 0; i < pointsOnEllipse; i++)
		{
			int x = static_cast<int>(diam * cos(2 * pi * i / pointsOnEllipse) / 2);
			int y = static_cast<int>(diam * sin(2 * pi * i / pointsOnEllipse) / 2);
			ellipse.push_back(centre + QPoint(x, y));
		}
		ellipse.push_back(QPoint(centre.x() + diam / 2, centre.y()));
		return ellipse;
	}
};
