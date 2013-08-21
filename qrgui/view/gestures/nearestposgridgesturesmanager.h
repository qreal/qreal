#pragma once

#include "abstractRecognizer.h"
#include "geometricForms.h"

namespace qReal {
namespace gestures {

class NearestPosGridGesturesManager : public GesturesRecognizer<double *>
{
public:
	NearestPosGridGesturesManager();
	~NearestPosGridGesturesManager();
	double getMaxDistance(QString const &);
	bool isMultistroke();
	double getDistance(double * const & key1, double * const &key2);
	double *getKey(PathVector const &path);
};

}
}
