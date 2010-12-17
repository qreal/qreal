#pragma once
#include <QMap>
#include <QString>
#include <QList>
#include <QPoint>
#include <QDebug>
#include "classifier.h"
#include "adopter.h"

class Sphere
{
public:
	Classifier centre;
	double rad;
	int pointNum;
	Sphere(const Classifier & c, double r)
	{
		rad = r;
		centre = c;
		rad = r;
		pointNum = 1;
	}
	Sphere(){}
};

class NeuralNetwork
{
private:
	QMap<QString, Sphere> mSphereObject;
	double minR;

public:
	NeuralNetwork()
	{
		minR = 1;
	}
	void learn(QString const & object, QList<QPoint> const & path)
	{
		Classifier newPoint(path);
		qDebug() << "classifier built";
		if (!mSphereObject.keys().contains(object))
		{
			Sphere sphere(newPoint, minR);
			mSphereObject.insert(object, sphere);
			qDebug() << "sphere created";
			return;
		}
		Sphere sphere = mSphereObject[object];
		int pointNum = sphere.pointNum + 1;
		sphere.centre = sphere.centre - (sphere.centre - newPoint) / pointNum;
		Classifier diff = newPoint - sphere.centre;
		sphere.rad = std::max(diff.norm() * (pointNum - 1) / pointNum,
							  sphere.rad + diff.norm() / pointNum);
		mSphereObject[object] = sphere;
	}
        QString recognize(const QList<QPoint> & path)
	{
		//qDebug() << "path" << Adopter::pathToString(path);
		Classifier point(path);
                QString object = "";
		double min = 1000000;
		foreach (QString element, mSphereObject.keys())
		{
			Sphere sphere = mSphereObject[element];
			Classifier diff = point - sphere.centre;
			double norm = diff.norm();
			//qDebug() << norm;
			if (norm < min && norm < sphere.rad)
			{
				min = norm;
                                object = element;
			}
		}
                return object;
	}

	void clear()
	{
		mSphereObject.clear();
	}

	void setMinRad(double r)
	{
		minR = r;
		clear();
	}
};
