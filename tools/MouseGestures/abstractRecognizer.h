#pragma once
#include <QString>
#include <QMap>
#include <QWidget>
#include <QMouseEvent>
#include "GeometricForms.h"
#include "validpathcreator.h"
#include "pathcorrector.h"
#include "paintmanager.h"
#include <QDebug>

class GesturesManager
{
public:
    virtual void initIdealGestures(QMap<QString, QList<QPoint> > const & objects) = 0;
    virtual void setKey(const QList<QPoint> & path) = 0;
    virtual double getMaxDistance(QString const & object) = 0;
    virtual double getDistance(QString const & object) = 0;
    virtual bool isMultistroke() = 0;
};

template <typename TKey>
        class GesturesRecognizer : public GesturesManager
{
public:
    GesturesRecognizer(){}

    double getDistance(QString const & item)
    {
        TKey key = mGestures[item];
        return getDistance(mKey, key);
    }

    void initIdealGestures(QMap<QString, QList<QPoint> > const & objects)
    {
        foreach (QString object, objects.keys())
        {
            qDebug() << object;
            TKey key = getKey(objects[object]);
            mGestures.insert(object, key);
            qDebug() << object << "inserted";
        }
    }

    void setKey(QList<QPoint> const & path)
    {
        mKey = getKey(path);
    }

    virtual double getMaxDistance(QString const & object) = 0;

protected:
    TKey mKey;
    virtual double getDistance(TKey const & key1, TKey const & key2) = 0;
    virtual TKey getKey(QList<QPoint> const & path) = 0;
    QMap<QString, TKey> mGestures;
    //maybe to do several lists for multistroke gestures
};

class AbstractRecognizer
{
public:
    AbstractRecognizer(GesturesManager * recognizer, const QList<Entity> & objects)
    {
        mGesturesManager = recognizer;
        setIdealGestures(objects);
    }

    AbstractRecognizer(GesturesManager * recognizer)
    {
        mGesturesManager = recognizer;
    }

    void setIdealGestures(const QList<Entity> & objects)
    {
        mObjects.clear();
        QMap<QString, QList<QPoint> > gestures;
        foreach (Entity entity, objects)
        {
            PathVector paths;
            if (!mGesturesManager->isMultistroke())
                paths.push_back(ValidPathCreator::createPath(entity.second));
            else
                paths = entity.second;
            mObjects.insert(entity.first, paths);
            QList<QPoint> path;
            foreach (QList<QPoint> gesture, paths)
            {
                path.append(PathCorrector::getMousePath(gesture));
            }
            gestures.insert(entity.first, path);
        }
        mGesturesManager->initIdealGestures(gestures);
    }

    QString recognizeObject()
    {
        if (mObjects.empty())
            return "";
        QString recognizedObject;
        if (!mGesturesManager->isMultistroke())
            mGesturesManager->setKey(mGesture.back());
        else
        {
            QList<QPoint> gesturePath;
            foreach (QList<QPoint> path, mGesture)
                gesturePath.append(path);
            mGesturesManager->setKey(gesturePath);
        }
        double minDist = mGesturesManager->getMaxDistance(mObjects.keys().at(0));
        foreach (QString object, mObjects.keys())
        {
            minDist = std::min(minDist, mGesturesManager->getMaxDistance(object));
            double dist = mGesturesManager->getDistance(object);
            if (dist < minDist)
            {
                minDist = dist;
                recognizedObject = object;
            }
        }
        mGesture.clear();
        return recognizedObject;
    }

    void mousePress(QPoint const & pnt)
    {
        QList<QPoint> path;
        path.push_back(pnt);
        mGesture.push_back(path);
    }

    void mouseMove(QPoint const & pnt)
    {
        PointVector path = mGesture.back();
        mGesture.pop_back();
        path.push_back(pnt);
        mGesture.push_back(path);
    }

    void mouseRelease(QPoint const & pnt)
    {
        mouseMove(pnt);
    }

    QList<QString> getObjects()
    {
        return mObjects.keys();
    }

    void drawGesture(QString const & object, PaintManager * paintManager)
    {
        if (mGesturesManager->isMultistroke())
            paintManager->drawGesture(mObjects[object]);
        else
            paintManager->drawGesture(mObjects[object].back());
    }

    PathVector getGesture()
    {
        return mGesture;
    }

    QString recognizeObject(QList<QPoint> const & gesture)
    {
        mGesture.clear();
        mGesture.append(gesture);
        return recognizeObject();
    }

private:
    GesturesManager * mGesturesManager;
    QMap<QString, PathVector> mObjects;
    PathVector mGesture;
};
