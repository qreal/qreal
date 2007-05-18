#include <QtGui>
#include <QtGlobal>

#include "editorviewscene.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

#include "realreporoles.h"

#include <math.h>

using namespace UML;

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

EdgeElement::EdgeElement()
: src(0), dst(0), portFrom(0), portTo(0), dragState(-1)
{
	setZValue(100);
	setFlag(ItemIsMovable, true);
	// FIXME: draws strangely...
	setFlag(ItemClipsToShape, false);

	m_penStyle = Qt::SolidLine;

	m_line << QPointF(-100,-30) << QPointF(100,30);
}

EdgeElement::~EdgeElement()
{
	if (src)
		src->delEdge(this);
	if (dst)
		dst->delEdge(this);
}

QRectF EdgeElement::boundingRect() const
{
	//	return m_line.boundingRect().adjusted(-kvadratik,-kvadratik,kvadratik,kvadratik);
	return m_line.boundingRect().adjusted(-20,-20,20,20);
}

static double lineAngle(const QLineF &line)
{
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = TwoPi - angle;

	return angle*180/Pi;
}

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	painter->save();
	QPen pen = painter->pen();
	pen.setColor(m_color);
	pen.setStyle(m_penStyle);
	painter->setPen(pen);
	painter->drawPolyline(m_line);
	painter->restore();

	painter->save();
	painter->translate(m_line[0]);
	painter->rotate(90-lineAngle(QLineF(m_line[1],m_line[0])));
	drawStartArrow(painter);
	painter->restore();

	painter->save();
	painter->translate(m_line[m_line.size()-1]);
	painter->rotate(90-lineAngle(QLineF(m_line[m_line.size()-2],m_line[m_line.size()-1])));
	drawEndArrow(painter);
	painter->restore();

	if (option->state & QStyle::State_Selected) {
		painter->setBrush(Qt::SolidPattern);
		foreach( QPointF point, m_line) {
			painter->drawRect(QRectF(point-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)));
		}
	}
}

bool canBeConnected( int linkID, int from, int to );

void EdgeElement::checkConnection()
{
	int type = this->type();
	int from = -1;
	int to = -1;

	if ( src != 0 )
		from = src->type();

	if ( dst != 0 )
		to = dst->type();

	if ( canBeConnected( type, from, to ) )
		m_color = Qt::black;
	else
		m_color = Qt::red;

}

QPainterPath EdgeElement::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	
	QPainterPathStroker ps;
	ps.setWidth(kvadratik);

	path.addPolygon(m_line);
	path = ps.createStroke(path);

	foreach( QPointF point, m_line) {
		path.addRect(QRectF(point-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)));
	}

	return path;
}

void EdgeElement::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	dragState = -1;

	if ( isSelected() ) {
		for ( int i = 0 ; i < m_line.size() ; i++ ) {
			if ( QRectF(m_line[i]-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( event->pos() ) ) {
				dragState = i;
				break;
			}
		}
	}

	if ( dragState == -1 )
		Element::mousePressEvent(event);
}

void EdgeElement::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	if ( dragState == -1 ) {
		Element::mouseMoveEvent(event);
	} else {
		prepareGeometryChange();
		m_line[dragState] = event->pos();
	}
}

void EdgeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	if ( dragState != -1 ) {

		//    int uuidFrom = idx.sibling(myrow,5).data().toInt();
		//    int uuidTo = idx.sibling(myrow,6).data().toInt();

		// FIXME: probably slow.
		NodeElement *e = 0;
		foreach( QGraphicsItem *item, scene()->items(mapToScene(event->pos())) ) {
			e = dynamic_cast<NodeElement *>(item); 
			if ( e ) {
				if ( dragState == 0 ) {
					src = e;
					src->addEdge(this);
					portFrom = src->getNearestPort(mapToItem(src,event->pos()));
					prepareGeometryChange();
					m_line[0] = mapFromItem(src,src->getPort(portFrom));
					QString fromPort = QString("%1:%2").arg(e->uuid()).arg(portFrom);

					QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
					im->setData(dataIndex, fromPort, Unreal::krneRelationship::fromRole );

				} else if ( dragState == m_line.size()-1 ) {
					dst = e;
					dst->addEdge(this);
					portTo = dst->getNearestPort(mapToItem(dst,event->pos()));
					prepareGeometryChange();
					m_line[m_line.size()-1] = mapFromItem(dst,dst->getPort(portTo));
					QString toPort = QString("%1:%2").arg(e->uuid()).arg(portTo);

					QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
					im->setData(dataIndex, toPort, Unreal::krneRelationship::toRole );
				}
				setFlag(ItemIsMovable, false);
				checkConnection();

				break;
			}
		}

		if ( !e ) {
			if ( dragState == 0 ) {
				if (src)
					src->delEdge(this);
				src = 0;

				QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
				im->setData(dataIndex, "0:0", Unreal::krneRelationship::fromRole );
			} else if ( dragState == m_line.size()-1 ) {
				if (dst)
					dst->delEdge(this);
				dst = 0;

				QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
				im->setData(dataIndex, "0:0", Unreal::krneRelationship::toRole );
			}
			if ( ! src && ! dst )
				setFlag(ItemIsMovable, true);
			checkConnection();
		}

		dragState = -1;
	} else
		Element::mouseReleaseEvent(event);

	dragState = -1;
}

void EdgeElement::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
	QMenu menu;

	QAction *addPointAction = menu.addAction("Add point");
	QAction *delPointAction = menu.addAction("Remove point");
	QAction *squarizeAction = menu.addAction("Squarize :)");

	if ( QAction *selectedAction = menu.exec(event->screenPos()) ) {
		if ( selectedAction == delPointAction ) {
			for ( int i = 1 ; i < m_line.size()-1 ; i++ ) {
				if ( QRectF(m_line[i]-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( event->pos() ) ) {
					prepareGeometryChange();
					m_line.remove(i);
					update();
					break;
				}
			}
		} else if ( selectedAction == addPointAction ) {
			for ( int i = 0; i < m_line.size()-1; i++ ) {
				QPainterPath path;
				path.moveTo(m_line[i]);
				path.lineTo(m_line[i+1]);
				if ( shape().contains(event->pos()) ) {
					m_line.insert(i+1,event->pos());
					break;
				}
			}
		} else if ( selectedAction == squarizeAction ) {
			prepareGeometryChange();
			for ( int i = 0; i < m_line.size()-1; i++ ) {
				QLineF line(m_line[i],m_line[i+1]);	
				if ( qAbs(line.dx()) < qAbs(line.dy()) ) {
					m_line[i+1].setX(m_line[i].x());		    
				} else {
					m_line[i+1].setY(m_line[i].y());
				}
			}
		} 
	}
}

void EdgeElement::adjustLink()
{
	prepareGeometryChange();
	if ( src )
		m_line[0] = mapFromItem(src, /* src->getPort(portFrom) */ QPointF(0,0) );
	if ( dst )
		m_line[m_line.size()-1] = mapFromItem(dst, /* dst->getPort(portTo) */ QPointF(0,0) );
	update();

//	qDebug() << m_line[0] << m_line[m_line.size()-1];
	qDebug() << portFrom << portTo;
}

void EdgeElement::updateData()
{
	Element::updateData();

	// temporary code, to be reworked with introduction of new schema handling
	QString from = dataIndex.data(Unreal::reqeP2N::fromRole).toString();
	QString to = dataIndex.data(Unreal::reqeP2N::toRole).toString();

	int uuidFrom = from.split(":").at(0).toInt();
	int uuidTo = to.split(":").at(0).toInt();

	if ( from.split(":").size() > 1 )
		portFrom = from.split(":").at(1).toInt();
	if ( to.split(":").size() > 1 )
		portTo = to.split(":").at(1).toInt();

	if ( EditorViewScene *scene = dynamic_cast<EditorViewScene *>(this->scene()) ) {

		if (src)
			src->delEdge(this);
		if (uuidFrom) {
			QGraphicsItem *tmp = scene->getElem(uuidFrom);
			if ( tmp ) 
				src = dynamic_cast<NodeElement *>(tmp);
		}
		if (src)
			src->addEdge(this);

		if (dst)
			dst->delEdge(this);
		if (uuidTo) {
			qDebug() << uuidTo;
			QGraphicsItem *tmp = scene->getElem(uuidTo);
			if ( tmp )
				dst = dynamic_cast<NodeElement *>(tmp);
		}
		if (dst)
			dst->addEdge(this);

		adjustLink();
	} else {
		qDebug() << "no scene!";
	}    

	if ( ! src && ! dst ) {
		setFlag(ItemIsMovable, true );
		setPos(dataIndex.data(Unreal::PositionRole).toPoint());
	} else {
		setFlag(ItemIsMovable, false );
	}
}
