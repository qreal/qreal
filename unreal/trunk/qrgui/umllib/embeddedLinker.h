#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "../kernel/definitions.h"
#include "uml_edgeelement.h"

namespace UML {
	class NodeElement;
	/** @class EmbeddedLinker
	* 	@brief Небольшой объект рядом с элементом, позволяющий быстро создавать связи.
	 * */
	class EmbeddedLinker : public QObject, public QGraphicsItem
	{
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

	public:
		/** @brief Конструктор */
		EmbeddedLinker();
		/** @brief Конструктор */
		EmbeddedLinker(QGraphicsItem *parent);
		/** @brief Деструктор */
		virtual ~EmbeddedLinker();

		/** @brief Установить элемент, управляющий объектом */
		void setMaster(NodeElement *element);
		/** @brief Сообщить, что NodeElement под указателем мыши */
		void setCovered(bool arg);

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
		bool covered;
		/** @brief Элемент, управляющий объектом */
		NodeElement *master;
		/** @brief Текущая связь */
		EdgeElement *mEdge;
		/** @brief Прямоугольник, ограничивающий объект */
		QRectF mRectangle;
		QRectF mInnerRectangle;
	signals:
		void coveredChanged();
	public slots:
		void changeShowState();
	};
}

