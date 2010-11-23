#pragma once
#include "GeometricForms.h"
#include "abstractRecognizer.h"
#include <QPair>
#include <QList>

class MultistrokeGesturesManager : public GesturesRecognizer<Key>
{
public:
	MultistrokeGesturesManager() {}
	double getMaxDistance();
	bool isMultistroke()
	{
		return true;
	}

protected:
	double getDistance(Key const & key1, Key const & key2);
	Key getKey(PointVector const & path);

private:
	Key transferKey(const Key & key, int size);
	Key sort(Key const & key);
	void analysePoints(QList<QPoint> const & path);
	double norm(SquarePos const & pos1, SquarePos const & pos2);
	PointVector createPath(PathVector const & paths);
	int mLeftBound;
	int mRightBound;
	int mUpperBound;
	int mLowerBound;
};
