#pragma once

#include "view/gestures/abstractRecognizer.h"
#include "view/gestures/geometricForms.h"

namespace qReal {
namespace gestures {

class RectangleGesturesManager : public GesturesRecognizer<double *>
{
public:
	RectangleGesturesManager();
	~RectangleGesturesManager();
	double getMaxDistance(QString const &);
	bool isMultistroke();
	double getDistance(double * const &key1, double * const &key2);
	double *getKey(PathVector const & path);
};

}
}
