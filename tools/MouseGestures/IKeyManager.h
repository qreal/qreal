#pragma once
#include <QString>
#include <QPoint>
#include <QList>

class IKeyManager
{
public:
	virtual QString getKey(QList<QPoint> const & path) = 0;
	virtual double getDistance(QString const & key1, QString const & key2) = 0;
};
