#pragma once
#include "GeometricForms.h"

static const int minMovement = 20;
static const int heightSize = 7;
static const int widthSize = 7;
static const int unidimensionalLimit = 3;

class KeyBuilder
{
public:
    static Key getKey(const PointVector &path)
    {
        Key key;
        if (path.isEmpty())
            return key;
        SquarePos last;
        SquarePos previous(-1, -1);
        int lower = lowerBound(path);
        int upper = upperBound(path);
        int right = rightBound(path);
        int left = leftBound(path);
        if ((lower - upper) / heightSize < unidimensionalLimit
            && (right - left) / widthSize < unidimensionalLimit)
            return key;
        foreach (QPoint point, path)
        {
            if((lower - upper) / heightSize < unidimensionalLimit)
            {
                last.first = (point.x() - left) * widthSize / (right - left);
                last.second = 0;
            }
            else if((right - left) / widthSize < unidimensionalLimit)
            {
                last.first = 0;
                last.second = (point.y() - upper) * heightSize / (lower - upper);
            }
            else
            {
                last.first = (int)((point.x() - left) * widthSize / (double)(right - left));
                last.second = (int)((point.y() - upper) * heightSize / (double)(lower - upper));
            }
            if (previous != last)
            {
                previous = last;
                key.push_back(last);
            }
        }
        return key;
    }
private:
    static int upperBound(const PointVector & path)
    {
        if (path.isEmpty())
            return 0;
        int upperBound = path.at(0).y();
        foreach (QPoint pnt, path)
            if (pnt.y() < upperBound)
                upperBound = pnt.y();
        return upperBound;
    }
    static int lowerBound(const PointVector & path)
    {
        if (path.isEmpty())
            return 0;
        int lowerBound = path.at(0).y();
        foreach (QPoint pnt, path)
            if (pnt.y() > lowerBound)
                lowerBound = pnt.y();
        return lowerBound;
    }
    static int leftBound(const PointVector & path)
    {
        if (path.isEmpty())
            return 0;
        int leftBound = path.at(0).x();
        foreach (QPoint pnt, path)
            if (pnt.x() < leftBound)
                leftBound = pnt.x();
        return leftBound;
    }
    static int rightBound(const PointVector & path)
    {
        if (path.isEmpty())
            return 0;
        int rightBound = path.at(0).x();
        foreach (QPoint pnt, path)
            if (pnt.x() > rightBound)
                rightBound = pnt.x();
        return rightBound;
    }
};
