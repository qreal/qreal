#include "validpathcreator.h"

ValidPathCreator::ValidPathCreator()
{
}

QList<QPoint> ValidPathCreator::createPath(PathVector const & components)
{
	if (components.size() == 0)
		return QList<QPoint>();
	PathVector paths;
	foreach (PointVector component, components)
	{
		int j = 0;
		while (j < paths.size())
		{
			if (getDistance(paths[j], component) == 0)
			{
				component = merge(paths[j], component, 0);
				paths.erase(paths.begin() + j);
			}
			else
			{
				j++;
			}
		}
		paths.push_back(component);
	}
	return connectComponents(paths);
}

QList<QPoint> ValidPathCreator::connectComponents(const PathVector &components)
{
	if (components.size() == 0)
		return QList<QPoint>();
	PointVector path;
	foreach (PointVector component, components)
	{
		path = merge(path, component, getDistance(path, component));
	}
	return path;
}

double ValidPathCreator::getDistance(const QList<QPoint> &path1, const QList<QPoint> &path2)
{
	if (path1.isEmpty() || path2.isEmpty())
		return 0;
	double min = std::min(getDistance(path1, path2[0]), getDistance(path1, path2.back()));
	if (isCycle(path2))
	{
		foreach (QPoint point, path2)
		{
			min = std::min(min, getDistance(path1, point));
		}
	}
	return min;
}

double ValidPathCreator::getDistance(const QList<QPoint> &path, const QPoint &point)
{
	int min = std::min((point - path[0]).manhattanLength(), (point - path.back()).manhattanLength());
	if (isCycle(path))
	{
		foreach (QPoint pnt, path)
		{
			min = std::min(min, (pnt - point).manhattanLength());
		}
	}
	return min;
}

QList<QPoint> ValidPathCreator::mergeNotCycles(PointVector const &path1, PointVector const &path2, int distance)
{
	if (path1.isEmpty())
		return path2;
	if (path2.isEmpty())
		return path1;
	QList<QPoint> path = path1;
	if ((path1.back() - path2[0]).manhattanLength() == distance)
		return pushBackPath(path1, path2, 0, path2.size() - 1);
	if ((path1[0] - path2.back()).manhattanLength() == distance)
		return pushBackPath(path2, path1, 0, path1.size() - 1);
	if ((path1.back() - path2.back()).manhattanLength() == distance)
	{
		for (int i = 0; i < path2.size(); i++)
		{
			path = pushBackPoint(path, path2[path2.size() - i - 1]);
		}
		return path;
	}
	if ((path1[0] - path2[0]).manhattanLength() == distance)
	{
		QList<QPoint> path;
		for (int i = path2.size() - 1; i >= 0; i--)
		{
			path.push_back(path2[i]);
		}
		foreach (QPoint point, path1)
		{
			path = pushBackPoint(path, point);
		}
		return path;
	}
	return path1;
}

QList<QPoint> ValidPathCreator::pushBackPoint(const QList<QPoint> &path, const QPoint &point)
{
	QList<QPoint> newPath = path;
	if (newPath.isEmpty() || newPath.back() != point)
		newPath.push_back(point);
	return newPath;
}

QList<QPoint> ValidPathCreator::merge(PointVector const &path1, PointVector const &path2, int distance)
{
	if (path1.isEmpty())
		return path2;
	if (path2.isEmpty())
		return path1;
	if (!isCycle(path1) && !isCycle(path2))
		return mergeNotCycles(path1, path2, distance);
	int cyclePos = 0;
	int graphPos = 0;
	PointVector cycle = path1;
	PointVector graph = path1;
	if (isCycle(path1))
		graph = path2;
	else
		cycle = path2;
	for (int i = 0; i < cycle.size(); i++)
	{
		for (int j = 0; j < graph.size(); j++)
		{
			if ((cycle[i] - graph[j]).manhattanLength() == distance)
			{
				cyclePos = i;
				graphPos = j;
			}
		}
	}
	return mergeGraphCycle(cycle, graph, cyclePos, graphPos);
}

QList<QPoint> ValidPathCreator::mergeGraphCycle(const PointVector &cycle, const PointVector &graph, int cyclePos, int graphPos)
{
	PointVector mergePath;
	if (graphPos != 0)
		mergePath = pushBackPath(mergePath, graph, 0, graphPos);
	mergePath = pushBackPath(mergePath, cycle, cyclePos, cycle.size() - 1);
	mergePath = pushBackPath(mergePath, cycle, 1, cyclePos);
	if (graphPos != graph.size() - 1)
		mergePath = pushBackPath(mergePath, graph, graphPos, graph.size() - 1);
	return mergePath;
}

QList<QPoint> ValidPathCreator::pushBackPath(QList<QPoint> mainPath, const QList<QPoint> &path, int begin, int end)
{
	if (begin >= 0 && end < path.size())
	{
		for (int i = begin; i <= end; i++)
		{
			mainPath = pushBackPoint(mainPath, path[i]);
		}
	}
	return mainPath;
}

bool ValidPathCreator::isCycle(PointVector const & path)
{
	return (!path.isEmpty() && path[0] == path.back());
}
