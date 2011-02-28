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

class GridClassifier
{
public:
    GridClassifier(const QList<QPoint> &path)
    {
        mKey = KeyBuilder::getKey(path, 120, 120);
    }

    GridClassifier(){}

    double getDistance(const GridClassifier &classifier)
    {
        Key key = classifier.key();
        return Distance::getL1Distance(key, mKey);
    }
    GridClassifier getPoint(GridClassifier const & centre, double centreWeight)
    {
        Key key1 = centre.key();
        if (key1.empty())
            return mKey;
        if (mKey.empty())
            return centre;
        Key key;
        foreach (SquarePos pos, key1)
        {
            SquarePos nearestPos = mKey.at(0);
            double min = Distance::norm(pos, nearestPos);
            foreach (SquarePos pos2, mKey)
            {
                double dist = Distance::norm(pos, pos2);
                if (dist < min)
                {
                    min = dist;
                    nearestPos = pos2;
                }
            }
            SquarePos keyPos((pos.first * (centreWeight - 1) + nearestPos.first) / centreWeight,
                             (pos.second * (centreWeight - 1) + nearestPos.second) / centreWeight);
            if (!key.contains(keyPos))
                key.push_back(keyPos);
        }
        //TODO:: kill Indian code
        foreach (SquarePos pos, mKey)
        {
            SquarePos nearestPos = key1.at(0);
            double min = Distance::norm(pos, nearestPos);
            foreach (SquarePos pos2, key1)
            {
                double dist = Distance::norm(pos, pos2);
                if (dist < min)
                {
                    min = dist;
                    nearestPos = pos2;
                }
            }
            SquarePos keyPos((pos.first + nearestPos.first * (centreWeight - 1)) / centreWeight,
                             (pos.second + nearestPos.second * (centreWeight - 1)) / centreWeight);
            if (!key.contains(keyPos))
                key.push_back(keyPos);
        }
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

//TODO:: rename, it's not neural network but training algorithm
//TODO:: TAKE CLASSIFIER WITH FIXED DIMENION!!!
template <class Classifier>
        class NeuralNetwork : public GesturesManager
{
private:
    QMap<QString, Sphere<Classifier> > mSphereObject;
    double minR;
    Classifier mCurrentClassifier;

public:
    NeuralNetwork()
    {
        minR = 100;
    }

    void learn(QString const & object, QList<QPoint> const & path)
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
    void initIdealGestures(const QMap<QString, QList<QPoint> > &objects)
    {
        foreach (QString name, objects.keys())
        {
            learn(name, objects[name]);
        }
    }
    void setKey(const QList<QPoint> &path)
    {
        mCurrentClassifier = Classifier(path);
    }
    double getMaxDistance(const QString & object)
    {
        return this->mSphereObject[object].rad;
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
