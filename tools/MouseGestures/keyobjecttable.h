#pragma once
#include "GeometricForms.h"
#include "mousemovementmanager.h"

// эту таблицу надо будет сериализовать

class KeyObjectTable
{
public:
	KeyObjectTable();
	void add(QString const &object, QList<QPoint> const &correctPath);
	void add(Objects const &objects);
	void setPath(QString const &object, QList<QPoint> const &correctPath);
	QString getObject(QList<QPoint> const &path);
	QString getKey(QString const &name);
	QList<QPoint> getPath(QString const &name);
	Objects getObjects();
	KeyObjectItem at(int pos);
	int size();
	void clear();

private:
	KeyObjectItem getItem(QString const &name);
	KeyObjectItems mKeyObjectTable;
	MouseMovementManager mMouseMovementManager;
};
