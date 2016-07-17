/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	for (const QString &object : mGestures.keys()) {
		delete[] mGestures[object].first;
		delete[] mGestures[object].second;
		mGestures.remove(object);
	}
}


qreal MixedGesturesManager::getMaxDistance(const QString &)
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

QPair<qreal *, qreal *> MixedGesturesManager::getKey(const PathVector &path)
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

MixedClassifier::MixedClassifier(const PathVector &path)
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

qreal MixedClassifier::getDistance(const MixedClassifier &classifier)
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
