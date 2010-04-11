#include "validpathcreator.h"

ValidPathCreator::ValidPathCreator()
{
}

QList<QPoint> ValidPathCreator::createPath(PathVector const & components)
{
    if (components.size() == 0)
    {
        return QList<QPoint>();
    }
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

QList<QPoint> ValidPathCreator::connectComponents(PathVector paths)
{
    QList<QPoint> path = paths[0];
    paths.removeAt(0);
    int min = 1000;
    while (!paths.isEmpty())
    {
        int pos = 0;
        for (int i = 0; i < paths.size(); i++)
        {
            if (min > getDistance(path, paths[i]))
            {
                min = getDistance(path, path[i]);
                pos = min;
            }
        }
        path = merge(path, paths[pos], min);
        paths.removeAt(pos);
    }
    return path;
}

double ValidPathCreator::getDistance(const QList<QPoint> &path1, const QList<QPoint> &path2)
{
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

PointVector ValidPathCreator::mergeNotCycles(PointVector path1, PointVector path2, int distance)
{
    if ((path1.back() - path2[0]).manhattanLength() == distance)
    {
        return pushBackPath(path1, path2, 1, path2.size());
    }
    if ((path1[0] - path2.back()).manhattanLength() == distance)
    {
        return pushBackPath(path2, path1, 1, path1.size());
    }
    if ((path1.back() - path2.back()).manhattanLength() == distance)
    {
        for (int i = 0; i < path2.size() - 1; i++)
        {
            path1.push_back(path2[path2.size() - 1 - i]);
        }
        return path1;
    }
    if ((path1[0] - path2[0]).manhattanLength() == distance)
    {
        QList<QPoint> path;
        for (unsigned i = path2.size() - 1; i > 0; i--)
        {
            path.push_back(path2[i]);
        }
        foreach (QPoint point, path1)
        {
            path.push_back(point);
        }
        return path;
    }
    return path1;
}

//todo:: слить компоненты нормально, чтобы точки не пропадали, если > 1 компонент связности
QList<QPoint> ValidPathCreator::merge(PointVector path1, PointVector path2, int distance)
{
    if (!isCycle(path1) && !isCycle(path2))
    {
        return mergeNotCycles(path1, path2, distance);
    }
    int cyclePos = 0;
    int graphPos = 0;
    PointVector cycle;
    PointVector graph;
    if (isCycle(path1))
    {
        cycle = path1;
        graph = path2;
    }
    else
    {
        cycle = path2;
        graph = path1;
    }
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
    PointVector mergePath;
    mergePath = pushBackPath(mergePath, graph, 0, graphPos - 1);
    mergePath = pushBackPath(mergePath, cycle, cyclePos, cycle.size());
    mergePath = pushBackPath(mergePath, cycle, 1, cyclePos + 1);
    mergePath = pushBackPath(mergePath, graph, graphPos + 1, graph.size());
    return mergePath;
}

QList<QPoint> ValidPathCreator::pushBackPath(QList<QPoint> mainPath, const QList<QPoint> &path, int begin, int end)
{
    if (begin >= 0 && end <= path.size())
    {
        for (int i = begin; i < end; i++)
        {
            mainPath.push_back(path[i]);
        }
    }
    return mainPath;
}

bool ValidPathCreator::isCycle(PointVector const & path)
{
    return path[0] == path.back();
}
