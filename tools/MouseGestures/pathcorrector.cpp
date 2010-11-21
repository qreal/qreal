#include "pathcorrector.h"
#include "math.h"
#include <QPoint>

const double mouseSpeed = 3;

QList<QPoint> PathCorrector::getMousePath(QList<QPoint> const & path)
{
	QList<QPoint> mousePath;
	if (path.isEmpty())
		return mousePath;
	mousePath.append(path[0]);
	for (int i = 1; i < path.size(); i++)
	{
		QPoint currentPoint = path[i];
		QPoint previousPoint = path[i - 1];
		int length = currentPoint.x() - previousPoint.x();
		int width = currentPoint.y() - previousPoint.y();
		int number = static_cast<int>((sqrt(pow(length, 2) + pow(width, 2))) / mouseSpeed + 1);
		for (int j = 1; j <= number; j++)
		{
			QPoint point(int(previousPoint.x() + length * j / number),
						 int(previousPoint.y() + width * j / number));
			mousePath.append(point);
		}
	}
	return mousePath;
}

QList<QPoint> PathCorrector::correctPath(QList<QPoint> const & path)
{
	QList<QPoint> newPath;
	if (path.isEmpty())
		return newPath;
	QPoint previousPoint = path[0];
	for (int i = 1; i < path.size(); i++)
	{
		QPoint currentPoint = path[i];
		double speed = (currentPoint - previousPoint).manhattanLength();
		double b = sense * (1 - 1 / exp(speedKoef * speed));
		previousPoint.setX((int) (b * currentPoint.x() + (1 - b) * previousPoint.x()));
		previousPoint.setY((int) (b * currentPoint.y() + (1 - b) * previousPoint.y()));
		newPath.push_back(previousPoint);
	}
	return newPath;
}

bool PathCorrector::isLine(QList<QPoint> const & path)
{
	const int minDifference = 8;
	const double minCos = 0.8;
	int difference = path.size();
	while(difference >= minDifference)
	{
		for (int i = 0; i < path.size() / difference; i++)
		{
			double cos = getCos(path[i * difference], path[((2 * i + 1) * difference - 1) / 2],
								path[(i + 1) * difference - 1]);
			if (cos < minCos)
				return false;
		}
		difference /= 2;
	}
	return true;
}

double PathCorrector::getCos(QPoint const & pnt1, QPoint const & pnt2, QPoint const & pnt3)
{
	QPoint vector1(pnt2.x() - pnt1.x(), pnt2.y() - pnt1.y());
	QPoint vector2(pnt3.x() - pnt2.x(), pnt3.y() - pnt2.y());
	double length1 = (pnt1 - pnt2).manhattanLength();
	double length2 = (pnt2 - pnt3).manhattanLength();
	if(length1 > 0 && length2 > 0)
		return (double) getScalarProduct(vector1, vector2) / (length1 * length2);
	return 1;
}

double PathCorrector::getScalarProduct(QPoint const & vector1, QPoint const & vector2)
{
	return vector1.x() * vector2.x() + vector1.y() * vector2.y();
}

QList<QPoint> PathCorrector::rotate(const QList<QPoint> &path)
{
	QList<QPoint> inversePath;
	foreach (QPoint pnt, path)
	{
		inversePath.push_front(pnt);
	}
	return inversePath;
}

QList<QPoint> PathCorrector::increase(const QList<QPoint> &path, double k)
{
	QList<QPoint> newPath;
	foreach (QPoint pnt, path)
	{
		newPath.push_back(QPoint((int)(pnt.x() * k), (int) (pnt.y() * k)));
	}
	return newPath;
}
