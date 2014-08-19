#include "mixedgesturesmanager.h"

#include "rectanglegesturesmanager.h"
#include "nearestposgridgesturesmanager.h"
#include "keyBuilder.h"

const double weight1 = 0.2; //0.3: 891 0.2: 899
const double weight2 = 1 - weight1;

using namespace qReal::gestures;

MixedGesturesManager::MixedGesturesManager()
{
}

MixedGesturesManager::~MixedGesturesManager()
{
	for (QString const &object : mGestures.keys()) {
		delete mGestures[object].first;
		delete mGestures[object].second;
		mGestures.remove(object);
	}
}


double MixedGesturesManager::getMaxDistance(QString const &)
{
	return 30;
}
bool MixedGesturesManager::isMultistroke()
{
	return true;
}

double MixedGesturesManager::getDistance(QPair<double *,double *> const &key1, QPair<double *, double *> const &key2)
{
	RectangleGesturesManager rectMan;
	NearestPosGridGesturesManager gridMan;
	qreal dist1 = rectMan.getDistance(key1.first, key2.first);
	qreal dist2 = gridMan.getDistance(key1.second, key2.second);
	return dist1 * weight1 + dist2 * weight2;
}

QPair<qreal *, qreal *> MixedGesturesManager::getKey(PathVector const &path)
{
	RectangleGesturesManager rectMan;
	NearestPosGridGesturesManager gridMan;
	qreal *key1 = rectMan.getKey(path);
	qreal *key2 = gridMan.getKey(path);
	return QPair<qreal *, qreal *>(key1, key2);
}

MixedClassifier::~MixedClassifier()
{
	delete mKey.first;
	delete mKey.second;
}
