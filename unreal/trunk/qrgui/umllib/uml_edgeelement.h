/** @file edgeelement.h
 * 	@brief Класс, представляющий связь на диаграмме
 * */
#pragma once

#include "uml_element.h"

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

namespace UML {
	/** @brief Тип стрелки */
	enum ArrowType { FILLED_ARROW, EMPTY_ARROW, FILLED_RHOMB, EMPTY_RHOMB, NO_ARROW, OPEN_ARROW };

	class NodeElement;
	/** @class EdgeElement
	 * 	@brief Класс, представляющий связь на диаграмме
	 * 	*/
	class EdgeElement : public Element
	{
		public:
			/** @brief Конструктор */
			EdgeElement();
			/** @brief Деструктор */
			virtual ~EdgeElement();

			/** @brief Обновить данные связи */
			void updateData();

			/** @brief Получить область, в рамках которой производится отрисовка связи
			 *	@brief @return Область, в рамках которой производится отрисовка связи
			 * */
			virtual QRectF boundingRect() const;
			/** @brief Получить ломаную для отрисовки связи
			 *	@brief @return Ломаня для отрисовки связи
			 * */
			QPainterPath shape() const;
			/** @brief Отрисовать связь */
			virtual void paint(QPainter* p, /**< Объект, осуществляющий отрисовку элементов */
						const QStyleOptionGraphicsItem* opt, /**< Настройки отрисовки */
						QWidget* w /**< Виджет, на котором осуществляется отрисовка */
						);

			/** @brief Перерисовать связь */
			void adjustLink();
			/** @brief Отсоединить связь от объекта */
			void removeLink(UML::NodeElement const *from /**< Объект */);

			virtual void connectToPort();

		protected:
			/** @brief Обработать нажатие кнопки мыши */
			virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
			/** @brief Обработать движение курсора мыши */
			virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
			/** @brief Обработать отпускание кнопки мыши */
			virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
			/** @brief Обработать открытие контекстного меню */
			virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

			/** @brief Отрисовать начало связи */
			virtual void drawStartArrow(QPainter *p /**< Объект, осуществляющий отрисовку элементов */) const = 0;
			/** @brief Отрисовать конец связи */
			virtual void drawEndArrow(QPainter *p /**< Объект, осуществляющий отрисовку элементов */) const = 0;

			/** @brief Стиль линии */
			Qt::PenStyle mPenStyle;
			/** @brief Текст над линией */
			QString mText;
			QString mFromMult, mToMult;
			/** @brief Тип стрелки начала связи */
			ArrowType mStartArrowStyle;
			/** @brief Тип стрелки конца связи */
			ArrowType mEndArrowStyle;

		private:

			/** @brief Получить точку на ломаной
			 * */
			int getPoint(const QPointF &location /**< Расположение точки */);
			/** @brief Получить объект, расположенный в данной точке сцены
			 * */
			NodeElement *getNodeAt(const QPointF &position /**< Точка на сцене */);
			/** @brief Обновить номер самой длинной части ломаной */
			void updateLongestPart();
			/** @brief Получить прямоугольник порта вокруг заданной точки */
			static QRectF getPortRect(QPointF const &point);

			void drawPort(QPainter *painter) const;

			/** @brief Объект, присоединенный к началу связи */
			NodeElement *mSrc;
			/** @brief Объект, присоединенный к концу связи */
			NodeElement *mDst;

			/** @brief Идентификатор порта объекта, к которому присоединено начало связи */
			qreal mPortFrom;
			/** @brief Идентификатор порта объекта, к которому присоединен конец связи */
			qreal mPortTo;
			/** @brief Состояние перемещения */
			int mDragState;

			/** @brief Номер самой длинной части ломаной */
			int mLongPart;

			/** @brief Массив точек для отрисовки связи */
			QPolygonF mLine;
			/** @brief Цвет линии */
			QColor mColor;

			NodeElement *mBeginning;
			NodeElement *mEnding;

			bool mChaoticEdition;
	};
}
