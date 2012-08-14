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

	// Triangle side values. 
	qreal const a = linePort.length();
	qreal const b = QLineF(linePort.p1(), location).length();
	qreal const c = QLineF(linePort.p2(), location).length();

	qreal const nearestPointOfLinePort = getNearestPointOfLinePort(linePortNumber, location);
	if ((nearestPointOfLinePort < 0) || (nearestPointOfLinePort > mMaximumFractionPartValue)) {
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
	QLineF linePort = newTransform(mLinePorts[linePortNumber]);
	qreal const y1 = linePort.y1();
	qreal const y2 = linePort.y2();
	qreal const x1 = linePort.x1();
	qreal const x2 = linePort.x2();

	if (x1 == x2) {
		nearestPointOfLinePort = (location.y() - y1) / (y2 - y1);
	} else if (y1 == y2) {
		nearestPointOfLinePort = (location.x() - x1) / (x2 - x1);
	} else {
		qreal const k = (y2 - y1) / (x2 - x1);
		qreal const b2 = location.y() + 1 / k * location.x();
		qreal const b = y1 - k * x1;
		qreal const x3 = k / (1 + k * k) * (b2 - b);
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

	int iid = portNumber(id);
	if (iid < mPointPorts.size()) {
		return newTransform(mPointPorts[iid]);
	}

	if (iid < mPointPorts.size() + mLinePorts.size()) {
		return newTransform(mLinePorts.at(iid - mPointPorts.size())).pointAt(id - qFloor(id));
	} else {
		return QPointF(0, 0);
	}
}

int PortHandler::portNumber(qreal id)
{
	return qFloor(id);
}

QPointF const PortHandler::getNearestPort(QPointF const &location) const
{
	QPointF nearestPortPoint;
	qreal minDistance = -1; // just smth negative 

	// Not const reference because port value is used for calculation.
	foreach (StatPoint port, mPointPorts) {
		QPointF const pointPort = newTransform(port);
		QPointF const translatedPort = pointPort + mNode->boundingRect().topLeft();
		qreal const currentDistance = QLineF(translatedPort, location).length();

		if (currentDistance < minDistance || minDistance < 0) {
			nearestPortPoint = translatedPort;
			minDistance = currentDistance;
		}
	}

	int num = 0;
	foreach (StatLine const &linePort, mLinePorts) {
		qreal positionAtLineCoef = qMin(qMax(0., getNearestPointOfLinePort(num, location)), mMaximumFractionPartValue);
		QLineF const sceneLine = newTransform(linePort);
		QPointF const port = sceneLine.pointAt(positionAtLineCoef);
		qreal const currentDistance = QLineF(port, location).length();

		if (currentDistance < minDistance || minDistance < 0) {
			nearestPortPoint = port;
			minDistance = currentDistance;
		}
		num++;
	}

	if (minDistance > 0) {
		return nearestPortPoint;
	}

	return location;
}

qreal PortHandler::getPointPortId(QPointF const &location) const
{
	int pointPortNumber = 0;
	foreach (StatPoint const &pointPort, mPointPorts) {
		if (QRectF(newTransform(pointPort) - QPointF(kvadratik, kvadratik),
					QSizeF(kvadratik * 2, kvadratik * 2)
			).contains(location))
		{
			return pointPortNumber;
		}
		pointPortNumber++;
	}

	return mNonexistentPortId;
}

qreal PortHandler::getLinePortId(QPointF const &location) const
{
	int linePortNumber = 0;
	foreach (StatLine const &linePort, mLinePorts) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik - 5);

		QPainterPath path;
		QLineF const line = newTransform(linePort);
		path.moveTo(line.p1());
		path.lineTo(line.p2());

		path = ps.createStroke(path);
		if (path.contains(location)) {
			return linePortNumber + mPointPorts.size()
				+ qMin(QLineF(line.p1(), location).length() / line.length()
					, mMaximumFractionPartValue);
		}
		linePortNumber++;
	}

	return mNonexistentPortId;
}

QPair<qreal, int> PortHandler::nearestPointPortNumberAndDistance(QPointF const &location) const
{
	qreal minDistance = -1; // just smth negative
	
	int pointPortNumber = 0;
	int minDistancePointPortNumber = -1; // just smth negative
	foreach (StatPoint const &pointPort, mPointPorts) {
		qreal const currentDistance = distanceFromPointPort(pointPortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistancePointPortNumber = pointPortNumber;
			minDistance = currentDistance;
		}
		pointPortNumber++;
	}

	return qMakePair(minDistance, minDistancePointPortNumber);
}

QPair<qreal, int> PortHandler::nearestLinePortNumberAndDistance(QPointF const &location) const
{
	qreal minDistance = -1; // just smth negative

	int linePortNumber = 0;
	int minDistanceLinePortNumber = -1; // just smth negative
	foreach (StatLine const &linePort, mLinePorts) {
		qreal const currentDistance = minDistanceFromLinePort(linePortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistanceLinePortNumber = linePortNumber;
			minDistance = currentDistance;
		}
		linePortNumber++;
	}

	return qMakePair(minDistance, minDistanceLinePortNumber);
}

qreal PortHandler::getPortId(QPointF const &location) const
{
	if (mPointPorts.empty() && mLinePorts.empty()) {
		return mNonexistentPortId;
	}

	// Finding in point port locality
	qreal pointPortId = getPointPortId(location);
	if (pointPortId != mNonexistentPortId) {
		return pointPortId; 
	}

	// Finding in line port locality
	qreal linePortId = getLinePortId(location);
	if (linePortId != mNonexistentPortId) {
		return linePortId;
	}

	// Nearest ports
	QPair<qreal, int> const pointPortRes = nearestPointPortNumberAndDistance(location);
	QPair<qreal, int> const linePortRes = nearestLinePortNumberAndDistance(location);

	// First field is distance.
	// In case it is less than 0 there is no ports of appropriate kind.
	// Second field is number of port in port list of appropriate kind.
	if (!(pointPortRes.first < 0)) {
		if (pointPortRes.first < linePortRes.first || linePortRes.first < 0) {
			return pointPortRes.second;
		}
	} else {
		// There is only line ports.
		// And they exist, because of first method if.
		qreal nearestPointOfLinePort = getNearestPointOfLinePort(linePortRes.second, location);

		// Moving nearestPointOfLinePort value to [0, 1).
		nearestPointOfLinePort = qMin(0., nearestPointOfLinePort);
		nearestPointOfLinePort = qMax(mMaximumFractionPartValue, nearestPointOfLinePort);

		return (linePortRes.second + mPointPorts.size()) // Integral part of ID.
			+ nearestPointOfLinePort; // Fractional part of ID.
		// More information about ID parts in *Useful information* before class declaration.
	}
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
	foreach (StatLine const &linePort, mLinePorts) {
		//sort first by slope, then by current portNumber
		QMap<QPair<qreal, qreal>, EdgeElement*> sortedEdges;
		QLineF portLine = linePort;
		qreal dx = portLine.dx();
		qreal dy = portLine.dy();
		foreach (EdgeElement* edge, mNode->edgeList()) {
			if (portNumber(edge->portIdOn(mNode)) == lpId) {
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
			qreal newId = lpId + (i + 1.0) / (n + 1);
			//qDebug() << "-" << edge->uuid().toString() << newId;
			edge->moveConnection(mNode, newId);
			i++;
		}

		lpId++; //next linear port.

	}
}

void PortHandler::setGraphicalAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
}
