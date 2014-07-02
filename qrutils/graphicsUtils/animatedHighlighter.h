#pragma once

#include <QtWidgets/QGraphicsItem>

#include "qrutils/utilsDeclSpec.h"

namespace graphicsUtils {

/// Highlights the given item animating its opacity.
class QRUTILS_EXPORT AnimatedHighlighter
{
public:
	/// Highlights the given item animating its opacity.
	static void highlight(QGraphicsItem * const item);
};

}
