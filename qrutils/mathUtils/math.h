#pragma once

#include <QtCore/QtGlobal>

namespace mathUtils {

class Math
{
public:
	static const qreal EPS = 0.00001;

	/// Compares given numbers with default precision
	static bool eq(qreal x, qreal y);

	/// Compares given numbers with given precision
	static bool eq(qreal x, qreal y, qreal eps);

	/// Returns if x less or equal to y
	static bool leq(qreal x, qreal y);

	/// Returns if x greater or equal to y
	static bool geq(qreal x, qreal y);

	static int sqr(int x);
	static qreal sqr(qreal x);

	static int sign(qreal x);

	/// Returns if border1 >= value >= border2 or border1 <= value <= border2
	static bool between(qreal border1, qreal border2, qreal value);
};

}
