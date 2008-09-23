/** @file uml_nodeelement.h
 * 	@brief Класс, представляющий объект на диаграмме
 * */

#ifndef UML_NODEELEMENT_H
#define UML_NODEELEMENT_H

#include "uml_element.h"
#include "uml_edgeelement.h"

#include <QtGui/QWidget>

/** @brief Размер порта объекта */
const int kvadratik = 5;

namespace UML {
	/** @class NodeElement
 	* 	@brief Класс, представляющий объект на диаграмме
	 * */
	class NodeElement : public Element
	{
		public:
			/** @brief Конструктор */
			NodeElement();
			/** @brief Деструктор */
			~NodeElement();

			/** @brief Отрисовать объект */
			virtual void paint(QPainter* p, /**< Объект, осуществляющий отрисовку элементов */
								const QStyleOptionGraphicsItem* opt, /**< Настройки отрисовки */
								QWidget* w /**< Виджет, на котором осуществляется отрисовка */
								);
			/** @brief Получить область, в рамках которой осуществляется отрисовка объекта
			 *	@brief @return Область, в рамках которой осуществляется отрисовка объекта 
			 * */
			QRectF boundingRect() const;
			/** @brief Получить область, в рамках которой возможна параметризация статического SVG
			 *	@brief @return Область, в рамках которой возможна параметризация статического SVG
			 * */
			QRectF contentsRect() const;

			/** @brief Обновить данные элемента */
			virtual void updateData();

			/** @brief Получить расположение порта
			 *	@brief @return Координаты порта
			 * */
			const QPointF getPortPos(qreal id /**< Идентификатор порта */) const;
			/** @brief Получить идентификатор порта
			 *	@brief @return Идентификатор порта
			 * */
			qreal getPortId(const QPointF &location /**< Расположение порта */) const;
			/** @brief Узнать, является ли этот объект дочерним для данного
			 * 	@brief @return Является ли этот объект дочерним для данного
			 * */
			bool isChildOf(int id /**< Идентификатор объекта */)
			{ 
				return parentsList.contains(id); 
			}

			/** @brief Добавить связь */
			void addEdge(EdgeElement *edge /**< Связь */) 
			{ 
				edgeList << edge; 
			};
			/** @brief Убрать связь */
			void delEdge(EdgeElement *edge /**< Связь */) 
			{ 
				edgeList.removeAt(edgeList.indexOf(edge)); 
			};

		protected:
			/** @brief Обработать событие наведения на объект курсора мыши */
			void mouseMoveEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
			/** @brief Обработать событие отпускания кнопки мыши */
			void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
			/** @brief Обработать событие нажатия кнопки мыши */
			void mousePressEvent( QGraphicsSceneMouseEvent * event /**< Событие */);

			/** @brief Обработать изменение данных объекта
			 *	@brief @return Измененные данные
			 * */
			virtual QVariant itemChange(GraphicsItemChange change, /**< Тип изменений */
											const QVariant &value /**< Величина изменения */
											);

			/** @brief Список родительских объектов */
			QList<int> parentsList;

			/** @brief Список точечных портов */
			QList<QPointF> pointPorts;
			/** @brief Список портов-линий */
			QList<QLineF> linePorts;
			
			/** @brief Область, в которой возможно отображение текста, параметризующего SVG */
			QRectF m_contents;
		private:
			/** @brief Список ассоциированных с объектом связей */
			QList<EdgeElement *> edgeList;

			/** @brief Направление растяжения элемента */
			enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
			/** @brief Направление растяжения */
			DragState dragState;

			/** @brief Описание двухмерной трансформации объекта */
			QTransform transform;
	};
};

#endif
