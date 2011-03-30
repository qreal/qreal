#include "nearestposgridgesturesmanager.h"
#include "curveKeyBuilder.h"
#include "QDebug"

const int rectSize = 80;

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
    for (int i = 0; i < rectSize * rectSize; i ++)
    {
        sum += std::abs(key1[i] - key2[i]);
        norm = std::max(norm, std::abs(key1[i] - key2[i]));
    }
    //return norm / (rectSize * rectSize);
    return norm + sum / (rectSize * rectSize);
}

double * NearestPosGridGesturesManager::getKey(PathVector const & path)
{
    Key key = KeyBuilder::getKey(path, rectSize - 1, rectSize - 1);
    double * finalKey = new double[rectSize * rectSize];
    for (int i = 0; i < rectSize * rectSize; i ++)
    {
        finalKey[i] = rectSize;
    }
    if (key.isEmpty())
        return finalKey;
    for (int i = 0; i < rectSize; i ++)
    {
        for (int j = 0; j < rectSize; j ++)
        {
            double dist = std::abs(key.at(0).first - i) + std::abs(key.at(0).second - j);
            foreach (SquarePos pos, key)
            {
                dist = std::min(dist, std::abs(pos.first - i) + std::abs(pos.second - j));
            }
            finalKey[i * rectSize + j] = dist;
        }
    }
    return finalKey;
}


