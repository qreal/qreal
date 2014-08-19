#pragma once

#include "geometricForms.h"
#include "pathCorrector.h"

namespace qReal {
namespace gestures {

class KeyBuilder
{
public:
	static Key getKey(PathVector const &mousePath, int heightSize, int widthSize);

private:
	static void rasterizeSegment(SquarePos const &pos1, SquarePos const &pos2, Key *segment);
};

}
}
