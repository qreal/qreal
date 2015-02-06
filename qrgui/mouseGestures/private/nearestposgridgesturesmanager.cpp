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
		sum += std::abs(key1[i] - key2[i]);
		norm = std::max(norm, std::abs(key1[i] - key2[i]));
	}

	return norm + sum / (gridSize * gridSize);
}

qreal *NearestPosGridGesturesManager::getKey(PathVector const &path)
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
			for (SquarePos const &pos : key) {
				dist = qMin(dist, qAbs(pos.first - i) + qAbs(pos.second - j));
			}

			finalKey[i * gridSize + j] = dist;
		}
	}

	return finalKey;
}
