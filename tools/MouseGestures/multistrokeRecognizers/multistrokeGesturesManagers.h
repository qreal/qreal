#pragma once
#include "GeometricForms.h"
#include "abstractRecognizer.h"
#include "distance.h"
#include "sorts.h"
#include "curveKeyBuilder.h"

static const double keyDistance = 200;
static const int size = 8;

class LevenshteinHullGesturesManager : public GesturesRecognizer<Key>
{
public:
    LevenshteinHullGesturesManager() {}
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

    Key getKey(PathVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortCurve(key);
    }
};
class OneSizeHullGesturesManager : public GesturesRecognizer<Key>
{
public:
    OneSizeHullGesturesManager() {}
    double getMaxDistance(QString const &)
    {
        return size * size * e;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getOneSizeDistance(key1, key2);
    }

    Key getKey(PathVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortCurve(key);
    }
};
class LevenshteinXYSortGesturesManager : public GesturesRecognizer<Key>
{
public:
    LevenshteinXYSortGesturesManager() {}
    double getMaxDistance(const QString &)
    {
        return size * size * e;
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

    Key getKey(PathVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortPicture(key);
    }
};
class OneSizeXYSortGesturesManager : public GesturesRecognizer<Key>
{
public:
    OneSizeXYSortGesturesManager() {}
    double getMaxDistance(QString const &)
    {
        return size * size * e;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getOneSizeDistance(key1, key2);
    }

    Key getKey(PathVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return Sorting::sortPicture(key);
    }
};
class SquaresCurveGesturesManager : public GesturesRecognizer<Key>
{
public:
    SquaresCurveGesturesManager() {}
    double getMaxDistance(const QString &)
    {
        return gridSize * e;
    }

    bool isMultistroke()
    {
        return true;
    }

protected:
    double getDistance(Key const & key1, Key const & key2)
    {
        return Distance::getSquaresCurveDistance(key1, key2);
    }

    Key getKey(PathVector const & path)
    {
        return KeyBuilder::getKey(path, gridSize, gridSize);
    }
};

class SimpleMultistrokeManager : public GesturesRecognizer<Key>
{
public:
    SimpleMultistrokeManager() {}
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

    Key getKey(PathVector const & path)
    {
        Key key = KeyBuilder::getKey(path, size, size);
        return key;
    }
};
