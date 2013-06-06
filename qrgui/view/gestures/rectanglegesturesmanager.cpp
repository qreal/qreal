#include "rectanglegesturesmanager.h"
#include "curveKeyBuilder.h"
#include "sorts.h"

using namespace qReal::gestures;

RectangleGesturesManager::RectangleGesturesManager()
{
}

RectangleGesturesManager::~RectangleGesturesManager()
{
	foreach (QString const object, mGestures.keys()) {
		delete mGestures[object];
		mGestures.remove(object);
	}
}

double RectangleGesturesManager::getMaxDistance(QString const &)
{
	return 1000;
}
bool RectangleGesturesManager::isMultistroke()
{
	return true;
}

double RectangleGesturesManager::getDistance(double * const & key1, double * const & key2)
{
	double norm = 0;
	double sum = 0;
	for (int i = 0; i < gridSize * gridSize; i ++) {
		sum += std::abs(key1[i] - key2[i]);
		norm = std::max(norm, std::abs(key1[i] - key2[i]));
	}
	return sum / (gridSize * gridSize);
}

double * RectangleGesturesManager::getKey(PathVector const & path)
{
	Key key = KeyBuilder::getKey(path, gridSize, gridSize);
	double *finalKey = new double[gridSize * gridSize];
	for (int i = 0; i < gridSize * gridSize; i++)
		finalKey[i] = key.size();
	for (int k = 0; k < key.size(); k++) {
		SquarePos pos = key.at(k);
		for (int i = 0; i < pos.first; i++) {
			for (int j = 0; j < pos.second; j++)
				finalKey[i * gridSize + j]--;
		}
	}
	return finalKey;
}


