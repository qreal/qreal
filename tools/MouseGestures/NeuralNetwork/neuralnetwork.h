#pragma once
#include <QMap>
#include <QString>
#include <QList>
#include <QPoint>
#include <QDebug>
#include "adopter.h"
#include "abstractRecognizer.h"
#include "multistrokeRecognizers/curveKeyBuilder.h"
#include "multistrokeRecognizers/distance.h"

//static const double e = 10;
//static const int gridSize = 80;//gridSize = 100 recognition = 833 gestures

class GridClassifier
{
public:
    GridClassifier(const PathVector &path)
    {
        mKey = KeyBuilder::getKey(path, gridSize, gridSize);
    }

    GridClassifier(){}

    double getDistance(const GridClassifier &classifier)
    {
        Key key = classifier.key();
        return Distance::getSquaresCurveDistance(key, mKey);
    }
    GridClassifier getPoint(GridClassifier const & centre, double centreWeight)
    {
        Key key1 = centre.key();
        if (key1.empty())
            return mKey;
        if (mKey.empty())
            return centre;
        Key key;
        key = getSquares(key1, mKey, key, centreWeight - 1, 1);
        key = getSquares(mKey, key1, key, 1, centreWeight - 1);
        return GridClassifier(key);
    }

    Key key() const
    {
        return mKey;
    }

private:
    Key mKey;
    GridClassifier(Key key)
    {
        mKey = key;
    }
    Key getSquares(const Key & key1, const Key & key2,
                   const Key & finalKey, int weight1, int weight2)
    {
        Key key = finalKey;
        int sumWeight = weight1 + weight2;
        foreach (SquarePos pos, key1)
        {
            SquarePos nearestPos = key2.at(0);
            double min = Distance::norm(pos, nearestPos);
            foreach (SquarePos pos2, key2)
            {
                double dist = Distance::norm(pos, pos2);
                if (dist < min)
                {
                    min = dist;
                    nearestPos = pos2;
                }
            }
            SquarePos keyPos((pos.first * weight1 + nearestPos.first * weight2) / sumWeight,
                             (pos.second * weight1 + nearestPos.second * weight2) / sumWeight);
            if (!key.contains(keyPos))
                key.push_back(keyPos);
        }
        return key;
    }
};

template <class Classifier>

        struct Sphere
{
public:
    Classifier centre;
    double rad;
    int pointNum;
    Sphere(const Classifier & c, double r)
    {
        centre = c;
        rad = r;
        pointNum = 1;
    }
    Sphere(){}
};

template <class Classifier>
        class TrainingGesturesManager : public GesturesManager
{
private:
    QMap<QString, Sphere<Classifier> > mSphereObject;
    double minR;
    Classifier mCurrentClassifier;

public:
    TrainingGesturesManager()
    {
        minR = gridSize * e / 5;
    }
    bool belong(const QString & object, const PathVector & path)
    {
        Sphere<Classifier> sphere = this->mSphereObject[object];
        Classifier point(path);
        return point.getDistance(sphere.centre) < sphere.rad;
    }

    void learn(const QString &object, const PointVector &path)
    {
        PathVector learnPath;
        learnPath.append(path);
        learn(object, learnPath);
    }
    void learn(QString const & object, PathVector const & path)
    {
        Classifier newPoint(path);
        qDebug() << "classifier built";
        if (!mSphereObject.keys().contains(object))
        {
            Sphere<Classifier> sphere(newPoint, minR);
            mSphereObject.insert(object, sphere);
            qDebug() << "sphere created";
            return;
        }
        Sphere<Classifier> sphere = mSphereObject[object];
        int pointNum = sphere.pointNum + 1;
        sphere.centre = newPoint.getPoint(sphere.centre, pointNum);
        double dist = newPoint.getDistance(sphere.centre);
        sphere.rad = std::max(dist * (pointNum - 1) / pointNum,
                              sphere.rad + dist / pointNum);
        mSphereObject[object] = sphere;
    }

    void clear()
    {
        mSphereObject.clear();
    }
    void initIdealGestures(const QMap<QString, PathVector> &objects)
    {
        foreach (QString name, objects.keys())
        {
            learn(name, objects[name]);
        }
    }
    void setKey(const PathVector &path)
    {
        mCurrentClassifier = Classifier(path);
    }
    double getMaxDistance(const QString & object)
    {
        return std::max(this->mSphereObject[object].rad, gridSize * e);
    }

    double getDistance(const QString &object)
    {
        return mCurrentClassifier.getDistance(this->mSphereObject[object].centre);
    }

    bool isMultistroke()
    {
        return true;
    }

    void setMinRad(double r)
    {
        minR = r;
        clear();
    }
};
