#include "math.h"

int mathUtils::Math::min(int x, int y)
{
	return x > y ? y : x;
}

qreal mathUtils::Math::min(qreal x, qreal y)
{
	return x > y ? y : x;
}

int mathUtils::Math::max(int x, int y)
{
	return x < y ? y : x;
}

qreal mathUtils::Math::max(qreal x, qreal y)
{
	return x < y ? y : x;
}

int mathUtils::Math::sign(qreal x)
{
	return x > EPS ? 1 : (x < -EPS ? -1 : 0);
}

int mathUtils::Math::sqr(int x)
{
	return x * x;
}

qreal mathUtils::Math::sqr(qreal x)
{
	return x * x;
}
