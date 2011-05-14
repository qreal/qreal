#pragma once
#include "GeometricForms.h"
#include "pathcorrector.h"
#include "cmath"

static const int minMovement = 20;
//static const int heightSize = 30;
//static const int widthSize = 30;
static const double maxRelation = 8;
static const int minPoint = -1000;

class KeyBuilder
{
public:
    static Key getKey(const PathVector &mousePath, int heightSize, int widthSize)
    {
        Key key;
        if (mousePath.isEmpty())
            return key;
        double lower = lowerBound(mousePath);
        double upper = upperBound(mousePath);
        double right = rightBound(mousePath);
        double left = leftBound(mousePath);
        if (right - left < minMovement && lower - upper < minMovement)
            return key;
        foreach (PointVector path, mousePath)
        {
            SquarePos previous(minPoint, minPoint);
            SquarePos last;
            foreach (QPoint point, path)
            {
                if((lower - upper) * maxRelation  < right - left)
                {
                    last.first = (point.x() - left) * widthSize / (right - left);
                    last.second = 0;
                }
                else if((right - left) * maxRelation < lower - upper)
                {
                    last.first = 0;
                    last.second = (point.y() - upper) * heightSize / (lower - upper);
                }
                else
                {
                    last.first = (int)((point.x() - left) * widthSize / (right - left));
                    last.second = (int)((point.y() - upper) * heightSize / (lower - upper));
                }
                if (last.first == widthSize)
                    last.first --;
                if (last.second == heightSize)
                    last.second --;
                if (previous.first != minPoint || previous.second != minPoint)
                {
                    rasterizeSegment(previous, last, &key);
                }
                previous = last;
            }
        }
        return key;
    }
private:
    static int upperBound(const PathVector & mousePath)
    {
        if (mousePath.isEmpty())
            return 0;
        int upperBound = mousePath.at(0).at(0).y();
        foreach (PointVector path, mousePath)
        {
            foreach (QPoint pnt, path)
                if (pnt.y() < upperBound)
                    upperBound = pnt.y();
        }
        return upperBound;
    }
    static int lowerBound(const PathVector & mousePath)
    {
        if (mousePath.isEmpty())
            return 0;
        int lowerBound = mousePath.at(0).at(0).y();
        foreach (PointVector path, mousePath)
        {
            foreach (QPoint pnt, path)
                if (pnt.y() > lowerBound)
                    lowerBound = pnt.y();
        }
        return lowerBound;
    }
    static int leftBound(const PathVector & mousePath)
    {
        if (mousePath.isEmpty())
            return 0;
        int leftBound = mousePath.at(0).at(0).x();
        foreach (PointVector path, mousePath)
        {
            foreach (QPoint pnt, path)
                if (pnt.x() < leftBound)
                    leftBound = pnt.x();
        }
        return leftBound;
    }
    static int rightBound(const PathVector & mousePath)
    {
        if (mousePath.isEmpty())
            return 0;
        int rightBound = mousePath.at(0).at(0).x();
        foreach (PointVector path, mousePath)
        {
            foreach (QPoint pnt, path)
                if (pnt.x() > rightBound)
                    rightBound = pnt.x();
        }
        return rightBound;
    }
    static void rasterizeSegment(SquarePos const & pos1, SquarePos const & pos2, Key * segment)
    {
        if (!segment->isEmpty() && pos1 == segment->at(0))
            segment->pop_back();
        if (pos1 == pos2)
        {
            segment->push_back(pos1);
            return;
        }
        int x = pos1.first;
        int y = pos1.second;
        int deltaX = abs(pos2.first - x);
        int deltaY = abs(pos2.second - y);
        int signX = sign(pos2.first - x);
        int signY = sign(pos2.second - y);
        bool isChanged = false;
        if (deltaY > deltaX)
        {
            int c = deltaX;
            deltaX = deltaY;
            deltaY = c;
            isChanged = true;
        }
        int e = 2 * deltaY - deltaX;
        for (int i = 0; i < deltaX; i ++)
        {
            segment->push_back(SquarePos(x, y));
            while (e >= 0)
            {
                if (isChanged)
                    x += signX;
                else
                    y +=signY;
                e -= 2 * deltaX;
            }
            if (isChanged)
                y += signY;
            else
                x += signX;
            e += 2 * deltaY;
        }
    }
    static int sign(int a)
    {
        if (a < 0)
            return -1;
        else if (a > 0)
            return 1;
        else return 0;
    }
};
