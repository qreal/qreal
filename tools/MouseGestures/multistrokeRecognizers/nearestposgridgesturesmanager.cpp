#include "nearestposgridgesturesmanager.h"
#include "curveKeyBuilder.h"
#include "QDebug"

NearestPosGridGesturesManager::NearestPosGridGesturesManager()
{
}

double NearestPosGridGesturesManager::getMaxDistance(QString const &)
{
    return 1000;
}
bool NearestPosGridGesturesManager::isMultistroke()
{
    return true;
}

double NearestPosGridGesturesManager::getDistance(double * const & key1, double * const & key2)
{
    double norm = 0;
    double sum = 0;
    for (int i = 0; i < gridSize * gridSize; i ++)
    {
        sum += std::abs(key1[i] - key2[i]);
        norm = std::max(norm, std::abs(key1[i] - key2[i]));
    }
    //return norm / (gridSize * gridSize);
    return norm + sum / (gridSize * gridSize);
}

double * NearestPosGridGesturesManager::getKey(PathVector const & path)
{
    Key key = KeyBuilder::getKey(path, gridSize - 1, gridSize - 1);
    double * finalKey = new double[gridSize * gridSize];
    for (int i = 0; i < gridSize * gridSize; i ++)
    {
        finalKey[i] = gridSize;
    }
    if (key.isEmpty())
        return finalKey;
    for (int i = 0; i < gridSize; i ++)
    {
        for (int j = 0; j < gridSize; j ++)
        {
            double dist = std::abs(key.at(0).first - i) + std::abs(key.at(0).second - j);
            foreach (SquarePos pos, key)
            {
                dist = std::min(dist, std::abs(pos.first - i) + std::abs(pos.second - j));
            }
            finalKey[i * gridSize + j] = dist;
        }
    }
    return finalKey;
}


