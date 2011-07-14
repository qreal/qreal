#pragma once

#include "controlsDisposer.h"

class TableDisposer : public ControlsDisposer
{
	public:
		TableDisposer();
		void dispose(Element *element, QList<Item> &items, QRectF &boundingRectToComputate) const;
	private:
		void    disposeOne(QList<Item> &items, QRectF &rect, qreal &curX, qreal &curY, int &i) const;
		QRectF  disposeInColumns(QList<Item> &items, const int columns, const qreal length,
						const qreal middleX, const qreal top) const;
};
