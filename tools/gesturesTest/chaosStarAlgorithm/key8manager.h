#pragma once
#include <QPoint>
#include <QString>
#include <QList>

class Key8Manager
{
public:
	Key8Manager();
	QString getKey(QList<QPoint> const & path);

private:
	static const double pi = 3.141592;

	static QString filterKey(QString const &path);
	static bool isClose(QChar a, QChar b);
};
