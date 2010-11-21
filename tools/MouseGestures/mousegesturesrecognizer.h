#pragma once
#include "GeometricForms.h"

//template <typename TKey>

class MouseGesturesRecognizer
{
public:
	MouseGesturesRecognizer(EntityVector const & idealGestures);
	QString getObject(QList<QPoint> const & path);

//protected:
//	double getDistance(TKey const & key1, TKey const & key2);
//	TKey getKey(QList<QPoint> const & path);
//	PointVector createPath(PathVector const & paths);
//	IdealGestures mIdealKeys;
//	float mMaxKeyDistance;
};
