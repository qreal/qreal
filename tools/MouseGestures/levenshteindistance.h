#pragma once
#include <QString>
#include <QList>

class LevenshteinDistance
{
public:
    LevenshteinDistance();
    static int getLevenshteinDistance(QString const & key1, QString const & key2);
};
