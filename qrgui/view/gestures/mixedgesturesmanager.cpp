#include "mixedgesturesmanager.h"
#include "rectanglegesturesmanager.h"
#include "nearestposgridgesturesmanager.h"
#include "curveKeyBuilder.h"

const double weight1 = 0.2; //0.3: 891 0.2: 899
const double weight2 = 1 - weight1;

MixedGesturesManager::MixedGesturesManager()
{
}

double MixedGesturesManager::getMaxDistance(QString const &)
{
    qDebug() << "max dist is 1000";
    return 1000;
}
bool MixedGesturesManager::isMultistroke()
{
    return true;
}

double MixedGesturesManager::getDistance(QPair<double *,double *> const & key1,
                                         QPair<double *, double *> const & key2)
{
    qDebug() << "try to get distance at MixedGesturesManager";
    double dist1 = (new RectangleGesturesManager())->getDistance(key1.first, key2.first);
    qDebug() << "got distance at RectangleGesturesManager";
    double dist2 = (new NearestPosGridGesturesManager)->getDistance(key1.second, key2.second);
    return dist1 * weight1 + dist2 * weight2;
}

QPair<double *, double *> MixedGesturesManager::getKey(PathVector const & path)
{
    double * key1 = (new RectangleGesturesManager())->getKey(path);
    double * key2 = (new NearestPosGridGesturesManager)->getKey(path);
    return QPair<double *, double *>(key1, key2);
}


