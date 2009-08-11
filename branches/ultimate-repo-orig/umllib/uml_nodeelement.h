/** @file uml_nodeelement.h
 * 	@brief Класс, представляющий объект на диаграмме

 * */

#pragma once

#include "uml_element.h"
#include "uml_edgeelement.h"
#include "sdfrenderer.h"
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsProxyWidget>

#include <QtGui/QWidget>

/** @brief Размер порта объекта */

const int kvadratik = 5;

namespace UML {
	/** @class NodeElement
	* 	@brief Класс, представляющий объект на диаграмме

	 * */
	class statLine;

	class ElementTitle : public QGraphicsTextItem
	{
		Q_OBJECT
		QString oldText;
	public:
		ElementTitle() {}
		~ElementTitle() {}
	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
		virtual void focusInEvent(QFocusEvent *event);
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void keyPressEvent(QKeyEvent *event);
	};

	class NodeElement :  public QObject, public Element
	{
		Q_OBJECT

	public:
		/** @brief Конструктор */

		NodeElement();
		/** @brief Деструктор */

		~NodeElement();

		void complexInlineEditing();

		/** @brief Отрисовать объект */
		virtual void paint(QPainter* p, /**< Объект, осуществляющий отрисовку элементов */
						   const QStyleOptionGraphicsItem* opt, /**< Настройки отрисовки */
						   QWidget* w, /**< Виджет, на котором осуществляется отрисовка */
						   SdfRenderer* portrenderer /**< Рендерер портов)*/);
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


		/** @brief Добавить связь */
		void addEdge(EdgeElement *edge /**< Связь */)


		{
			edgeList << edge;
		}
		/** @brief Убрать связь */
		void delEdge(EdgeElement *edge /**< Связь */)


		{
			edgeList.removeAt(edgeList.indexOf(edge));
		}

		void setPortsVisible(bool value);

		ElementTitle d;


		//QString PutName();
		QString oldName;
			  public slots:
		void changeName();


	protected:


		bool portsVisible;
		QRectF scrollRect;

		QGraphicsProxyWidget scroll;

		/** @brief Обработать событие наведения на объект курсора мыши */
		void mouseMoveEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
		/** @brief Обработать событие отпускания кнопки мыши */
		void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event /**< Событие */);
		/** @brief Обработать событие нажатия кнопки мыши */
		void mousePressEvent( QGraphicsSceneMouseEvent * event /**< Событие */);








		//bool sceneEvent ( QEvent * event );
		/** @brief Обработать изменение данных объекта
			 *	@brief @return Измененные данные


			 * */
		virtual QVariant itemChange(GraphicsItemChange change, /**< Тип изменений */
									const QVariant &value /**< Величина изменения */


									);

		/** @brief Список точечных портов */

		QList<QPointF> pointPorts;
		/** @brief Список портов-линий */

		QList<statLine> linePorts;
		/** @brief Область, в которой возможно отображение текста, параметризующего SVG */

		QRectF m_contents;

		bool mLockChangeName;
		bool mLockUpdateText;
		ElementTitle docvis;
		ElementTitle doctype;
		QString typetext;
		QString vistext;

		float coord_def(QString , float ,float);
		float x_def(QString );
		float y_def(QString );
		float first_size_x;
		float first_size_y;


	private:

		/** @brief Получить объект, расположенный в данной точке сцены
			*	@brief @return Объект, расположенный в данной точке сцены


			* */
		NodeElement *getNodeAt( const QPointF &position /**< Точка на сцене */);


		/** @brief Список ассоциированных с объектом связей */

		QList<EdgeElement *> edgeList;

		/** @brief Направление растяжения элемента */

		enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
		/** @brief Направление растяжения */

		DragState dragState;

		/** @brief Описание двухмерной трансформации объекта */

		QTransform transform;
		QLineF newTransform(const statLine& port)  const;
		void adjustEdges();

	};

	/** @brief Описание линейного порта, реагирующего на абсолютные координаты */

	class statLine
	{
	public:
		QLineF line;
		bool prop_x1;
		bool prop_y1;
		bool prop_x2;
		bool prop_y2;
			public:
		statLine() {line = QLineF(0,0,0,0); prop_x1 = false; prop_x2 = false; prop_y1 = false; prop_y2 = false;}
		operator QLineF () const {return line;}
		void operator = (QLineF& l) {line = l; prop_x1 = false; prop_x2 = false; prop_y1 = false; prop_y2 = false;}
	};
}
