#pragma once

#include "../../element.h"
#include "embeddedControls.h"

class Item;
class ControlsDisposer
{
	public:
		virtual bool isFolded(Element *element) const = 0;
		virtual void switchFolding(const bool request, Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const = 0;
	protected:
		virtual void fold(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const = 0;
		virtual void dispose(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const = 0;
};
