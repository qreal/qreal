#include "math.h"

#include <QtCore/qmath.h>

#include <qrkernel/settingsManager.h>

using namespace mathUtils;

int Math::sign(qreal x, qreal eps)
{
	return x > eps ? 1 : (x < -eps? -1 : 0);
}

bool Math::eq(qreal x, qreal y, qreal eps)
{
	return qAbs(x - y) < eps;
}

bool Math::leq(qreal x, qreal y, qreal eps)
{
	return x < y || eq(x, y, eps);
}

bool Math::geq(qreal x, qreal y, qreal eps)
{
	return x > y || eq(x, y, eps);
}

int Math::sqr(int x)
{
	return x * x;
}

qreal Math::sqr(qreal x)
{
	return x * x;
}

bool Math::between(qreal border1, qreal border2, qreal value, qreal eps)
{
	return (leq(border1, value, eps) && leq(value, border2, eps))
			|| (geq(border1, value, eps) && geq(value, border2, eps));
}

int Math::truncateToInterval(int border1, int border2, int value)
{
	return (value>= border1 && value <= border2) ? value : (value < border1 ? border1 : border2);
}

qreal Math::gaussianNoise(qreal variance)
{
	qreal const mu = 0.5;
	qreal const var = 0.083; // 1/12

	const int approximationLevel = qReal::SettingsManager::value("approximationLevel", 12).toInt();

	qreal result = 0.0;
	for (int i = 0; i < approximationLevel; ++i) {
		result += static_cast<qreal>(qrand()) / (static_cast<unsigned int>(RAND_MAX) + 1);
	}

	result -= approximationLevel * mu;
	result *= qSqrt(variance / (approximationLevel * var));

	return result;
}
