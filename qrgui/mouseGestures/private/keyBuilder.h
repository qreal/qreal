#pragma once

#include "geometricForms.h"
#include "pathCorrector.h"

namespace qReal {
namespace gestures {

class KeyBuilder
{
public:
	static Key getKey(const PathVector &mousePath, int heightSize, int widthSize);

private:
	static void rasterizeSegment(const SquarePos &pos1, const SquarePos &pos2, Key *segment);
};

}
}
