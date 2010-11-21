#pragma once
#include <QString>
#include <QMap>
#include <QWidget>
#include <QMouseEvent>
#include "GeometricForms.h"

class KeyBuilder
{
public:
	virtual void initIdealGestures(QList<Entity> const & objects) = 0;
	virtual void setKey(const QList<QPoint> & path) = 0;
	virtual double getMaxDistance() = 0;
	virtual double getDistance(QString const & object) = 0;
};

template <typename TKey>
		class GesturesRecognizer : public KeyBuilder
{
public:
	GesturesRecognizer(){}

	double getDistance(QString const & item)
	{
		TKey key = mKeys[item];
		return getDistance(mKey, key);
	}

	void initIdealGestures(QList<Entity> const & objects)
	{
		foreach (Entity object, objects)
		{
			TKey key = getKey(object.second);
			mKeys.insert(object.first, key);
		}
	}
	void setKey(QList<QPoint> const & path)
	{
		mKey = getKey(path);
	}

	virtual double getMaxDistance() = 0;

protected:
	TKey mKey;
	virtual double getDistance(TKey const & key1, TKey const & key2) = 0;
	virtual TKey getKey(QList<QPoint> const & path) = 0;
	virtual TKey getKey(PathVector const & components) = 0;
	QMap<QString, TKey> mKeys;
	//maybe to do several lists for multistroke gestures
};

class AbstractRecognizer
{
public:
	AbstractRecognizer(KeyBuilder * recognizer,const QList<Entity> & objects)
	{
		mRecognizer = recognizer;
		mRecognizer->initIdealGestures(objects);
		foreach (Entity entity, objects)
			mObjects.push_back(entity.first);
	}

	QString recognizeObject()
	{
		QString recognizedObject;
		mRecognizer->setKey(mPath);
		double minDist = mRecognizer->getMaxDistance();
		foreach (QString object, mObjects)
		{
			double dist = mRecognizer->getDistance(object);
			if (dist < minDist)
			{
				minDist = dist;
				recognizedObject = object;
			}
		}
		mPath.clear();
		return recognizedObject;
	}

	void mousePress(QPoint const & pnt)
	{
		mPath.push_back(pnt);
	}

	void mouseMove(QPoint const & pnt)
	{
		mPath.push_back(pnt);
	}

	void mouseRelease(QPoint const & pnt)
	{
		mPath.push_back(pnt);
	}

private:
	KeyBuilder * mRecognizer;
	QList<QString> mObjects;
	QList<QPoint> mPath;
};
