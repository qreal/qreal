#pragma once
#include <QPoint>
#include <QList>
#include <QMap>
#include <QPair>
#include "abstractRecognizer.h"

class MouseMovementManager : GesturesRecognizer<QString>
{
public:
	MouseMovementManager();
	QString getKey(QList<QPoint> const & path);
	double getMaxDistance();

protected:
	double getDistance(QString const & key1, QString const & key2);
	QString getKey(const PathVector & components);

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
