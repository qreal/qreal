#pragma once
#include "GeometricForms.h"
#include "abstractRecognizer.h"
#include "distance.h"
#include "sorts.h"
#include "curveKeyBuilder.h"

static const double keyDistance = 200;
static const int size = 8;
static const int gridSize = 80;//gridSize = 100 recognition = 833 gestures

//todo:: RENAME!!!!!! and do smth with Indian code
class LevCurveGesturesManager : public GesturesRecognizer<Key>
{
public:
    LevCurveGesturesManager() {}
    double getMaxDistance(QString const &)
    {
        return keyDistance;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getLevenshteinDistance(key1, key2);
    }

    Key getKey(PointVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortCurve(key);
    }
};
class CurveDistCurveSortGesturesManager : public GesturesRecognizer<Key>
{
public:
    CurveDistCurveSortGesturesManager() {}
    double getMaxDistance(QString const &)
    {
        //todo:: change max distance
        return keyDistance;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getCurveDistance(key1, key2);
    }

    Key getKey(PointVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortCurve(key);
    }
};
class LevPictureGesturesManager : public GesturesRecognizer<Key>
{
public:
    LevPictureGesturesManager() {}
    double getMaxDistance(const QString &)
    {
        //todo: change max distance
        return keyDistance;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getLevenshteinDistance(key1, key2);
    }

    Key getKey(PointVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortPicture(key);
    }
};
class CurvePictureGesturesManager : public GesturesRecognizer<Key>
{
public:
    CurvePictureGesturesManager() {}
    double getMaxDistance(QString const &)
    {
        //todo:: change max distance
        return keyDistance;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getCurveDistance(key1, key2);
    }

    Key getKey(PointVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortPicture(key);
    }
};
class L1GesturesManager : public GesturesRecognizer<Key>
{
public:
    L1GesturesManager() {}
    double getMaxDistance(const QString &)
    {
        //todo:: change max distance
        return keyDistance;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getL1Distance(key1, key2);
    }

    Key getKey(PointVector const & path)
    {
        return KeyBuilder::getKey(path, gridSize, gridSize);
    }
};
