/** @file uml_nodeelement.h
 * 	@brief РљР»Р°СЃСЃ, РїСЂРµРґСЃС‚Р°РІР»СЏСЋС‰РёР№ РѕР±СЉРµРєС‚ РЅР° РґРёР°РіСЂР°РјРјРµ
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

/** @brief Р Р°Р·РјРµСЂ РїРѕСЂС‚Р° РѕР±СЉРµРєС‚Р° */
const int kvadratik = 5;

namespace UML {
	/** @class NodeElement
	* 	@brief РљР»Р°СЃСЃ, РїСЂРµРґСЃС‚Р°РІР»СЏСЋС‰РёР№ РѕР±СЉРµРєС‚ РЅР° РґРёР°РіСЂР°РјРјРµ
	 * */
	class statLine;

	// Why subclassing? Because the same is needed for class members, which is already subclassed.
	class ElementTitle : public QGraphicsTextItem
	{
		Q_OBJECT
	public:
		ElementTitle() {}
		~ElementTitle() {}
	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event)
		{
			QGraphicsTextItem::mousePressEvent(event);
			if (!(event->modifiers() & Qt::ControlModifier))
				scene()->clearSelection();
			parentItem()->setSelected(true);
		}
	};

	class NodeElement :  public QObject, public Element
	{
		Q_OBJECT

	public:
		/** @brief РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ */
		NodeElement();
		/** @brief Р”РµСЃС‚СЂСѓРєС‚РѕСЂ */
		~NodeElement();    

		void complexInlineEditing();
		
		/** @brief РћС‚СЂРёСЃРѕРІР°С‚СЊ РѕР±СЉРµРєС‚ */
		virtual void paint(QPainter* p, /**< РћР±СЉРµРєС‚, РѕСЃСѓС‰РµСЃС‚РІР»СЏСЋС‰РёР№ РѕС‚СЂРёСЃРѕРІРєСѓ СЌР»РµРјРµРЅС‚РѕРІ */
						   const QStyleOptionGraphicsItem* opt, /**< РќР°СЃС‚СЂРѕР№РєРё РѕС‚СЂРёСЃРѕРІРєРё */
						   QWidget* w, /**< Р’РёРґР¶РµС‚, РЅР° РєРѕС‚РѕСЂРѕРј РѕСЃСѓС‰РµСЃС‚РІР»СЏРµС‚СЃСЏ РѕС‚СЂРёСЃРѕРІРєР° */
						   SdfRenderer* portrenderer /**< Р РµРЅРґРµСЂРµСЂ РїРѕСЂС‚РѕРІ)*/);
		/** @brief РџРѕР»СѓС‡РёС‚СЊ РѕР±Р»Р°СЃС‚СЊ, РІ СЂР°РјРєР°С… РєРѕС‚РѕСЂРѕР№ РѕСЃСѓС‰РµСЃС‚РІР»СЏРµС‚СЃСЏ РѕС‚СЂРёСЃРѕРІРєР° РѕР±СЉРµРєС‚Р°
			 *	@brief @return РћР±Р»Р°СЃС‚СЊ, РІ СЂР°РјРєР°С… РєРѕС‚РѕСЂРѕР№ РѕСЃСѓС‰РµСЃС‚РІР»СЏРµС‚СЃСЏ РѕС‚СЂРёСЃРѕРІРєР° РѕР±СЉРµРєС‚Р°
			 * */
		QRectF boundingRect() const;
		/** @brief РџРѕР»СѓС‡РёС‚СЊ РѕР±Р»Р°СЃС‚СЊ, РІ СЂР°РјРєР°С… РєРѕС‚РѕСЂРѕР№ РІРѕР·РјРѕР¶РЅР° РїР°СЂР°РјРµС‚СЂРёР·Р°С†РёСЏ СЃС‚Р°С‚РёС‡РµСЃРєРѕРіРѕ SVG
			 *	@brief @return РћР±Р»Р°СЃС‚СЊ, РІ СЂР°РјРєР°С… РєРѕС‚РѕСЂРѕР№ РІРѕР·РјРѕР¶РЅР° РїР°СЂР°РјРµС‚СЂРёР·Р°С†РёСЏ СЃС‚Р°С‚РёС‡РµСЃРєРѕРіРѕ SVG
			 * */
		QRectF contentsRect() const;

		/** @brief РћР±РЅРѕРІРёС‚СЊ РґР°РЅРЅС‹Рµ СЌР»РµРјРµРЅС‚Р° */
		virtual void updateData();

		/** @brief РџРѕР»СѓС‡РёС‚СЊ СЂР°СЃРїРѕР»РѕР¶РµРЅРёРµ РїРѕСЂС‚Р°
			 *	@brief @return РљРѕРѕСЂРґРёРЅР°С‚С‹ РїРѕСЂС‚Р°
			 * */
		const QPointF getPortPos(qreal id /**< РРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ РїРѕСЂС‚Р° */) const;
		/** @brief РџРѕР»СѓС‡РёС‚СЊ РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ РїРѕСЂС‚Р°
			 *	@brief @return РРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ РїРѕСЂС‚Р°
			 * */
		qreal getPortId(const QPointF &location /**< Р Р°СЃРїРѕР»РѕР¶РµРЅРёРµ РїРѕСЂС‚Р° */) const;

		/** @brief Р”РѕР±Р°РІРёС‚СЊ СЃРІСЏР·СЊ */
		void addEdge(EdgeElement *edge /**< РЎРІСЏР·СЊ */)
		{
			edgeList << edge;
		}
		/** @brief РЈР±СЂР°С‚СЊ СЃРІСЏР·СЊ */
		void delEdge(EdgeElement *edge /**< РЎРІСЏР·СЊ */)
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

		/** @brief РћР±СЂР°Р±РѕС‚Р°С‚СЊ СЃРѕР±С‹С‚РёРµ РЅР°РІРµРґРµРЅРёСЏ РЅР° РѕР±СЉРµРєС‚ РєСѓСЂСЃРѕСЂР° РјС‹С€Рё */
		void mouseMoveEvent ( QGraphicsSceneMouseEvent * event /**< РЎРѕР±С‹С‚РёРµ */);
		/** @brief РћР±СЂР°Р±РѕС‚Р°С‚СЊ СЃРѕР±С‹С‚РёРµ РѕС‚РїСѓСЃРєР°РЅРёСЏ РєРЅРѕРїРєРё РјС‹С€Рё */
		void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event /**< РЎРѕР±С‹С‚РёРµ */);
		/** @brief РћР±СЂР°Р±РѕС‚Р°С‚СЊ СЃРѕР±С‹С‚РёРµ РЅР°Р¶Р°С‚РёСЏ РєРЅРѕРїРєРё РјС‹С€Рё */
		void mousePressEvent( QGraphicsSceneMouseEvent * event /**< РЎРѕР±С‹С‚РёРµ */);


		//bool sceneEvent ( QEvent * event );
		/** @brief РћР±СЂР°Р±РѕС‚Р°С‚СЊ РёР·РјРµРЅРµРЅРёРµ РґР°РЅРЅС‹С… РѕР±СЉРµРєС‚Р°
			 *	@brief @return РР·РјРµРЅРµРЅРЅС‹Рµ РґР°РЅРЅС‹Рµ
			 * */
		virtual QVariant itemChange(GraphicsItemChange change, /**< РўРёРї РёР·РјРµРЅРµРЅРёР№ */
									const QVariant &value /**< Р’РµР»РёС‡РёРЅР° РёР·РјРµРЅРµРЅРёСЏ */
									);

		/** @brief РЎРїРёСЃРѕРє С‚РѕС‡РµС‡РЅС‹С… РїРѕСЂС‚РѕРІ */
		QList<QPointF> pointPorts;
		/** @brief РЎРїРёСЃРѕРє РїРѕСЂС‚РѕРІ-Р»РёРЅРёР№ */
		QList<statLine> linePorts;
		/** @brief РћР±Р»Р°СЃС‚СЊ, РІ РєРѕС‚РѕСЂРѕР№ РІРѕР·РјРѕР¶РЅРѕ РѕС‚РѕР±СЂР°Р¶РµРЅРёРµ С‚РµРєСЃС‚Р°, РїР°СЂР°РјРµС‚СЂРёР·СѓСЋС‰РµРіРѕ SVG */
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

		/** @brief РџРѕР»СѓС‡РёС‚СЊ РѕР±СЉРµРєС‚, СЂР°СЃРїРѕР»РѕР¶РµРЅРЅС‹Р№ РІ РґР°РЅРЅРѕР№ С‚РѕС‡РєРµ СЃС†РµРЅС‹
			*	@brief @return РћР±СЉРµРєС‚, СЂР°СЃРїРѕР»РѕР¶РµРЅРЅС‹Р№ РІ РґР°РЅРЅРѕР№ С‚РѕС‡РєРµ СЃС†РµРЅС‹
			* */
		NodeElement *getNodeAt( const QPointF &position /**< РўРѕС‡РєР° РЅР° СЃС†РµРЅРµ */);

		/** @brief РЎРїРёСЃРѕРє Р°СЃСЃРѕС†РёРёСЂРѕРІР°РЅРЅС‹С… СЃ РѕР±СЉРµРєС‚РѕРј СЃРІСЏР·РµР№ */
		QList<EdgeElement *> edgeList;

		/** @brief РќР°РїСЂР°РІР»РµРЅРёРµ СЂР°СЃС‚СЏР¶РµРЅРёСЏ СЌР»РµРјРµРЅС‚Р° */
		enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
		/** @brief РќР°РїСЂР°РІР»РµРЅРёРµ СЂР°СЃС‚СЏР¶РµРЅРёСЏ */
		DragState dragState;

		/** @brief РћРїРёСЃР°РЅРёРµ РґРІСѓС…РјРµСЂРЅРѕР№ С‚СЂР°РЅСЃС„РѕСЂРјР°С†РёРё РѕР±СЉРµРєС‚Р° */
		QTransform transform;
		QLineF newTransform(const statLine& port)  const;

	};

	/** @brief РћРїРёСЃР°РЅРёРµ Р»РёРЅРµР№РЅРѕРіРѕ РїРѕСЂС‚Р°, СЂРµР°РіРёСЂСѓСЋС‰РµРіРѕ РЅР° Р°Р±СЃРѕР»СЋС‚РЅС‹Рµ РєРѕРѕСЂРґРёРЅР°С‚С‹ */
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
