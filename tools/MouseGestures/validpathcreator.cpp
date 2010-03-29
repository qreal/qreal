#include "validpathcreator.h"

ValidPathCreator::ValidPathCreator()
{
}

QList<QPoint> ValidPathCreator::createPath(Graph const & lines)
{
    if (lines.size() == 0)
    {
        return QList<QPoint>();
    }
    PathVector paths;
    foreach (Line line, lines)
    {
        PointVector path = lineToPath(line);
        int j = 0;
        while (j < paths.size())
        {
            if (canMerge(paths[j], path))
            {
                path = merge(paths[j], path);
                paths.erase(paths.begin() + j);
            }
            else
            {
                j++;
            }
        }
        paths.push_back(path);
    }
    //todo:: слить компоненты связности в одну или вернуть максимальную по кол-ву элементов
    return paths[0];
}

PointVector ValidPathCreator::mergeNotCycles(PointVector path1, PointVector path2)
{
    //todo:: вынести циклы в отдельный метод
    if (path1.back() == path2[0])
    {
        for (int i = 1; i < path2.size(); i++)
        {
            path1.push_back(path2[i]);
        }
        return path1;
    }
    if (path1[0] == path2.back())
    {
        for (int i = 1; i < path1.size(); i++)
            path2.push_back(path1[i]);
        return path2;
    }
    if (path1.back() == path2.back())
    {
        for (int i = 0; i < path2.size() - 1; i++)
        {
            path1.push_back(path2[path2.size() - 1 - i]);
        }
        return path1;
    }
    if (path1[0] == path2[0])
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

QList<QPoint> ValidPathCreator::merge(PointVector path1, PointVector path2)
{
    if (!isCycle(path1) && !isCycle(path2))
    {
        return mergeNotCycles(path1, path2);
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
            if (cycle[i] == graph[j])
            {
                cyclePos = i;
                graphPos = j;
            }
        }
    }
    PointVector mergePath;
    if (graphPos > 0)
    {
        for (int i = 0; i < graphPos - 1; i++)
        {
            mergePath.push_back(graph[i]);
        }
    }
    for (int i = cyclePos; i < cycle.size(); i++)
    {
        mergePath.push_back(cycle[i]);
    }
    if (cyclePos > 0)
    {
        for (int i = 1; i <= cyclePos; i++)
        {
            mergePath.push_back(cycle[i]);
        }
    }
    if (graphPos < graph.size() - 1)
    {
        for (int i = graphPos + 1; i < graph.size(); i++)
        {
            mergePath.push_back(graph[i]);
        }
    }
    return mergePath;
}

bool ValidPathCreator::isCycle(PointVector const & path)
{
    return path[0] == path.back();
}

bool ValidPathCreator::canMerge(PointVector const & path1, PointVector const & path2)
{
    if (path1.size() == 0 || path2.size() == 0)
    {
        return false;
    }
    if (path1[0] == path2[0] || path1.back() == path2[0] ||
        path1.back() == path2.back() || path2.back() == path1[0])
    {
        return true;
    }
    if (!isCycle(path1) && !isCycle(path2))
    {
        return false;
    }
    for (int i = 0; i < path1.size() - 1; i++)
    {
        for (int j = 0; j < path2.size() - 1; j++)
        {
            if (path1[i] == path2[j])
            {
                return true;
            }
        }
    }
    return false;
}

PointVector ValidPathCreator::lineToPath(Line const & line)
{
    PointVector path;
    path.push_back(line.point1);
    path.push_back(line.point2);
    return path;
}
