#pragma once
#include "IKeyManager.h"
#include <QPoint>
#include <QList>
#include <QMap>
#include <QPair>

class MouseMovementManager : public IKeyManager
{
public:
	MouseMovementManager();
	QString getKey(QList<QPoint> const & path);
	double getDistance(QString const & key1, QString const & key2);

private:
	void analysePoints(QList<QPoint> const & path);
	void clear();
	void createWeights();
	int strIndex(int row, int column);
	int mLeftBound;
	int mRightBound;
	int mUpperBound;
	int mLowerBound;
	QMap<QPair<QChar, QChar>, double> mWeights;
};
