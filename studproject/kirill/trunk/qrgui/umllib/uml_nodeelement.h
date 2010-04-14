/** @file uml_nodeelement.h
 * 	@brief Класс, представляющий объект на диаграмме
 * */

#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QtGui/QWidget>

#include "uml_element.h"
#include "uml_edgeelement.h"
#include "sdfrenderer.h"

/** @brief Размер порта объекта */
const int kvadratik = 5;

namespace UML {
	/** @class NodeElement
	* 	@brief Класс, представляющий объект на диаграмме
	 * */
	class NodeElement : public Element
	{
		Q_OBJECT

	public:
		/** @brief Конструктор */
		NodeElement();

		/** @brief Деструктор */
		virtual ~NodeElement();

		/** @brief Отрисовать объект */
		virtual void paint(QPainter *p, /**< Объект, осуществляющий отрисовку элементов */
			const QStyleOptionGraphicsItem *opt, /**< Настройки отрисовки */
			QWidget *w, /**< Виджет, на котором осуществляется отрисовка */
			SdfRenderer *portrenderer /**< Рендерер портов)*/);

		/** @brief Получить область, в рамках которой осуществляется отрисовка объекта
			 *	@brief @return Область, в рамках которой осуществляется отрисовка объекта
			 * */
		QRectF boundingRect() const;

		/** @brief Получить область, в рамках которой возможна параметризация статического SVG
			 *	@brief @return Область, в рамках которой возможна параметризация статического SVG
			 * */
		QRectF contentsRect() const;

		/** @brief Обновить данные элемента из модели */
		virtual void updateData();
		/** @brief Установить новые размеры и позицию (не сохраняя в модель) */
		void setGeometry(QRectF const &geom);
		/** @brief Сохранить текущие размеры и позицию в модель */
		void storeGeometry();
		/** @brief Установить новое имя и сохранить его в модель */
		void setName(QString name);

		/** @brief Получить расположение порта
			 *	@brief @return Координаты порта
			 * */
		const QPointF getPortPos(qreal id /**< Идентификатор порта */) const;

		/** @brief Получить идентификатор порта
			 *	@brief @return Идентификатор порта
			 * */
		qreal getPortId(const QPointF &location /**< Расположение порта */) const;

		/** @brief Добавить связь */
		void addEdge(EdgeElement *edge);

		/** @brief Убрать связь */
		void delEdge(EdgeElement *edge);

		void setPortsVisible(bool value);

	protected:
		/** @brief Описание линейного порта, реагирующего на абсолютные координаты */
		struct StatLine
		{
			QLineF line;
			bool prop_x1;
			bool prop_y1;
			bool prop_x2;
			bool prop_y2;

			StatLine() : line(QLineF(0, 0, 0, 0)), prop_x1(false), prop_y1(false),
				prop_x2(false), prop_y2(false) {}

			operator QLineF () const
			{
				return line;
			}

			void operator = (QLineF const &l)
			{
				line = l;
				prop_x1 = false;
				prop_x2 = false;
				prop_y1 = false;
				prop_y2 = false;
			}
		};

		/** @brief Обработать событие наведения на объект курсора мыши */
		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать событие отпускания кнопки мыши */
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать событие нажатия кнопки мыши */
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать изменение данных объекта
			 *	@brief @return Измененные данные
			 * */
		virtual QVariant itemChange(GraphicsItemChange change, /**< Тип изменений */
									const QVariant &value /**< Величина изменения */
									);

		bool mPortsVisible;

		/** @brief Список точечных портов */
		QList<QPointF> mPointPorts;
		/** @brief Список портов-линий */
		QList<StatLine> mLinePorts;
		/** @brief Область, в которой возможно отображение текста, параметризующего SVG */
		QRectF mContents;

 private:
		/** @brief Направление растяжения элемента */
		enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };

		/** @brief Получить объект, расположенный в данной точке сцены
			*	@brief @return Объект, расположенный в данной точке сцены
			* */
		NodeElement *getNodeAt( const QPointF &position /**< Точка на сцене */);

		void adjustLinks();

		QLineF newTransform(const StatLine& port)  const;

		qreal minDistanceFromLinePort(int linePortNumber, const QPointF &location) const;
		qreal distanceFromPointPort(int pointPortNumber, const QPointF &location) const;
		qreal getNearestPointOfLinePort(int linePortNumber, const QPointF &location) const;

		/** @brief Список ассоциированных с объектом связей */
		QList<EdgeElement *> mEdgeList;

		/** @brief Направление растяжения */
		DragState mDragState;

		/** @brief Описание двухмерной трансформации объекта */
		QTransform mTransform;
	};

}
