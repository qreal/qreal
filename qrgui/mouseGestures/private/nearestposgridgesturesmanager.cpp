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

#include "nearestposgridgesturesmanager.h"

#include "keyBuilder.h"

using namespace qReal::gestures;

NearestPosGridGesturesManager::NearestPosGridGesturesManager()
{
}

NearestPosGridGesturesManager::~NearestPosGridGesturesManager()
{
	qDeleteAll(mGestures);
}

qreal NearestPosGridGesturesManager::getMaxDistance(const QString &)
{
	return 1000;
}

bool NearestPosGridGesturesManager::isMultistroke()
{
	return true;
}

qreal NearestPosGridGesturesManager::getDistance(qreal * const &key1, qreal * const &key2)
{
	qreal norm = 0;
	qreal sum = 0;
	for (int i = 0; i < gridSize * gridSize; i++) {
		sum += qAbs(key1[i] - key2[i]);
		norm = qMax(norm, qAbs(key1[i] - key2[i]));
	}

	return norm + sum / (gridSize * gridSize);
}

qreal *NearestPosGridGesturesManager::getKey(const PathVector &path)
{
	Key key = KeyBuilder::getKey(path, gridSize, gridSize);
	qreal * finalKey = new qreal[gridSize * gridSize]; // deal with this too
	for (int i = 0; i < gridSize * gridSize; ++i) {
		finalKey[i] = gridSize;
	}

	if (key.isEmpty()) {
		return finalKey;
	}

	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			qreal dist = qAbs(key.at(0).first - i) + qAbs(key.at(0).second - j);
			for (const SquarePos &pos : key) {
				dist = qMin(dist, qAbs(pos.first - i) + qAbs(pos.second - j));
			}

			finalKey[i * gridSize + j] = dist;
		}
	}

	return finalKey;
}
