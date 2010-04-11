#pragma once
#include <QList>
#include <QPoint>
#include <QString>

//todo:: в этот класс пока пихается все по мелочи, со временем его надо разбить и убить (вмдимо на 2 класса)

class Adopter
{
public:
    Adopter();
    static QString pathToString(QList<QPoint> const & path);
    static QList<QPoint> stringToPath(QString const & str);

private:
    static bool isDigit(QChar const & symbol);
    static bool isPath(QString const & str);
};
