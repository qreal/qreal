#pragma once

#include <QtCore/QtGlobal>

namespace mathUtils {

class Math
{
public:
	static const qreal EPS = 0.000000001;

	static bool eq(qreal x, qreal y);

	static int sqr(int x);
	static qreal sqr(qreal x);

	static int sign(qreal x);
};

}
