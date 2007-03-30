#include <QtGui>

#include "editorviewscene.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

using namespace UML;

const int kvadratik = 4;

EdgeElement::EdgeElement()
    : src(0), dst(0), srcPoint(0,0), dstPoint(50,150), portFrom(0), portTo(0)
{
    dragState = 0;
    setZValue(100);
    setFlag(ItemIsMovable, true);
}

EdgeElement::~EdgeElement()
{
    qDebug() << "edgeelement destructor";
    if (src)
        src->delEdge(this);
    if (dst)
	dst->delEdge(this);			
}

QRectF EdgeElement::boundingRect() const
{
   qreal penWidth = 1;
   qreal arrowSize = 10;
   qreal extra = (penWidth + arrowSize + kvadratik) / 2.0 + 20;

   return QRectF(srcPoint, QSizeF(dstPoint.x() - srcPoint.x(),
                     dstPoint.y() - srcPoint.y()))
             .normalized()
             .adjusted(-extra, -extra, extra, extra);
}

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    QPainterPath path;
    path.moveTo(srcPoint);
    path.lineTo(dstPoint);
    painter->drawPath(path);


///////tmp///////

    QLineF line(srcPoint,dstPoint);

    double arrowSize = 10.0;
    
    // Draw the arrows if there's enough room
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
            angle = TwoPi - angle;

    QPointF sourceArrowP1 = srcPoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                            cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = srcPoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                            cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = dstPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                            cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = dstPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                            cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::white);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
																				

//////tmp///////    
    
    

    if (option->state & QStyle::State_Selected) {
        painter->setBrush(Qt::SolidPattern);
        painter->drawRect(QRectF(srcPoint-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)));
        painter->drawRect(QRectF(dstPoint-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)));
    }
}

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
    p.addPath(path);
    return p;
}

QPainterPath EdgeElement::shape() const
{
    QPainterPath path;
    path.moveTo(srcPoint);
    path.lineTo(dstPoint);
    return qt_graphicsItem_shapeFromPath(path,QPen(Qt::black,kvadratik*2));
}

void EdgeElement::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    dragState = 0;
    if ( QRectF(srcPoint-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( event->pos() ) ) {
	dragState = 1;
    } else if ( QRectF(dstPoint-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( event->pos() ) ) {
        dragState = 2;
    } else
	Element::mousePressEvent(event);
}

void EdgeElement::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    if ( dragState == 0 ) {
        Element::mouseMoveEvent(event);
    } else {
	    prepareGeometryChange();
	    if ( dragState == 1 ) {
		srcPoint = event->pos();
	    } else if ( dragState == 2 ) {
        	dstPoint = event->pos();
	    }
    }
}

void EdgeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if ( dragState != 0 ) {
	    
//    int uuidFrom = idx.sibling(myrow,5).data().toInt();
//    int uuidTo = idx.sibling(myrow,6).data().toInt();

	    // FIXME: probably slow.
	    NodeElement *e = 0;
	    foreach( QGraphicsItem *item, scene()->items(mapToScene(event->pos())) ) {
                if ( e = dynamic_cast<NodeElement *>(item) ) {
		    if ( dragState == 1 ) {
			src = e;
			src->addEdge(this);
			portFrom = src->getNearestPort(mapToItem(src,event->pos()));
			srcPoint = src->getPort(portFrom);
    		
		        QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
			im->setData(dataIndex.sibling(dataIndex.row(),5), e->uuid() );

		    } else if ( dragState == 2 ) {
			dst = e;
			dst->addEdge(this);
                        portTo = dst->getNearestPort(mapToItem(dst,event->pos()));
                        dstPoint = dst->getPort(portTo);

        		QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
			im->setData(dataIndex.sibling(dataIndex.row(),6), e->uuid() );
		    }
		    setFlag(ItemIsMovable, false);

		    break;
	        }
	    }
	    
	    if ( !e ) {
		if ( dragState == 1 ) {
		    if (src) {
			src->delEdge(this);
		    }
		    src = 0;

	            QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
    	            im->setData(dataIndex.sibling(dataIndex.row(),5), 0 );
        	} else if ( dragState == 2 ) {
		    if (dst) {
		        dst->delEdge(this);
		    }
             	    dst = 0;

		    QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
    	            im->setData(dataIndex.sibling(dataIndex.row(),6), 0 );
    	        }
		if ( ! src && ! dst )
		        setFlag(ItemIsMovable, true);
	    }

            dragState = 0;
    } else
        Element::mouseReleaseEvent(event);
}

void EdgeElement::adjustLink()
{
    prepareGeometryChange();
    if ( src )
	srcPoint = mapFromItem(src, src->getPort(portFrom));
    if ( dst )
        dstPoint = mapFromItem(dst, dst->getPort(portTo));
    update();
}

void EdgeElement::updateData()
{
    int myrow = dataIndex.row();
    int uuidFrom = dataIndex.sibling(myrow,5).data().toInt();
    int uuidTo = dataIndex.sibling(myrow,6).data().toInt();

    qDebug() << "src" << uuidFrom << "dst" << uuidTo;
    
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

    setFlag(ItemIsMovable, ( ! src && ! dst ) );

    Element::updateData();
}




