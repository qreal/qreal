#include <QtGui>
#include <QtGlobal>

#include "editorviewscene.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

using namespace UML;

const int kvadratik = 4;

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
	// We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
	// if we pass a value of 0.0 to QPainterPathStroker::setWidth()
	const qreal penWidthZero = qreal(0.00000001);

	if (path == QPainterPath())
		return path;
	QPainterPathStroker ps;
	ps.setCapStyle(pen.capStyle());
	ps.setWidth(pen.widthF() <= 0.0 ? penWidthZero : pen.widthF());
	ps.setJoinStyle(pen.joinStyle());
	ps.setMiterLimit(pen.miterLimit());
	QPainterPath p = ps.createStroke(path);
	return p;
}

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


EdgeElement::EdgeElement()
	: src(0), dst(0), portFrom(0), portTo(0)
{
	dragState = -1;
	setZValue(100);
	setFlag(ItemIsMovable, true);

	m_line << QPointF(-50,-10) << QPointF(50,10);
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
	return m_line.boundingRect().adjusted(-kvadratik,-kvadratik,kvadratik,kvadratik);
}

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	painter->drawPolyline(m_line);

	if (option->state & QStyle::State_Selected) {
		painter->setBrush(Qt::SolidPattern);
		foreach( QPointF point, m_line) {
			painter->drawRect(QRectF(point-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)));
		}
	}
}

QPainterPath EdgeElement::shape() const
{
	QPainterPath path;
	path.addPolygon(m_line);
	return qt_graphicsItem_shapeFromPath(path,QPen(Qt::black,kvadratik*2));
}

void EdgeElement::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	dragState = -1;

	for ( int i = 0 ; i < m_line.size() ; i++ ) {
		if ( QRectF(m_line[i]-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( event->pos() ) ) {
			dragState = i;
			break;
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
			if ( e = dynamic_cast<NodeElement *>(item) ) {
				if ( dragState == 0 ) {
					src = e;
					src->addEdge(this);
					portFrom = src->getNearestPort(mapToItem(src,event->pos()));
					prepareGeometryChange();
					m_line[0] = mapFromItem(src,src->getPort(portFrom));
					QString fromPort = QString("%1:%2").arg(e->uuid()).arg(portFrom);

					QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
					im->setData(dataIndex.sibling(dataIndex.row(),7), fromPort);

				} else if ( dragState == m_line.size()-1 ) {
					dst = e;
					dst->addEdge(this);
					portTo = dst->getNearestPort(mapToItem(dst,event->pos()));
					prepareGeometryChange();
					m_line[m_line.size()-1] = mapFromItem(dst,dst->getPort(portTo));
					QString toPort = QString("%1:%2").arg(e->uuid()).arg(portTo);

					QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
					im->setData(dataIndex.sibling(dataIndex.row(),8), toPort );
				}
				setFlag(ItemIsMovable, false);

				break;
			}
		}

		if ( !e ) {
			if ( dragState == 0 ) {
				if (src) {
					src->delEdge(this);
				}
				src = 0;

				QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
				im->setData(dataIndex.sibling(dataIndex.row(),7), "0:0" );
			} else if ( dragState == m_line.size()-1 ) {
				if (dst) {
					dst->delEdge(this);
				}
				dst = 0;

				QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
				im->setData(dataIndex.sibling(dataIndex.row(),8), "0:0" );
			}
			if ( ! src && ! dst )
				setFlag(ItemIsMovable, true);
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
				if ( qt_graphicsItem_shapeFromPath(path,QPen(Qt::black,kvadratik*2)).contains(event->pos()) ) {
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
		m_line[0] = mapFromItem(src, src->getPort(portFrom));
	if ( dst )
		m_line[m_line.size()-1]= mapFromItem(dst, dst->getPort(portTo));
	update();
}

void EdgeElement::updateData()
{
	Element::updateData();

	// temporary code, to be reworked with introduction of new schema handling

	int myrow = dataIndex.row();

	QString from = dataIndex.sibling(myrow,7).data().toString();
	QString to = dataIndex.sibling(myrow,8).data().toString();

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
		int x = dataIndex.sibling(myrow,4).data().toInt();
		int y = dataIndex.sibling(myrow,5).data().toInt();
		setPos(x,y);

	} else {
		setFlag(ItemIsMovable, false );
	}
}




