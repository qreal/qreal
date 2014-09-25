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

qreal RectangleGesturesManager::getMaxDistance(QString const &)
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
		sum += std::abs(key1[i] - key2[i]);
		norm = std::max(norm, std::abs(key1[i] - key2[i]));
	}

	return sum / (gridSize * gridSize);
}

qreal *RectangleGesturesManager::getKey(PathVector const & path)
{
	Key const key = KeyBuilder::getKey(path, gridSize, gridSize);
	qreal *finalKey = new qreal[gridSize * gridSize];
	for (int i = 0; i < gridSize * gridSize; ++i) {
		finalKey[i] = key.size();
	}

	for (SquarePos const &pos : key) {
		for (int i = 0; i < pos.first; ++i) {
			for (int j = 0; j < pos.second; ++j) {
				--finalKey[i * gridSize + j];
			}
		}
	}

	return finalKey;
}


