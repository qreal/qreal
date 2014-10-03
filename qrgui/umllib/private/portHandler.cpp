#include "portHandler.h"

#include <QtCore/qmath.h>

#include "view/editorViewScene.h"
#include "umllib/nodeElement.h"

qreal const PortHandler::mMaximumFractionPartValue = 0.9999;

/// Value for determing ID of nonexistent port.
qreal const nonexistentPortId = -1; // just smth negative

PortHandler::PortHandler(NodeElement *node, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
		, QList<PortInterface *> const &ports)
		: mNode(node), mGraphicalAssistApi(graphicalAssistApi)
{
	foreach (PortInterface *port, ports) {
		StatPoint *point = dynamic_cast<StatPoint *>(port);
		if (point) {
			mPointPorts << point;
		} else {
			StatLine *line = dynamic_cast<StatLine *>(port);
			if (line) {
				mLinePorts << line;
			}
		}
	}
}

PortHandler::~PortHandler()
{
	foreach (StatPoint *point, mPointPorts) {
		delete point;
	}

	foreach (StatLine *line, mLinePorts) {
		delete line;
	}
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

QLineF PortHandler::transformPortForNodeSize(StatLine const * const port) const
{
	return port->transformForContents(mNode->contentsRect());
}

QPointF PortHandler::transformPortForNodeSize(StatPoint const * const port) const
{
	return port->transformForContents(mNode->contentsRect());
}

void PortHandler::connectTemporaryRemovedLinksToPort(IdList const &temporaryRemovedLinks, QString const &direction)
{
	foreach (Id const &edgeId, temporaryRemovedLinks) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(static_cast<EditorViewScene *>(mNode->scene())->getElem(edgeId));
		if (edge == NULL) {
			continue;
		}

		if (direction == "from") {
			QPointF const startPos = edge->mapFromItem(mNode, nearestPort(edge->line().first(), edge->fromPortTypes()));
			edge->placeStartTo(startPos);
		} else {
			QPointF const endPos = edge->mapFromItem(mNode, nearestPort(edge->line().last(), edge->toPortTypes()));
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

QPointF const PortHandler::nearestPort(QPointF const &location, QStringList const &types) const
{
	// location in scene coords, so we map it to mNode.
	QPointF const locationInLocalCoords = mNode->mapFromScene(location);

	QPointF nearestPortPoint;
	qreal minDistance = -1; // just smth negative

	// Point port observing.
	QPair<int, qreal> const pointPortRes = nearestPointPortNumberAndDistance(locationInLocalCoords, types);
	if (pointPortRes.second >= 0) {
		minDistance = pointPortRes.second;
		nearestPortPoint = transformPortForNodeSize(mPointPorts[pointPortRes.first]);
	}

	// Line port observing.
	QPair<int, qreal> const linePortRes = nearestLinePortNumberAndDistance(locationInLocalCoords, types);
	if (linePortRes.second >= 0 &&
		(linePortRes.second < minDistance || minDistance < 0)
	) {
		minDistance = linePortRes.second;
		qreal const positionAtLineCoef = qMin(qMax(0., nearestPointOfLinePort(linePortRes.first, locationInLocalCoords))
				, mMaximumFractionPartValue);
		QLineF const sceneLine = transformPortForNodeSize(mLinePorts[linePortRes.first]);
		nearestPortPoint = sceneLine.pointAt(positionAtLineCoef);
	}

	if (minDistance > -0.5) {
		// Moving to scene coords.
		return nearestPortPoint;
	}

	return location;
}

qreal PortHandler::pointPortId(QPointF const &location, QStringList const &types) const
{
	for (int pointPortNumber = 0; pointPortNumber < mPointPorts.count(); pointPortNumber++) {
		StatPoint const * const pointPort = mPointPorts.at(pointPortNumber);
		if (QRectF(transformPortForNodeSize(pointPort) - QPointF(kvadratik, kvadratik)
				, QSizeF(kvadratik * 2, kvadratik * 2)).contains(location) && types.contains(pointPort->type()))
		{
			return pointPortNumber;
		}
	}

	return nonexistentPortId;
}

qreal PortHandler::linePortId(QPointF const &location, QStringList const &types) const
{
	for (int linePortNumber = 0; linePortNumber < mLinePorts.count(); linePortNumber++) {
		StatLine const * const linePort = mLinePorts.at(linePortNumber);
		if (!types.contains(linePort->type())) {
			continue;
		}

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
	}

	return nonexistentPortId;
}

QPair<int, qreal> PortHandler::nearestPointPortNumberAndDistance(QPointF const &location
		, QStringList const &types) const
{
	qreal minDistance = -1; // just smth negative

	int minDistancePointPortNumber = -1; // just smth negative
	for (int pointPortNumber = 0; pointPortNumber < mPointPorts.size(); pointPortNumber++) {
		if (!types.contains(mPointPorts.at(pointPortNumber)->type())) {
			continue;
		}

		qreal const currentDistance = distanceFromPointPort(pointPortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistancePointPortNumber = pointPortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistancePointPortNumber, minDistance);
}

QPair<int, qreal> PortHandler::nearestLinePortNumberAndDistance(QPointF const &location, QStringList const &types) const
{
	qreal minDistance = -1; // just smth negative

	int minDistanceLinePortNumber = -1; // just smth negative
	for (int linePortNumber = 0; linePortNumber < mLinePorts.size(); linePortNumber++) {
		if (!types.contains(mLinePorts.at(linePortNumber)->type())) {
			continue;
		}

		qreal const currentDistance = minDistanceFromLinePort(linePortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistanceLinePortNumber = linePortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistanceLinePortNumber, minDistance);
}

qreal PortHandler::portId(QPointF const &location, QStringList const &types) const
{
	if (mPointPorts.empty() && mLinePorts.empty()) {
		return nonexistentPortId;
	}

	// Finding in point port locality
	qreal locationPointPortId = pointPortId(location, types);
	if (locationPointPortId != nonexistentPortId) {
		return locationPointPortId;
	}

	// Finding in line port locality
	qreal locationLinePortId = linePortId(location, types);
	if (locationLinePortId != nonexistentPortId) {
		return locationLinePortId;
	}

	// Nearest ports
	QPair<int, qreal> const pointPortRes = nearestPointPortNumberAndDistance(location, types);
	QPair<int, qreal> const linePortRes = nearestLinePortNumberAndDistance(location, types);

	// Second field is distance.
	// In case it is less than 0 there is no ports of appropriate kind.
	// First field is number of port in port list of appropriate kind.
	if (!(pointPortRes.second < 0)) {
		if (pointPortRes.second < linePortRes.second || linePortRes.second < 0) {
			return pointPortRes.first;
		}

		return nonexistentPortId;
	} else {
		if (linePortRes.second < 0) {
			return nonexistentPortId;
		}

		qreal nearestPoint = nearestPointOfLinePort(linePortRes.first, location);

		// Moving nearestPointOfLinePort value to [0, 1).
		nearestPoint = qMin(0., nearestPoint);
		nearestPoint = qMax(mMaximumFractionPartValue, nearestPoint);

		return (linePortRes.first + mPointPorts.size()) // Integral part of ID.
			+ nearestPoint; // Fractional part of ID.
		// More information about ID parts in *Useful information* before class declaration.
	}
}

int PortHandler::numberOfPorts() const
{
	return (mPointPorts.size() + mLinePorts.size());
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

	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi.temporaryRemovedLinksFrom(mNode->id()), "from");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi.temporaryRemovedLinksTo(mNode->id()), "to");
	connectTemporaryRemovedLinksToPort(mGraphicalAssistApi.temporaryRemovedLinksNone(mNode->id()), QString());
	mGraphicalAssistApi.removeTemporaryRemovedLinks(mNode->id());

	// i have no idea what this method does, but it is called when the element
	// is dropped on scene. so i'll just leave this code here for now.
	connectLinksToPorts();
}

void PortHandler::arrangeLinearPorts()
{
	for (int linePortId = mPointPorts.size(); linePortId < mPointPorts.size() + mLinePorts.size(); linePortId++) {
		QMap<EdgeArrangeCriteria, EdgeElement*> sortedEdges;
		QLineF const portLine = mLinePorts.at(linePortId)->transformForContents(mNode->contentsRect());
		foreach (EdgeElement* edge, mNode->edgeList()) {
			QPair<qreal, qreal> edgePortId = edge->portIdOn(mNode);
			qreal currentPortId = -1.0;
			if (portNumber(edgePortId.first) == linePortId) {
				currentPortId = edgePortId.first;
			}
			if (portNumber(edgePortId.second) == linePortId) {
				currentPortId = edgePortId.second;
			}

			if (currentPortId != -1.0) {
				EdgeArrangeCriteria const arrangeCriteria = edge->arrangeCriteria(mNode, portLine);
				sortedEdges.insertMulti(arrangeCriteria, edge);
			}
		}

		int const n = sortedEdges.size();
		int i = 0;
		foreach (EdgeElement * const edge, sortedEdges) {
			qreal const newId = linePortId + (i + 1.0) / (n + 1);
			edge->moveConnection(mNode, newId);
			i++;
		}
	}
}

void PortHandler::drawPorts(QPainter *painter, QRectF const &contents, QStringList const &types)
{
	foreach (StatPoint const * const pointPort, mPointPorts) {
		if (types.contains(pointPort->type())) {
			pointPort->paint(painter, contents);
		}
	}

	foreach (StatLine const * const linePort, mLinePorts) {
		if (types.contains(linePort->type())) {
			linePort->paint(painter, contents);
		}
	}
}
