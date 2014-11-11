#pragma once

#include <QtGui/QPainter>

#include "qrutils/utilsDeclSpec.h"

namespace graphicsUtils {

class QRUTILS_EXPORT PainterInterface
{
public:
	virtual void paint(QPainter *painter) = 0;
	virtual void clear() = 0;
};

}
