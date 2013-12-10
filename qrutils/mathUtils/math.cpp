#include "math.h"

const qreal EPS = 0.00001;

qreal mathUtils::Math::eps()
{
	return EPS;
}

int mathUtils::Math::sign(qreal x)
{
	return x > EPS ? 1 : (x < -EPS ? -1 : 0);
}

bool mathUtils::Math::eq(qreal x, qreal y)
{
	return eq(x, y, EPS);
}

bool mathUtils::Math::eq(qreal x, qreal y, qreal eps)
{
	return qAbs(x - y) < eps;
}

bool mathUtils::Math::leq(qreal x, qreal y)
{
	return x < y || eq(x, y);
}

bool mathUtils::Math::geq(qreal x, qreal y)
{
	return x > y || eq(x, y);
}

int mathUtils::Math::sqr(int x)
{
	return x * x;
}

qreal mathUtils::Math::sqr(qreal x)
{
	return x * x;
}

bool mathUtils::Math::between(qreal border1, qreal border2, qreal value)
{
	return (leq(border1, value) && leq(value, border2)) || (geq(border1, value) && geq(value, border2));
}
