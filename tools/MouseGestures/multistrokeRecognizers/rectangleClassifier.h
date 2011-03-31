#pragma once
#include "abstractRecognizer.h"
#include "GeometricForms.h"

template <class GesManager>
class RectangleClassifier
{
public:
    RectangleClassifier(const PathVector &path)
    {
        GesManager gManager;
        mKey = gManager.getKey(path);
    }

    RectangleClassifier(){}

    double getDistance(const RectangleClassifier &classifier)
    {
        double * key = classifier.key();
        GesManager gManager;
        return gManager.getDistance(key, mKey);
    }
    RectangleClassifier getPoint(RectangleClassifier const & centre, double centreWeight)
    {
        double * key1 = centre.key();
        double * finalKey = new double[gridSize * gridSize];
        for (int i = 0; i < gridSize * gridSize; i ++)
        {
            finalKey[i] = (key1[i] * centreWeight + mKey[i]) / (centreWeight + 1);
        }
        return RectangleClassifier(finalKey);
    }

    double * key() const
    {
        return mKey;
    }

private:
    RectangleClassifier(double * const & key)
    {
        mKey = key;
    }
    double * mKey;
};
