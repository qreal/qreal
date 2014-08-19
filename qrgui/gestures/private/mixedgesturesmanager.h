#pragma once

#include "abstractRecognizer.h"
#include "geometricForms.h"

namespace qReal {
namespace gestures {

class MixedGesturesManager : public GesturesRecognizer<QPair<qreal *, qreal *> >
{
public:
	MixedGesturesManager();
	~MixedGesturesManager();
	qreal getMaxDistance(QString const &);
	bool isMultistroke();
	qreal getDistance(QPair<qreal *, qreal *> const &key1, QPair<qreal *, qreal *> const &key2);
	QPair<qreal *, qreal *> getKey(PathVector const &path);
};

class MixedClassifier
{
public:
	MixedClassifier(const PathVector &path)
	{
		MixedGesturesManager gManager;
		mKey = gManager.getKey(path);
	}

	MixedClassifier(){}

	~MixedClassifier();

	qreal getDistance(const MixedClassifier &classifier)
	{
		QPair<double *, double *> key = classifier.key();
		MixedGesturesManager gManager;
		return gManager.getDistance(key, mKey);
	}

	MixedClassifier getPoint(MixedClassifier const &centre, double centreWeight)
	{
		double * key1 = centre.key().first;
		double * key2 = centre.key().second;
		double * finalKey1 = new double[gridSize * gridSize];
		double * finalKey2 = new double[gridSize * gridSize];
		for (int i = 0; i < gridSize * gridSize; i ++) {
			finalKey1[i] = (key1[i] * centreWeight + mKey.first[i]) / (centreWeight + 1);
			finalKey2[i] = (key2[i] * centreWeight + mKey.second[i]) / (centreWeight + 1);
		}
		return MixedClassifier(QPair<double *, double *>(finalKey1, finalKey2));
	}

	QPair<double *, double *> key() const
	{
		return mKey;
	}

private:
	MixedClassifier(QPair<double *, double *> const & key)
	{
		mKey = key;
	}
	QPair<double *, double *> mKey;
};

}
}
