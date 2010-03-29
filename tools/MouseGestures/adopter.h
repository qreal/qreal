#pragma once
#include <QList>
#include <QPoint>
#include <QString>

//todo:: в этот класс пока пихается все по мелочи, со временем его надо разбить и убить (вмдимо на 2 класса)

class Adopter
{
public:
    Adopter();
    static bool isLine(QList<QPoint> const & path);
    static QString pathToString(QList<QPoint> const & path);
    static QList<QPoint> stringToPath(QString const & str);
    static QList<QPoint> getMousePath(QList<QPoint> const & path);

private:
    static double getCos(QPoint const & pnt1, QPoint const & pnt2, QPoint const & pnt3);
    static double getScalarProduct(QPoint const & vector1, QPoint const & vector2);
    static double getLength(QPoint const & vector);
    static bool isDigit(QChar const & symbol);
    static bool isPath(QString const & str);
};
