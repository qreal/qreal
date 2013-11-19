#pragma once

#include <QtCore/QtGlobal>

namespace mathUtils {

class Math
{
public:
	static const qreal EPS = 0.000000001;

	static int sqr(int x);
	static qreal sqr(qreal x);

	static int min(int x, int y);
	static qreal min(qreal x, qreal y);

	static int max(int x, int y);
	static qreal max(qreal x, qreal y);

	static int sign(qreal x);
};

}
