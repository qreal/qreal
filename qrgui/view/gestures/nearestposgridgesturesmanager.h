#pragma once
#include "abstractRecognizer.h"
#include "GeometricForms.h"

class NearestPosGridGesturesManager : public GesturesRecognizer<double *>
{
public:
    NearestPosGridGesturesManager();
    double getMaxDistance(QString const &);
    bool isMultistroke();
    double getDistance(double * const & key1, double * const & key2);
    double * getKey(PathVector const & path);
};
