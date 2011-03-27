#pragma once
#include <QList>
#include <QPoint>
#include "GeometricForms.h"

class PathCorrector
{
public:
    static QList<QPoint> correctPath(QList<QPoint> const & path);
    static bool isLine(QList<QPoint> const & path);
    static QList<QPoint> getMousePath(QList<QPoint> const & path);
    static QList<QPoint> rotate(QList<QPoint> const & path);
    static QList<QPoint> increase(QList<QPoint> const & path, double k);
    static PathVector distortGesture(PathVector const & idealGesture, double angleA, double angleB);

private:
    static double getCos(QPoint const & pnt1, QPoint const & pnt2, QPoint const & pnt3);
    static double getScalarProduct(QPoint const & vector1, QPoint const & vector2);
    static const double sense = 1;
    static const double speedKoef = 0.0275;
};
