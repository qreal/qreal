/** @file uml_nodeelement.h
 * 	@brief Класс, представляющий объект на диаграмме
 * */

#pragma once

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtGui/QWidget>

#include "embeddedLinker.h"
#include "sdfrenderer.h"
#include "uml_element.h"
#include "uml_edgeelement.h"
#include "elementImpl.h"

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
		NodeElement(ElementImpl *impl);

		/** @brief Деструктор */
		virtual ~NodeElement();

		/** @brief Отрисовать объект */
		virtual void paint(QPainter *p, /**< Объект, осуществляющий отрисовку элементов */
			const QStyleOptionGraphicsItem *opt, /**< Настройки отрисовки */
			QWidget *w, /**< Виджет, на котором осуществляется отрисовка */
			SdfRenderer *portrenderer /**< Рендерер портов)*/);
		virtual void paint(QPainter *, /**< Объект, осуществляющий отрисовку элементов */
			const QStyleOptionGraphicsItem *, /**< Настройки отрисовки */
			QWidget * /**< Виджет, на котором осуществляется отрисовка */);

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
		virtual void setName(QString name);

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

	private:
		enum { OBJECT_MIN_SIZE = 10, SIZE_OF_FORESTALLING = 25 };

		//события мыши

		/** @brief Обработать событие нажатия кнопки мыши */
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );

		/** @brief Обработать событие перемещения мыши во время нажатия */
		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

		/** @brief Обработать событие отпускания кнопки мыши */
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

		//события наведения мыши

		/** @brief Обработать событие наведения мыши */
		virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );

		/** @brief Обработать событие перемещения мыши над элементом */
		virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

		/** @brief Обработать событие покидания области элемента мышью */
		virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );

		/** @brief Обработать изменение данных объекта
			 *	@brief @return Измененные данные
			 * */
		virtual QVariant itemChange(GraphicsItemChange change, /**< Тип изменений */
			const QVariant &value /**< Величина изменения */);

		bool mPortsVisible;

		/** @brief Список точечных портов */
		QList<QPointF> mPointPorts;
		/** @brief Список портов-линий */
		QList<StatLine> mLinePorts;
		/** @brief Область, в которой возможно отображение текста, параметризующего SVG */
		QRectF mContents;

		/** @brief Направление растяжения элемента */
		enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };

		/** @brief Получить объект, расположенный в данной точке сцены
			*	@brief @return Объект, расположенный в данной точке сцены
			* */
		NodeElement *getNodeAt( const QPointF &position /**< Точка на сцене */);

		void adjustLinks();

		QLineF newTransform(const StatLine& port)  const;

		void resize(QRectF newContents);
		void moveChildren(qreal dx, qreal dy);
		void moveChildren(QPointF);

		qreal minDistanceFromLinePort(int linePortNumber, const QPointF &location) const;
		qreal distanceFromPointPort(int pointPortNumber, const QPointF &location) const;
		qreal getNearestPointOfLinePort(int linePortNumber, const QPointF &location) const;

		/** @brief Список ассоциированных с объектом связей */
		QList<EdgeElement *> mEdgeList;

		/** @brief Направление растяжения */
		DragState mDragState;

		/** @brief EmbeddedLinker */
		EmbeddedLinker *mEmbeddedLinker;

		/** @brief Описание двухмерной трансформации объекта */
		QTransform mTransform;

		ElementImpl* mElementImpl;

		SdfRenderer *mPortRenderer;
	};

}

