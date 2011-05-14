#include "sumMultistrokeGesturesManager.h"
#include "curveKeyBuilder.h"
#include "sorts.h"
#include "QDebug"

double SumGesturesManager::getMaxDistance(QString const &)
{
    return 1000;
}
bool SumGesturesManager::isMultistroke()
{
    return true;
}

double SumGesturesManager::getDistance(double * const & key1, double * const & key2)
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

double * SumGesturesManager::getKey(PathVector const & path)
{
    Key key = KeyBuilder::getKey(path, gridSize, gridSize);
    key = Sorting::sortPicture(key);
    int sum = 1;
    double * finalKey = new double[gridSize * gridSize];
    for (int i = 0; i < gridSize * gridSize; i ++)
    {
        finalKey[i] = 0;
    }
    for (int i = 0; i < key.size(); i ++)
    {
        SquarePos prevPos = key.at(i);
        SquarePos nextPos(gridSize - 1, gridSize - 1);
        if (i != key.size() - 1)
            nextPos = key.at(i + 1);
        for (int j = prevPos.first * gridSize + prevPos.second;
             j <= nextPos.first * gridSize + nextPos.second; j++)
        {
            finalKey[j] = sum;
            //qDebug() << sum << nextPos;
        }
        sum ++;
    }
    return finalKey;
}


