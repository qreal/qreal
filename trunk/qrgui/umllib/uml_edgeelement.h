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
			~EdgeElement();

			/** @brief Обновить данные связи */
			void updateData();

			/** @brief Получить область, в рамках которой производится отрисовка связи
			 *	@brief @return Область, в рамках которой производится отрисовка связи
			 * */
			QRectF boundingRect() const;
			/** @brief Получить ломаную для отрисовки связи
			 *	@brief @return Ломаня для отрисовки связи
			 * */
			QPainterPath shape() const;
			/** @brief Отрисовать связь */
			void paint(QPainter* p, /**< Объект, осуществляющий отрисовку элементов */
						const QStyleOptionGraphicsItem* opt, /**< Настройки отрисовки */
						QWidget* w /**< Виджет, на котором осуществляется отрисовка */
						);

			/** @brief Перерисовать связь */
			void adjustLink();
			/** @brief Отсоединить связь от объекта */
			void removeLink(UML::NodeElement *from /**< Объект */)
			{
				if (src == from)
				{
					src = NULL;
				};
				if (dst == from)
				{
					dst = NULL;
				};
			}

			virtual void connectToPort();

		private:

			NodeElement *beginning;
			NodeElement *ending;

			/** @brief Получить точку на ломаной
			 *	@brief @return Точка на ломаной
			 * */
			int getPoint( const QPointF &location /**< Расположение точки */ );
			/** @brief Получить объект, расположенный в данной точке сцены
			 *	@brief @return Объект, расположенный в данной точке сцены
			 * */
			NodeElement *getNodeAt( const QPointF &position /**< Точка на сцене */);
//			void checkConnection();
			/** @brief Объект, присоединенный к началу связи */
			NodeElement *src;
			/** @brief Объект, присоединенный к концу связи */
			NodeElement *dst;
			//	QPointF srcPoint, dstPoint;

			/** @brief Идентификатор порта объекта, к которому присоединено начало связи */
			qreal portFrom;
			/** @brief Идентификатор порта объекта, к которому присоединен конец связи */
			qreal portTo;
			/** @brief Состояние перемещения */
			int dragState;

			/** @brief Номер самой длинной части ломаной */
			int longPart;

			/** @brief Массив точек для отрисовки связи */
			QPolygonF m_line;
			/** @brief Цвет линии */
			QColor m_color;

			/** @brief Обновить номер самой длинной части ломаной */
			void updateLongestPart();
		protected:
			/** @brief Обработать нажатие кнопки мыши */
			void mousePressEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
			/** @brief Обработать движение курсора мыши */
			void mouseMoveEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
			/** @brief Обработать отпускание кнопки мыши */
			void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
			/** @brief Обработать открытие контекстного меню */
			void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event /**< Событие */);

			/** @brief Отрисовать начало связи */
			virtual void drawStartArrow ( QPainter * p /**< Объект, осуществляющий отрисовку элементов */) const = 0;
			/** @brief Отрисовать конец связи */
			virtual void drawEndArrow ( QPainter * p /**< Объект, осуществляющий отрисовку элементов */) const = 0;

			/** @brief Стиль линии */
			Qt::PenStyle m_penStyle;
			/** @brief Текст над линией */
			QString m_text;
			QString m_fromMult, m_toMult;
			/** @brief Тип стрелки начала связи */
			ArrowType m_startArrowStyle;
			/** @brief Тип стрелки конца связи */
			ArrowType m_endArrowStyle;
	};
}
