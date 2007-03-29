#include <QtGui>

#include "editorviewscene.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

using namespace UML;

const int kvadratik = 4;

EdgeElement::EdgeElement()
    : src(0), dst(0), srcPoint(0,0), dstPoint(50,150)
{
    dragState = 0;
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

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    QPainterPath path;
    path.moveTo(srcPoint);
    path.lineTo(dstPoint);
    painter->drawPath(path);

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
	    } else if ( dragState = 2 ) {
        	dstPoint = event->pos();
	    }
    }
}

void EdgeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if ( dragState != 0 ) {
	    
//    int uuidFrom = idx.sibling(myrow,6).data().toInt();
//    int uuidTo = idx.sibling(myrow,7).data().toInt();

	    // FIXME: probably slow.
	    NodeElement *e = 0;
	    foreach( QGraphicsItem *item, scene()->items(mapToScene(event->pos())) ) {
                if ( e = dynamic_cast<NodeElement *>(item) ) {
		    if ( dragState == 1 ) {
			src = e;
			src->addEdge(this);
    		
            QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
			im->setData(dataIndex.sibling(dataIndex.row(),5), e->uuid() );
		    } else if ( dragState == 2 ) {
			dst = e;
			dst->addEdge(this);
    		        QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
			im->setData(dataIndex.sibling(dataIndex.row(),6), e->uuid() );
		    }

		    break;
	        }
	    }
	    
	    if ( !e ) {
		if ( dragState == 1 ) {
		    if (src) {
			src->delEdge(this);
		    }
		    src = 0;

//	            QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
//    	            im->setData(dataIndex.sibling(dataIndex.row(),6), 0 );
        	} else if ( dragState == 2 ) {
		    if (dst) {
		        dst->delEdge(this);
		    }
             	    dst = 0;

//                  QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
//	            im->setData(dataIndex.sibling(dataIndex.row(),7), 0 );
    	        }
	    }

            dragState = 0;
    } else
        Element::mouseReleaseEvent(event);
}

void EdgeElement::adjustLink()
{
    prepareGeometryChange();
    if ( src )
	srcPoint = mapFromItem(src, 0, 0);
    if ( dst )
        dstPoint = mapFromItem(dst, 0, 0);
    update();
}

void EdgeElement::updateData()
{
    int myrow = dataIndex.row();
    int uuidFrom = dataIndex.sibling(myrow,6).data().toInt();
    int uuidTo = dataIndex.sibling(myrow,7).data().toInt();
    
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

    Element::updateData();
}




