#pragma once

#include "abstractRecognizer.h"
#include "geometricForms.h"

namespace qReal {
namespace gestures {

class NearestPosGridGesturesManager : public GesturesRecognizer<qreal *>
{
public:
	NearestPosGridGesturesManager();
	~NearestPosGridGesturesManager();
	qreal getMaxDistance(QString const &);
	bool isMultistroke();
	qreal getDistance(qreal * const & key1, qreal * const &key2);
	qreal *getKey(PathVector const &path);
};

}
}
