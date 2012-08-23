#include "../../view/editorViewScene.h"

#include <QtCore/qmath.h>
#include "../nodeElement.h"
#include "portHandler.h"

PortHandler::PortHandler(NodeElement *node, qReal::models::GraphicalModelAssistApi *graphicalAssistApi,
	QList<StatPoint> const &pointPorts, QList<StatLine> const &linePorts)
	: mNode(node), mGraphicalAssistApi(graphicalAssistApi)
	, mPointPorts(pointPorts), mLinePorts(linePorts)
{
}

qreal PortHandler::minDistanceFromLinePort(int linePortNumber, QPointF const &location) const
{
	QLineF const linePort = transformPortForNodeSize(mLinePorts[linePortNumber]);

	// Triangle side values. 
	qreal const a = linePort.length();
	qreal const b = QLineF(linePort.p1(), location).length();
	qreal const c = QLineF(linePort.p2(), location).length();

	qreal const nearestPoint= nearestPointOfLinePort(linePortNumber, location);
	if ((nearestPoint < 0) || (nearestPoint > mMaximumFractionPartValue)) {
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
	return QLineF(transformPortForNodeSize(mPointPorts[pointPortNumber]), location).length();
}

qreal PortHandler::nearestPointOfLinePort(int linePortNumber, QPointF const &location) const
{
	qreal nearestPointOfLinePort = 0;
	QLineF linePort = transformPortForNodeSize(mLinePorts[linePortNumber]);
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

QLineF PortHandler::transformPortForNodeSize(StatLine const &port) const
{
	qreal const x1 = port.line.x1() * (port.prop_x1 ? port.initWidth : mNode->contentsRect().width());	
	qreal const y1 = port.line.y1() * (port.prop_y1 ? port.initHeight : mNode->contentsRect().height());

	qreal const x2 = port.line.x2() * (port.prop_x2 ? port.initWidth : mNode->contentsRect().width());	
	qreal const y2 = port.line.y2() * (port.prop_y2 ? port.initHeight : mNode->contentsRect().height());

	return QLineF(x1, y1, x2, y2);
}

QPointF PortHandler::transformPortForNodeSize(StatPoint const &port) const
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
			QPointF const startPos = edge->mapFromItem(mNode, nearestPort(edge->line().first()));
			edge->placeStartTo(startPos);
		} else {
			QPointF const endPos = edge->mapFromItem(mNode, nearestPort(edge->line().last()));
			edge->placeEndTo(endPos);
		}
		edge->connectToPort();
	}
}

QPointF const PortHandler::portPos(qreal id) const
{
	if (id < 0.0) {
		return QPointF(0, 0);
	}

	// portNum - integral id part, number of port
	int portNum = portNumber(id);
	if (portNum < mPointPorts.size()) {
		return transformPortForNodeSize(mPointPorts[portNum]);
	}

	if (portNum < mPointPorts.size() + mLinePorts.size()) {
		return transformPortForNodeSize(mLinePorts.at(portNum - mPointPorts.size())).pointAt(id - qFloor(id));
	} else {
		return QPointF(0, 0);
	}
}

int PortHandler::portNumber(qreal id)
{
	return qFloor(id);
}

QPointF const PortHandler::nearestPort(QPointF const &location) const
{
	// location in scene coords, so we map it to mNode.
	QPointF const locationInLocalCoords = location - mNode->boundingRect().topLeft();

	QPointF nearestPortPoint;
	qreal minDistance = -1; // just smth negative

	// Point port observing.	
	QPair<int, qreal> const pointPortRes = nearestPointPortNumberAndDistance(locationInLocalCoords);
	if (pointPortRes.second >= 0) {
		minDistance = pointPortRes.second;
		nearestPortPoint = transformPortForNodeSize(mPointPorts[pointPortRes.first]);
	}

	// Line port observing.	
	QPair<int, qreal> const linePortRes = nearestLinePortNumberAndDistance(locationInLocalCoords);
	if (linePortRes.second >= 0 &&
		(linePortRes.second < minDistance || minDistance < 0)
	) {
		minDistance = linePortRes.second;
		qreal const positionAtLineCoef = qMin(qMax(0., nearestPointOfLinePort(linePortRes.first, locationInLocalCoords)), mMaximumFractionPartValue);
		QLineF const sceneLine = transformPortForNodeSize(mLinePorts[linePortRes.first]);
		nearestPortPoint = sceneLine.pointAt(positionAtLineCoef);
	}

	if (minDistance > 0) {
		// Moving to scene coords.
		return nearestPortPoint + mNode->boundingRect().topLeft();
	}

	return location;
}

qreal PortHandler::pointPortId(QPointF const &location) const
{
	int pointPortNumber = 0;
	foreach (StatPoint const &pointPort, mPointPorts) {
		if (QRectF(transformPortForNodeSize(pointPort) - QPointF(kvadratik, kvadratik),
					QSizeF(kvadratik * 2, kvadratik * 2)
			).contains(location))
		{
			return pointPortNumber;
		}
		pointPortNumber++;
	}

	return mNonexistentPortId;
}

qreal PortHandler::linePortId(QPointF const &location) const
{
	int linePortNumber = 0;
	foreach (StatLine const &linePort, mLinePorts) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik - 5);

		QPainterPath path;
		QLineF const line = transformPortForNodeSize(linePort);
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

QPair<int, qreal> PortHandler::nearestPointPortNumberAndDistance(QPointF const &location) const
{
	qreal minDistance = -1; // just smth negative
	
	int minDistancePointPortNumber = -1; // just smth negative
	for (int pointPortNumber = 0; pointPortNumber < mPointPorts.size(); pointPortNumber++) {
		qreal const currentDistance = distanceFromPointPort(pointPortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistancePointPortNumber = pointPortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistancePointPortNumber, minDistance);
}

QPair<int, qreal> PortHandler::nearestLinePortNumberAndDistance(QPointF const &location) const
{
	qreal minDistance = -1; // just smth negative

	int minDistanceLinePortNumber = -1; // just smth negative
	for (int linePortNumber = 0; linePortNumber < mLinePorts.size(); linePortNumber++) {
		qreal const currentDistance = minDistanceFromLinePort(linePortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistanceLinePortNumber = linePortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistanceLinePortNumber, minDistance);
}

qreal PortHandler::portId(QPointF const &location) const
{
	if (mPointPorts.empty() && mLinePorts.empty()) {
		return mNonexistentPortId;
	}

	// Finding in point port locality
	qreal locationPointPortId = pointPortId(location);
	if (locationPointPortId != mNonexistentPortId) {
		return locationPointPortId; 
	}

	// Finding in line port locality
	qreal locationLinePortId = linePortId(location);
	if (locationLinePortId != mNonexistentPortId) {
		return locationLinePortId;
	}

	// Nearest ports
	QPair<int, qreal> const pointPortRes = nearestPointPortNumberAndDistance(location);
	QPair<int, qreal> const linePortRes = nearestLinePortNumberAndDistance(location);

	// Second field is distance.
	// In case it is less than 0 there is no ports of appropriate kind.
	// First field is number of port in port list of appropriate kind.
	if (!(pointPortRes.second < 0)) {
		if (pointPortRes.second < linePortRes.second || linePortRes.second < 0) {
			return pointPortRes.first;
		}

		return mNonexistentPortId;
	} else {
		// There is only line ports.
		// And they exist, because of first method if.
		qreal nearestPoint = nearestPointOfLinePort(linePortRes.first, location);

		// Moving nearestPointOfLinePort value to [0, 1).
		nearestPoint = qMin(0., nearestPoint);
		nearestPoint = qMax(mMaximumFractionPartValue, nearestPoint);

		return (linePortRes.first + mPointPorts.size()) // Integral part of ID.
			+ nearestPoint; // Fractional part of ID.
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
	//FIXME

	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksFrom(mNode->id()), "from");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksTo(mNode->id()), "to");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi->temporaryRemovedLinksNone(mNode->id()), QString());
	mGraphicalAssistApi->removeTemporaryRemovedLinks(mNode->id());

	// i have no idea what this method does, but it is called when the element
	// is dropped on scene. so i'll just leave this code here for now.
	connectLinksToPorts();
}

void PortHandler::arrangeLinearPorts()
{
	int lpId = mPointPorts.size(); //point ports before linear
	foreach (StatLine const &linePort, mLinePorts) {
		//sort first by slope, then by current portNumber
		QMap<QPair<qreal, qreal>, EdgeElement*> sortedEdges;
		QLineF const portLine = linePort;
		qreal const dx = portLine.dx();
		qreal const dy = portLine.dy();
		foreach (EdgeElement* edge, mNode->edgeList()) {
			if (portNumber(edge->portIdOn(mNode)) == lpId) {
				QPointF const conn = edge->connectionPoint(mNode);
				QPointF const next = edge->nextFrom(mNode);
				qreal const x1 = conn.x();
				qreal const y1 = conn.y();
				qreal const x2 = next.x();
				qreal const y2 = next.y();
				qreal const len = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				qreal const scalarProduct = ((x2 - x1) * dx + (y2 - y1) * dy) / len;
				sortedEdges.insertMulti(qMakePair(edge->portIdOn(mNode), scalarProduct), edge);
			}
		}

		//by now, edges of this port are sorted by their optimal slope.
		int const n = sortedEdges.size();
		int i = 0;
		foreach (EdgeElement* edge, sortedEdges) {
			qreal const newId = lpId + (i + 1.0) / (n + 1);
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
