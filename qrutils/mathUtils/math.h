#pragma once

#include <QtCore/QtGlobal>

#include <qrutils/utilsDeclSpec.h>

namespace mathUtils {

// Default precision for all floating point numbers comparison methods
qreal const EPS = 0.0000000001;

// There's no such thing in standard math.h in C++11
qreal const pi = 3.14159265358979323846;

/// Contains useful methods for using in some mathematical expressions
class QRUTILS_EXPORT Math
{
public:
	/// Compares given numbers with given precision
	static bool eq(qreal x, qreal y, qreal eps = EPS);

	/// Returns if x less or equal to y with given precision
	static bool leq(qreal x, qreal y, qreal eps = EPS);

	/// Returns if x greater or equal to y
	static bool geq(qreal x, qreal y, qreal eps = EPS);

	/// Returns the square of x (x * x)
	static int sqr(int x);

	/// Returns the square of x (x * x)
	static qreal sqr(qreal x);

	/// Returns the sign of x (-1, 0 or 1)
	static int sign(qreal x, qreal eps = EPS);

	/// Returns if border1 >= value >= border2 or border1 <= value <= border2
	static bool between(qreal border1, qreal border2, qreal value, qreal eps = EPS);

	/// Returns value if border1 <= value <= border2 or the closest border in other case.
	static int truncateToInterval(int border1, int border2, int value);
};

}
