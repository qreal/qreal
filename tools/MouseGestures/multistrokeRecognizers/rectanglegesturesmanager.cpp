#include "rectanglegesturesmanager.h"
#include "curveKeyBuilder.h"
#include "sorts.h"
#include "QDebug"

const int rectSize = 80;

//TODO::kill copy past
RectangleGesturesManager::RectangleGesturesManager()
{
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
    for (int i = 0; i < rectSize * rectSize; i ++)
    {
        sum += std::abs(key1[i] - key2[i]);
        norm = std::max(norm, std::abs(key1[i] - key2[i]));
    }
    return sum / (rectSize * rectSize);
}

double * RectangleGesturesManager::getKey(PathVector const & path)
{
    Key key = KeyBuilder::getKey(path, rectSize - 1, rectSize - 1);
    double * finalKey = new double[rectSize * rectSize];
    for (int i = 0; i < rectSize * rectSize; i ++)
    {
        finalKey[i] = key.size();
    }
    for (int k = 0; k < key.size(); k ++)
    {
        SquarePos pos = key.at(k);
        for (int i = 0; i < pos.first; i ++)
        {
            for (int j = 0; j < pos.second; j ++)
            {
                finalKey[i * rectSize + j] --;
            }
        }
    }
    return finalKey;
}


