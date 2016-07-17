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

#include "rectanglegesturesmanager.h"

#include "keyBuilder.h"
#include "sorts.h"

using namespace qReal::gestures;

RectangleGesturesManager::RectangleGesturesManager()
{
}

RectangleGesturesManager::~RectangleGesturesManager()
{
	qDeleteAll(mGestures);
}

qreal RectangleGesturesManager::getMaxDistance(const QString &)
{
	return 1000;
}

bool RectangleGesturesManager::isMultistroke()
{
	return true;
}

qreal RectangleGesturesManager::getDistance(qreal * const & key1, qreal * const & key2)
{
	qreal norm = 0;
	qreal sum = 0;
	for (int i = 0; i < gridSize * gridSize; i ++) {
		sum += qAbs(key1[i] - key2[i]);
		norm = qMax(norm, qAbs(key1[i] - key2[i]));
	}

	return sum / (gridSize * gridSize);
}

qreal *RectangleGesturesManager::getKey(const PathVector & path)
{
	const Key key = KeyBuilder::getKey(path, gridSize, gridSize);
	qreal *finalKey = new qreal[gridSize * gridSize];
	for (int i = 0; i < gridSize * gridSize; ++i) {
		finalKey[i] = key.size();
	}

	for (const SquarePos &pos : key) {
		for (int i = 0; i < pos.first; ++i) {
			for (int j = 0; j < pos.second; ++j) {
				--finalKey[i * gridSize + j];
			}
		}
	}

	return finalKey;
}


