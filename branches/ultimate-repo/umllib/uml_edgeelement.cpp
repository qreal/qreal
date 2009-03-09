/** @file edgeelement.cpp
 * 	@brief Класс, представляющий связь на диаграмме
 * */
#include <QtGui>
#include <QtGlobal>

#include "editorviewscene.h"

#include "uml_edgeelement.h"
#include "uml_nodeelement.h"

#include "realreporoles.h"
#include "realrepoinfo.h"

#include <math.h>

using namespace UML;

#ifndef M_PI
/** @brief Константа ПИ */
#define M_PI 3.14159265358979323846264338327950288419717
/** @brief Константа 1/ПИ */
#define M_1_PI 1/M_PI;
// Реквестирую ещё массу бозона Хиггса!
// Here you are: The God's particle energy (in GeV)
#define HIGGS_BOSON_MASS_APPROX 251
#endif //M_PI

/** @brief Индикатор перемещения связи */
// static bool moving = false;

EdgeElement::EdgeElement()
: beginning(0), ending(0), src(0), dst(0), portFrom(0), portTo(0), dragState(-1), longPart(0)
{
	setZValue(100);
	setFlag(ItemIsMovable, true);
	// FIXME: draws strangely...
	setFlag(ItemClipsToShape, false);

	m_penStyle = Qt::SolidLine;
	m_line << QPointF(0,0) << QPointF(200,60);

	m_endArrowStyle = NO_ARROW;
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
	// return m_line.boundingRect().adjusted(-kvadratik,-kvadratik,kvadratik,kvadratik);
	return m_line.boundingRect().adjusted(-20,-20,20,20);
}

static double lineAngle(const QLineF &line)
{
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2*M_PI - angle;

	return angle*180*M_1_PI;
}

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	painter->save();
	QPen pen = painter->pen();
	pen.setColor(m_color);
	pen.setStyle(m_penStyle);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawPolyline(m_line);
	painter->restore();

	painter->save();
	painter->translate(m_line[0]);
	painter->drawText(QPointF(10,20), m_fromMult);
	painter->rotate(90-lineAngle(QLineF(m_line[1],m_line[0])));
	drawStartArrow(painter);
	painter->restore();

	painter->save();
	painter->translate(m_line[m_line.size()-1]);
	painter->drawText(QPointF(10,20), m_toMult);
	painter->rotate(90-lineAngle(QLineF(m_line[m_line.size()-2],m_line[m_line.size()-1])));
	drawEndArrow(painter);
	painter->restore();

	if (option->state & QStyle::State_Selected) {
		painter->setBrush(Qt::SolidPattern);
		foreach( QPointF point, m_line)
		{
			QPen pen;
			QColor color;

			color.setNamedColor("#c3dcc4");
			pen.setWidth(11);
			pen.setColor(color);
			painter->setPen(pen);
			painter->drawPoint(point);

			color.setNamedColor("#465945");
			pen.setWidth(3);
			pen.setColor(color);
			painter->setPen(pen);
			painter->drawPoint(point);
		}
	}

	if ( ! m_text.isEmpty() ) {
		painter->save();
		QLineF longest(m_line[longPart],m_line[longPart+1]);
		painter->translate(m_line[longPart]);
		painter->rotate(-lineAngle(longest));

//		painter->drawText(0,-15,longest.length(),15,
//				Qt::TextSingleLine | Qt::AlignCenter, m_text);

		QTextDocument d;
		d.setHtml(m_text);
		d.setTextWidth(longest.length());
//		painter->drawRect(QRectF(0,0,longest.length(),20));
		d.drawContents(painter);

		painter->restore();
	}
}

bool canBeConnected( int linkID, int from, int to );
/*
void EdgeElement::checkConnection()
{
//	int type = this->type();
	int from = -1;
	int to = -1;

	if ( src != 0 )
		from = src->type();

	if ( dst != 0 )
		to = dst->type();

	m_color = Qt::black;
}
*/
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

int EdgeElement::getPoint( const QPointF &location )
{
	for ( int i = 0 ; i < m_line.size() ; i++ )
		if ( QRectF(m_line[i]-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( location ) )
			return i;

	return -1;
}

void EdgeElement::updateLongestPart()
{
	qreal maxLen = 0.0;
	int maxIdx = 0;
	for ( int i = 0; i < m_line.size() - 1 ; i++ ) {
		qreal newLen = QLineF(m_line[i],m_line[i+1]).length();
		if ( newLen > maxLen ) {
			maxLen = newLen;
			maxIdx = i;
		}
	}
	longPart = maxIdx;
}

void EdgeElement::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	dragState = -1;

	if ( isSelected() )
		dragState = getPoint( event->pos() );

	if ( dragState == -1 )
		Element::mousePressEvent(event);
}

void EdgeElement::connectToPort() {
	QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());

	setPos(pos()+m_line[0]);
	m_line.translate(-m_line[0]);

	moving = true;

	// Now we check whether start or end have been connected
	NodeElement *new_src = getNodeAt(m_line[0]);
	if ( new_src )
		portFrom = new_src->getPortId( mapToItem(new_src, m_line[0]) );
	else
		portFrom = -1.0;

	if ( src ) {
		src->delEdge(this);
		src = 0;
	}

	if ( portFrom >= 0.0 ) {
		src = new_src;
		src->addEdge(this);
	}

	if ( src )
		im->setData(dataIndex, src->uuid(), Unreal::krneRelationship::fromRole);
	else
		im->setData(dataIndex, 0, Unreal::krneRelationship::fromRole);

	im->setData(dataIndex, portFrom, Unreal::krneRelationship::fromPortRole);


	NodeElement *new_dst = getNodeAt(m_line[m_line.size()-1]);
	if ( new_dst )
		portTo = new_dst->getPortId( mapToItem(new_dst, m_line[m_line.size()-1]) );
	else
		portTo = -1.0;

	if ( dst ) {
		dst->delEdge(this);
		dst = 0;
	}

	if ( portTo >= 0.0 ) {
		dst = new_dst;
		dst->addEdge(this);
	}

	if ( dst )
		im->setData(dataIndex, dst->uuid(), Unreal::krneRelationship::toRole);
	else
		im->setData(dataIndex, 0, Unreal::krneRelationship::toRole);

	im->setData(dataIndex, portTo, Unreal::krneRelationship::toPortRole);

	setFlag(ItemIsMovable, !(dst||src) );

	im->setData(dataIndex, pos(), Unreal::PositionRole);
	im->setData(dataIndex, m_line.toPolygon(), Unreal::ConfigurationRole);

	moving = false;

	adjustLink();
}

void EdgeElement::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	NodeElement *new_src = getNodeAt(m_line[0]);
	NodeElement *new_dst = getNodeAt(m_line[m_line.size()-1]);
	if (beginning) {
		if (beginning!=new_src) {
			beginning->setPortsVisible(false);
		}
	}
	if (ending) {
		if (ending!=new_dst) {
			ending->setPortsVisible(false);
		}
	}
	beginning = new_src;
	ending = new_dst;
	if (beginning) {
		beginning->setPortsVisible(true);
	}
	if (ending) {
		ending->setPortsVisible(true);
	}

	if ( dragState == -1 ) {
		Element::mouseMoveEvent(event);
	} else {
		prepareGeometryChange();
		m_line[dragState] = event->pos();
		updateLongestPart();
	}
}

void EdgeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	if ( dragState == -1 )
		Element::mouseReleaseEvent(event);
	else
		dragState = -1;
	connectToPort();
	if (beginning) {
		beginning->setPortsVisible(false);
	}
	if (ending) {
		ending->setPortsVisible(false);
	}
	// cleanup after moving/resizing
	beginning = ending = NULL;
}

NodeElement *EdgeElement::getNodeAt( const QPointF &position )
{
	foreach( QGraphicsItem *item, scene()->items(mapToScene(position)) ) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if ( e )
			return e;
	}
	return 0;
}

void EdgeElement::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
	QMenu menu;

	QAction *addPointAction = menu.addAction("Add point");
	QAction *delPointAction = menu.addAction("Remove point");
	QAction *squarizeAction = menu.addAction("Squarize :)");

	if ( QAction *selectedAction = menu.exec(event->screenPos()) ) {
		if ( selectedAction == delPointAction ) {
			int i = getPoint( event->pos() );
			if ( i != -1 ) {
				prepareGeometryChange();
				m_line.remove(i);
				longPart = 0;
				update();
			}
		} else if ( selectedAction == addPointAction ) {
			for ( int i = 0; i < m_line.size()-1; i++ ) {
				QPainterPath path;
				QPainterPathStroker ps;
				ps.setWidth(kvadratik);

				path.moveTo(m_line[i]);
				path.lineTo(m_line[i+1]);
				if ( ps.createStroke(path).contains(event->pos()) ) {
					m_line.insert(i+1,event->pos());
					update();
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
			adjustLink();
			update();
		}
	}
}

void EdgeElement::adjustLink()
{
	prepareGeometryChange();
	if ( src )
		m_line[0] = mapFromItem(src, src->getPortPos(portFrom) );
	if ( dst )
		m_line[m_line.size()-1] = mapFromItem(dst, dst->getPortPos(portTo) );
	updateLongestPart();
}

void EdgeElement::updateData()
{
	if (moving)
		return;

	Element::updateData();

	RealRepoInfo info;
	TypeIdType type = dataIndex.data(Unreal::TypeRole).toString();

	m_fromMult = dataIndex.data(info.roleByColumnName(type, "fromMultiplicity")).toString();
	m_toMult = dataIndex.data(info.roleByColumnName(type, "toMultiplicity")).toString();

	setPos(dataIndex.data(Unreal::PositionRole).toPointF());
	QPolygonF newLine = dataIndex.data(Unreal::ConfigurationRole).value<QPolygon>();
	if (!newLine.isEmpty())
		m_line = newLine;

	qDebug() << "from role: " << Unreal::krneRelationship::fromRole
		<< "to role: " << Unreal::krneRelationship::toRole;

	IdType uuidFrom = dataIndex.data(info.roleByColumnName(type, "from")).toString();
	IdType uuidTo = dataIndex.data(info.roleByColumnName(type, "to")).toString();

	qDebug() << "from: " << uuidFrom << ", to: " << uuidTo;

	if (src)
		src->delEdge(this);
	if (dst)
		dst->delEdge(this);

	src = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(uuidFrom));
	dst = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(uuidTo));

	if (src)
		src->addEdge(this);
	if (dst)
		dst->addEdge(this);

	setFlag(ItemIsMovable, !(dst || src));

	portFrom = dataIndex.data(info.roleByColumnName(type, "fromPort")).toDouble();
	portTo = dataIndex.data(info.roleByColumnName(type, "toPort")).toDouble();

	adjustLink();
}
