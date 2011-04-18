#pragma once
#include "abstractRecognizer.h"
#include "GeometricForms.h"

class MixedGesturesManager : public GesturesRecognizer<QPair<double *, double *> >
{
public:
    MixedGesturesManager();
    double getMaxDistance(QString const &);
    bool isMultistroke();
    double getDistance(QPair<double *, double *> const & key1, QPair<double *, double *> const & key2);
    QPair<double *, double *> getKey(PathVector const & path);
};
