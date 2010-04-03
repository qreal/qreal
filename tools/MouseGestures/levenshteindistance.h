#pragma once
#include <QString>
#include <QList>

class LevenshteinDistance
{
public:
    static int getLevenshteinDistance(QString const & key1, QString const & key2);
};
