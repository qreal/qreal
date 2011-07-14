#pragma once

#include "../element.h"
#include "embeddedControls.h"

class Item;
class ControlsDisposer
{
	public: virtual void dispose(Element *element, QList<Item> &items, QRectF &boundingRectToComputate) const = 0;
};
