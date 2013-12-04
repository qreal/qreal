#pragma once

#include <QtCore/QtGlobal>

#include <qrutils/utilsDeclSpec.h>

namespace mathUtils {

/// Contains useful methods for using in some mathematical expressions
class QRUTILS_EXPORT Math
{
public:
	/// Default precision for all
	static qreal const QRUTILS_EXPORT EPS = 0.00001;

	/// Compares given numbers with default precision
	static bool eq(qreal x, qreal y);

	/// Compares given numbers with given precision
	static bool eq(qreal x, qreal y, qreal eps);

	/// Returns if x less or equal to y
	static bool leq(qreal x, qreal y);

	/// Returns if x greater or equal to y
	static bool geq(qreal x, qreal y);

	/// Returns the square of x (x * x)
	static int sqr(int x);

	/// Returns the square of x (x * x)
	static qreal sqr(qreal x);

	/// Returns the sign of x (-1, 0 or 1)
	static int sign(qreal x);

	/// Returns if border1 >= value >= border2 or border1 <= value <= border2
	static bool between(qreal border1, qreal border2, qreal value);
};

}
