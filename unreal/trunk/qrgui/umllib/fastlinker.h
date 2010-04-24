#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "../kernel/definitions.h"
#include "uml_edgeelement.h"

namespace UML {
	class NodeElement;
	/** @class FastLinker
	* 	@brief Небольшой объект рядом с элементом, позволяющий быстро создавать связи.
	 * */
	class FastLinker : public QObject, public QGraphicsItem
	{
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

	public:
		/** @brief Конструктор */
		FastLinker();
		/** @brief Конструктор */
		FastLinker(QGraphicsItem *parent);
		/** @brief Деструктор */
		virtual ~FastLinker();

		/** @brief Установить элемент, управляющий объектом */
		void setMaster(NodeElement *element);

		/** @brief Отрисовать объект */
		virtual void paint(QPainter *p, /**< Объект, осуществляющий отрисовку элементов */
			const QStyleOptionGraphicsItem *opt, /**< Настройки отрисовки */
			QWidget *w /**< Виджет, на котором осуществляется отрисовка */);
		/** @brief Метод перемещения объекта, автоматически помещает объект вне рамок элемента, к которому он прикрепен */
		virtual void moveTo(QPointF pos);
		/** @brief Ограничивающий прямоугольник */
		QRectF boundingRect() const;


		/** @brief Обработать событие наведения на объект курсора мыши */
		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать событие отпускания кнопки мыши */
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event);

		/** @brief Обработать событие нажатия кнопки мыши */
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event);
	private:
		/** @brief Элемент, управляющий объектом */
		NodeElement *master;
		/** @brief Текущая связь */
		EdgeElement *mEdge;
		/** @brief Прямоугольник, ограничивающий объект */
		QRectF mRectangle;
		QRectF mInnerRectangle;

	};
}

