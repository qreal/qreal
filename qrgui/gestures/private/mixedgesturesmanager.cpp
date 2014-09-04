#include "mixedgesturesmanager.h"

#include "rectanglegesturesmanager.h"
#include "nearestposgridgesturesmanager.h"
#include "keyBuilder.h"

const qreal weight1 = 0.2; //0.3: 891 0.2: 899
const qreal weight2 = 1 - weight1;

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


qreal MixedGesturesManager::getMaxDistance(QString const &)
{
	return 30;
}
bool MixedGesturesManager::isMultistroke()
{
	return true;
}

qreal MixedGesturesManager::getDistance(QPair<qreal *,qreal *> const &key1, QPair<qreal *, qreal *> const &key2)
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

MixedClassifier::MixedClassifier(QPair<qreal *, qreal *> const &key)
{
	mKey = key;
}

MixedClassifier::MixedClassifier(PathVector const &path)
{
	MixedGesturesManager gManager;
	mKey = gManager.getKey(path);
}

MixedClassifier::MixedClassifier()
{
}

MixedClassifier::~MixedClassifier()
{
	delete mKey.first;
	delete mKey.second;
}

qreal MixedClassifier::getDistance(MixedClassifier const &classifier)
{
	QPair<qreal *, qreal *> key = classifier.key();
	MixedGesturesManager gManager;
	return gManager.getDistance(key, mKey);
}

MixedClassifier MixedClassifier::getPoint(const MixedClassifier &centre, qreal centreWeight)
{
	qreal *key1 = centre.key().first;
	qreal *key2 = centre.key().second;
	qreal *finalKey1 = new qreal[gridSize * gridSize];
	qreal *finalKey2 = new qreal[gridSize * gridSize];
	for (int i = 0; i < gridSize * gridSize; i ++) {
		finalKey1[i] = (key1[i] * centreWeight + mKey.first[i]) / (centreWeight + 1);
		finalKey2[i] = (key2[i] * centreWeight + mKey.second[i]) / (centreWeight + 1);
	}

	return MixedClassifier(QPair<qreal *, qreal *>(finalKey1, finalKey2));
}

QPair<qreal *, qreal *> MixedClassifier::key() const
{
	return mKey;
}
