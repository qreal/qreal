#include "../view/editorViewScene.h"

#include <QtCore/qmath.h>
#include "nodeElement.h"
#include "portHandler.h"

PortHandler::PortHandler(NodeElement *node, qReal::models::GraphicalModelAssistApi *graphicalAssistApi,
	QList<StatPoint> const &pointPorts, QList<StatLine> const &linePorts)
	: mNode(node), mGraphicalAssistApi(graphicalAssistApi)
	, mPointPorts(pointPorts), mLinePorts(linePorts)
{
}

qreal PortHandler::minDistanceFromLinePort(int linePortNumber, QPointF const &location) const
{
	QLineF const linePort = newTransform(mLinePorts[linePortNumber]);
	qreal const a = linePort.length();
	qreal const b = QLineF(linePort.p1(), location).length();
	qreal const c = QLineF(linePort.p2(), location).length();

	qreal const nearestPointOfLinePort = getNearestPointOfLinePort(linePortNumber, location);
	if ((nearestPointOfLinePort < 0) || (nearestPointOfLinePort > 0.9999)) {
		return qMin(b, c);
	} else {
		qreal const p = (a + b + c) / 2;
		qreal const triangleSquare = sqrt(p * (p - a) * (p - b) * (p - c));
		qreal const minDistance = 2 * triangleSquare / a;
		return minDistance;
	}
}

qreal PortHandler::distanceFromPointPort(int pointPortNumber, QPointF const &location) const
{
	return QLineF(newTransform(mPointPorts[pointPortNumber]), location).length();
}

qreal PortHandler::getNearestPointOfLinePort(int linePortNumber, QPointF const &location) const
{
	qreal nearestPointOfLinePort = 0;
	QLineF nearestLinePort = newTransform(mLinePorts[linePortNumber]);
	qreal y1 = nearestLinePort.y1();
	qreal y2 = nearestLinePort.y2();
	qreal x1 = nearestLinePort.x1();
	qreal x2 = nearestLinePort.x2();

	if (x1 == x2) {
		nearestPointOfLinePort = (location.y() - y1) / (y2 - y1);
	} else if (y1 == y2) {
		nearestPointOfLinePort = (location.x() - x1) / (x2 - x1);
	} else {
		qreal k = (y2 - y1) / (x2 - x1),
			b2 = location.y() + 1 / k * location.x(),
			b = y1 - k * x1,
			x3 = k / (1 + k * k) * (b2 - b);
		nearestPointOfLinePort = (x3 - x1) / (x2 - x1);
	}
	return nearestPointOfLinePort;
}

QLineF PortHandler::newTransform(StatLine const &port) const
{
	qreal const x1 = port.line.x1() * (port.prop_x1 ? port.initWidth : mNode->contentsRect().width());	
	qreal const y1 = port.line.y1() * (port.prop_y1 ? port.initHeight : mNode->contentsRect().height());

	qreal const x2 = port.line.x2() * (port.prop_x2 ? port.initWidth : mNode->contentsRect().width());	
	qreal const y2 = port.line.y2() * (port.prop_y2 ? port.initHeight : mNode->contentsRect().height());

	return QLineF(x1, y1, x2, y2);
}

QPointF PortHandler::newTransform(StatPoint const &port) const
{
	qreal const x = port.point.x() * (port.prop_x ? port.initWidth : mNode->contentsRect().width());
	qreal const y = port.point.y() * (port.prop_y ? port.initHeight : mNode->contentsRect().height());

	return QPointF(x, y);
}

void PortHandler::connectTemporaryRemovedLinksToPort(IdList const &temporaryRemovedLinks, QString const &direction)
{
	foreach (Id const &edgeId, temporaryRemovedLinks) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(static_cast<EditorViewScene *>(mNode->scene())->getElem(edgeId));
		if (edge == NULL) {
			continue;
		}

		if (direction == "from") {
			QPointF startPos = edge->mapFromItem(mNode, getNearestPort(edge->line().first()));
			edge->placeStartTo(startPos);
		} else {
			QPointF endPos = edge->mapFromItem(mNode, getNearestPort(edge->line().last()));
			edge->placeEndTo(endPos);
		}
		edge->connectToPort();
	}
}

QPointF const PortHandler::getPortPos(qreal id) const
{
	if (id < 0.0) {
		return QPointF(0, 0);
	}

	int iid = portId(id);
	if (id < mPointPorts.size()) {
		return newTransform(mPointPorts[iid]);
	}

	if (id < mPointPorts.size() + mLinePorts.size()) {
		return newTransform(mLinePorts.at(iid - mPointPorts.size())).pointAt(id - 1.0 * iid);
	} else {
		return QPointF(0, 0);
	}
}

int PortHandler::portId(qreal id)
{
	return qFloor(id);
}

QPointF const PortHandler::getNearestPort(QPointF const &location) const
{
	QPointF min;

	if (mPointPorts.size() > 0) {
		QPointF const pointPort = newTransform(mPointPorts[0]);
		min.setX(pointPort.x() + mNode->boundingRect().left());
		min.setY(pointPort.y() + mNode->boundingRect().top());

		// Not const reference because port value is used for calculation.
		foreach (StatPoint port, mPointPorts) {
			QPointF const pointPort = newTransform(port);
			port.point.setX(pointPort.x() + mNode->boundingRect().left());
			port.point.setY(pointPort.y() + mNode->boundingRect().top());
			if (QLineF(port.point, location).length() < QLineF(min, location).length()) {
				min = port.point;
			}
		}
		return min;
	}

	if (mLinePorts.size() > 0) {
		int num = 0;
		foreach (StatLine const &line, mLinePorts) {
			qreal k = qMin(qMax(0., getNearestPointOfLinePort(num, location)), 0.9999);
			QLineF const sceneLine = newTransform(line);
			QPointF const port = sceneLine.pointAt(k);

			if (QLineF(port, location).length() < QLineF(min, location).length() || num == 0) {
				min = port;
			}
			num++;
		}

		return min;
	}

	return location;
}

qreal PortHandler::getPortId(QPointF const &location) const
{
	int pointPortNumber = 0;
	foreach (StatPoint const &pointPort, mPointPorts) {
		if (QRectF(newTransform(pointPort) - QPointF(kvadratik, kvadratik),
					QSizeF(kvadratik * 2, kvadratik * 2)).contains(location))
		{
			return 1.0 * pointPortNumber;
		}
		pointPortNumber++;
	}

	int linePortNumber = 0;
	foreach (StatLine const &linePort, mLinePorts) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik - 5);

		QPainterPath path;
		path.moveTo(newTransform(linePort).p1());
		path.lineTo(newTransform(linePort).p2());

		path = ps.createStroke(path);
		if (path.contains(location)) {
			return 1.0 * (linePortNumber + mPointPorts.size())
				+ qMin(0.9999
						, QLineF(QLineF(newTransform(linePort)).p1(), location).length()
							/ newTransform(linePort).length()
				);
		}
		linePortNumber++;
	}

	qreal minDistance = 0;
	int numMinDistance = -1;
	if (mPointPorts.size() != 0) {
		numMinDistance = 0;
		minDistance = distanceFromPointPort(0, location);
		for (int i = 1; i < mPointPorts.size(); i++) {
			qreal currentDistance = distanceFromPointPort(i, location);
			if (currentDistance < minDistance) {
				numMinDistance = i;
				minDistance = currentDistance;
			}
		}
	}

	if (mLinePorts.size() == 0) {
		if (numMinDistance >= 0) {
			return 1.0 * numMinDistance;
		}
	}
	
	bool linePort = false;
	if (numMinDistance == -1) {
		numMinDistance = 0;
		minDistance = minDistanceFromLinePort(0, location);
		linePort = true;
	}
	for (int i = 0; i < mLinePorts.size(); i++) {
		qreal currentDistance = minDistanceFromLinePort(i, location);
		if (currentDistance < minDistance) {
			numMinDistance = i;
			minDistance = currentDistance;
			linePort = true;
		}
	}
	if (linePort) {
		qreal nearestPointOfLinePort = getNearestPointOfLinePort(numMinDistance, location);
		if (nearestPointOfLinePort < 0) {
			nearestPointOfLinePort = 0;
		} else if (nearestPointOfLinePort > 0.9999) {
			nearestPointOfLinePort = 0.9999;
		}
		return 1.0 * (numMinDistance + nearestPointOfLinePort + mPointPorts.size());
	} else {
		return 1.0 * numMinDistance;
	}
	
	return -1.0;
}

void PortHandler::connectLinksToPorts()
{
	QList<QGraphicsItem *> const items = mNode->scene()->items(mNode->scenePos());
	foreach (QGraphicsItem *item, items) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(item);
		if (edge) {
			edge->connectToPort();
			return;
		}
	}
}

void PortHandler::checkConnectionsToPort()
{
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksFrom(mNode->id()), "from");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksTo(mNode->id()), "to");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksNone(mNode->id()), QString());
	mGraphicalAssistApi->removeTemporaryRemovedLinks(mNode->id());

	// i have no idea what this method does, but it is called when the element
	// is dropped on scene. so i'll just leave this code here for now.
	connectLinksToPorts();
}

void PortHandler::arrangeLinearPorts() {
	//qDebug() << "linear ports on" << uuid().toString();
	int lpId = mPointPorts.size(); //point ports before linear
	foreach (StatLine const &line, mLinePorts) {
		//sort first by slope, then by current portId
		QMap<QPair<qreal, qreal>, EdgeElement*> sortedEdges;
		QLineF portLine = line;
		qreal dx = portLine.dx();
		qreal dy = portLine.dy();
		foreach (EdgeElement* edge, mNode->edgeList()) {
			if (portId(edge->portIdOn(mNode)) == lpId) {
				QPointF conn = edge->connectionPoint(mNode);
				QPointF next = edge->nextFrom(mNode);
				qreal x1 = conn.x();
				qreal y1 = conn.y();
				qreal x2 = next.x();
				qreal y2 = next.y();
				qreal len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				qreal scalarProduct = ((x2 - x1) * dx + (y2 - y1) * dy) / len;
				sortedEdges.insertMulti(qMakePair(edge->portIdOn(mNode), scalarProduct), edge);
				//qDebug() << "+" << edge->uuid().toString() <<"pr=" <<scalarProduct << "; p=" << edge->portIdOn(mNode);
				//qDebug("'--> vector: (%g, %g)", (x2-x1)/len, (y2-y1)/len);
				//qDebug() << "'------> because " << (QVariant)conn << "->" << (QVariant)next;
			}
		}

		//by now, edges of this port are sorted by their optimal slope.
		int const n = sortedEdges.size();
		int i = 0;
		foreach (EdgeElement* edge, sortedEdges) {
			qreal newId = lpId + (1.0 + i++) / (n + 1);
			//qDebug() << "-" << edge->uuid().toString() << newId;
			edge->moveConnection(mNode, newId);
		}

		lpId++; //next linear port.

	}
}

void PortHandler::setGraphicalAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
}
