#pragma once

#include "controlsDisposer.h"

#include <QGraphicsRectItem>

class TableDisposer : public ControlsDisposer
{
	public:
		TableDisposer();
		bool isFolded(Element *element) const;
		void switchFolding(const bool request, Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const;
	protected:
		void fold(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const;
		void dispose(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const;
	private:
		void    disposeOne(QList<Item> &items, QRectF &rect, qreal &curX, qreal &curY, int &i) const;
		QRectF  disposeInColumns(QList<Item> &items, const int columns, const qreal length,
						const qreal middleX, const qreal top) const;

		static void initFoldedGlobalState(Element *element);
		static void initSwitchesGlobalState(Element *element, QList<Item> &items, QRectF &boundingRectToCompute);
		static QMap<Element*, QGraphicsItem*> switches;
		static QMap<Element*, bool> folded;
};
