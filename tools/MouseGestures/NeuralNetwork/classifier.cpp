#include "classifier.h"
#include <stdio.h>
#include <math.h>
#include <QDebug>

Classifier::Classifier(const QList<QPoint> &path)
{
    //qDebug() << "try to build classifier";
    for (int i = 0; i < size * size; i ++)
    {
        mPoint[i] = 0.0;
        mPoint[i + size * size] = 1.0;
    }
    int left = leftBound(path);
    int right = rightBound(path);
    int upper = upperBound(path);
    int lower = lowerBound(path);
    //qDebug() << "got bounds" << left << right << upper << lower;
    if (right == left || lower == upper)
        return;
    foreach (QPoint point, path)
    {
        int x = (point.x() - left) * (size - 1) / (right - left);
        int y = (point.y() - upper) * (size - 1) / (lower - upper);
        //qDebug() << x << y <<point.x() <<point.y() << right << left <<lower<< upper;
        mPoint[x * size + y] = 1;
        //qDebug() << x * size + y;
        mPoint[x * size + y + size * size] = 0;
        //qDebug() << x * size + y + size * size;
    }
}

int Classifier::leftBound(const QList<QPoint> &path)
{
    int left;
    if (!path.isEmpty())
    {
        left = path[0].x();
    }
    foreach (QPoint pnt, path)
    {
        if (pnt.x() < left) left = pnt.x();
    }
    return left;
}

int Classifier::rightBound(const QList<QPoint> &path)
{
    int right;
    if (!path.isEmpty())
    {
        right = path[0].x();
    }
    foreach (QPoint pnt, path)
    {
        if (pnt.x() > right) right = pnt.x();
    }
    return right;
}

int Classifier::upperBound(const QList<QPoint> &path)
{
    int upper;
    if (path.size() > 0)
    {
        upper = path[0].y();
    }
    foreach (QPoint pnt, path)
    {
        if (pnt.y() < upper) upper = pnt.y();
    }
    return upper;
}

int Classifier::lowerBound(const QList<QPoint> &path)
{
    int lower;
    if (path.size() > 0)
    {
        lower = path[0].y();
    }
    foreach (QPoint pnt, path)
    {
        if (pnt.y() > lower) lower = pnt.y();
    }
    return lower;
}

Classifier::Classifier(double point[2 * size * size])
{
    for(int i = 0; i < size* size * 2; i++)
    {
        mPoint[i] = point[i];
    }
}

Classifier Classifier::operator - (Classifier const & other)
{
    double newPoint[size * size * 2];
    for (int i = 0; i < size * size * 2; i++)
    {
        newPoint[i] = mPoint[i] - other.mPoint[i];
    }
    return Classifier(newPoint);
}

Classifier Classifier::operator /(double a)
{
    double newPoint[size * size * 2];
    for (int i = 0; i < size * size * 2; i++)
    {
        newPoint[i] = mPoint[i] / a;
    }
    return Classifier(newPoint);
}

double Classifier::norm()
{
    double norm = 0;
    for (int i = 0; i < size * size * 2; i++)
    {
        norm += fabs(mPoint[i]);
    }
    return norm;
}
