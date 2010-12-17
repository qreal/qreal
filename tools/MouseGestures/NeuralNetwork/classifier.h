#pragma once
#include <QList>
#include <QPoint>

const int size = 8;

class Classifier
{
public:
	Classifier(const QList<QPoint> & path);
	Classifier(){}
	double norm();
	Classifier operator - (Classifier const & other);
	Classifier operator / (double a);

private:
	double mPoint[size * size * 2];
	Classifier(double point[size * size * 2]);
	int leftBound(QList<QPoint> const & path);
	int rightBound(QList<QPoint> const & path);
	int upperBound(QList<QPoint> const & path);
	int lowerBound(QList<QPoint> const & path);
};
