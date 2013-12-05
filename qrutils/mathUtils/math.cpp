#include "math.h"

int mathUtils::Math::sign(qreal x, qreal eps)
{
	return x > eps ? 1 : (x < -eps? -1 : 0);
}

bool mathUtils::Math::eq(qreal x, qreal y, qreal eps)
{
	return qAbs(x - y) < eps;
}

bool mathUtils::Math::leq(qreal x, qreal y, qreal eps)
{
	return x < y || eq(x, y, eps);
}

bool mathUtils::Math::geq(qreal x, qreal y, qreal eps)
{
	return x > y || eq(x, y, eps);
}

int mathUtils::Math::sqr(int x)
{
	return x * x;
}

qreal mathUtils::Math::sqr(qreal x)
{
	return x * x;
}

bool mathUtils::Math::between(qreal border1, qreal border2, qreal value, qreal eps)
{
	return (leq(border1, value, eps) && leq(value, border2, eps))
			|| (geq(border1, value, eps) && geq(value, border2, eps));
}
