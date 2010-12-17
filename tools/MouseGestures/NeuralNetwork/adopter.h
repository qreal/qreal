#pragma once
#include <QList>
#include <QPoint>
#include <QStringList>

const QString comma = ", ";
const QString pointDelimeter = " : ";

class Adopter
{
public:
	static QList<QPoint> stringToPath(QString const &valueStr)
	{
		QStringList points = valueStr.split(pointDelimeter, QString::SkipEmptyParts);
		QList<QPoint> result;
		foreach (QString str, points)
		{
			QPoint point = stringToPoint(str);
			result.push_back(point);
		}
		return result;
	}

	static QString pathToString(QList<QPoint> const &path)
	{
		QString result = "";
		foreach (QPoint point, path)
		{
			result += pointToString(point) + pointDelimeter;
		}
		return result;
	}

	static QList<QPoint> getMousePath(QList<QPoint> const & path)
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
			int number = 10;
			for (int j = 1; j <= number; j++)
			{
				QPoint point(int(previousPoint.x() + length * j / number),
							 int(previousPoint.y() + width * j / number));
				mousePath.append(point);
			}
		}
		return mousePath;
	}



private:
	static QPoint stringToPoint(QString const &str)
	{
		bool isInt;
		int x = str.section(comma, 0, 0).toInt(&isInt, 0);
		int y = str.section(comma, 1, 1).toInt(&isInt, 0);
		return QPoint(x, y);
	}

	static QString pointToString(QPoint const &p)
	{
		return QString("%1").arg(p.x()) + comma + QString("%1").arg(p.y());
	}

};
