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

	qreal getMaxDistance(const QString &);
	bool isMultistroke();
	qreal getDistance(QPair<qreal *, qreal *> const &key1, QPair<qreal *, qreal *> const &key2);
	QPair<qreal *, qreal *> getKey(PathVector const &path);
};

class MixedClassifier
{
public:
	MixedClassifier();
	explicit MixedClassifier(PathVector const &path);
	~MixedClassifier();

	qreal getDistance(MixedClassifier const &classifier);
	MixedClassifier getPoint(MixedClassifier const &centre, qreal centreWeight);
	QPair<qreal *, qreal *> key() const;

private:
	explicit MixedClassifier(QPair<qreal *, qreal *> const & key);

	QPair<qreal *, qreal *> mKey;
};

}
}
