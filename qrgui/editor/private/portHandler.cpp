/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "portHandler.h"

#include <QtCore/qmath.h>
#include <QtGui/QTransform>

#include "editor/editorViewScene.h"
#include "editor/nodeElement.h"

using namespace qReal::gui::editor;

const qreal PortHandler::mMaximumFractionPartValue = 0.9999;

/// Value for determing ID of nonexistent port.
const qreal nonexistentPortId = -1; // just smth negative

PortHandler::PortHandler(NodeElement *node
		, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
		, const QList<StatPoint *> &pointPorts
		, const QList<StatLine *> &linePorts
		, const QList<StatCircular *> &circularPorts)
	: mNode(node)
	, mGraphicalAssistApi(graphicalAssistApi)
	, mPointPorts(pointPorts)
	, mLinePorts(linePorts)
	, mCircularPorts(circularPorts)
{
}

PortHandler::~PortHandler()
{
	qDeleteAll(mPointPorts);
	qDeleteAll(mLinePorts);
	qDeleteAll(mCircularPorts);
}

qreal PortHandler::minDistanceFromLinePort(int linePortNumber, const QPointF &location) const
{
	const QLineF linePort = transformPortForNodeSize(mLinePorts[linePortNumber]);

	// Triangle side values.
	const qreal a = linePort.length();
	const qreal b = QLineF(linePort.p1(), location).length();
	const qreal c = QLineF(linePort.p2(), location).length();

	const qreal nearestPoint= nearestPointOfLinePort(linePortNumber, location);
	if ((nearestPoint < 0) || (nearestPoint > mMaximumFractionPartValue)) {
		return qMin(b, c);
	} else {
		const qreal p = (a + b + c) / 2;
		const qreal triangleSquare = sqrt(p * (p - a) * (p - b) * (p - c));
		const qreal minDistance = 2 * triangleSquare / a;
		return minDistance;
	}
}

qreal PortHandler::distanceFromPointPort(int pointPortNumber, const QPointF &location) const
{
	return QLineF(transformPortForNodeSize(mPointPorts[pointPortNumber]), location).length();
}

qreal PortHandler::nearestPointOfLinePort(int linePortNumber, const QPointF &location) const
{
	qreal nearestPointOfLinePort = 0;
	QLineF linePort = transformPortForNodeSize(mLinePorts[linePortNumber]);
	const qreal y1 = linePort.y1();
	const qreal y2 = linePort.y2();
	const qreal x1 = linePort.x1();
	const qreal x2 = linePort.x2();

	if (x1 == x2) {
		nearestPointOfLinePort = (location.y() - y1) / (y2 - y1);
	} else if (y1 == y2) {
		nearestPointOfLinePort = (location.x() - x1) / (x2 - x1);
	} else {
		const qreal k = (y2 - y1) / (x2 - x1);
		const qreal b2 = location.y() + 1 / k * location.x();
		const qreal b = y1 - k * x1;
		const qreal x3 = k / (1 + k * k) * (b2 - b);
		nearestPointOfLinePort = (x3 - x1) / (x2 - x1);
	}
	return nearestPointOfLinePort;
}

qreal PortHandler::pointByCircularPortAngle(int circularPortNumber, const QPointF &location) const
{
	StatCircular::CircularPort circularPort = transformPortForNodeSize(mCircularPorts[circularPortNumber]);
	const qreal x = circularPort.x;
	const qreal y = circularPort.y;

	if (location.x() == x) {
		if (location.y() < y) {
			return 90.0;
		} else {
			return 270.0;
		}
	}

	if (location.y() == y) {
		if (location.x() < x) {
			return 180.0;
		} else {
			return 0.0;
		}
	}

	const qreal tg = fabs((location.y() - y) / (location.x() - x));
	qreal angleInDegrees = qRadiansToDegrees(qAtan(tg));

	if ((location.x() < x) && (location.y() < y)) {
		angleInDegrees = 180.0 - angleInDegrees;
	}

	if ((location.x() < x) && (location.y() > y)) {
		angleInDegrees = 180.0 + angleInDegrees;
	}

	if ((location.x() > x) && (location.y() > y)) {
		angleInDegrees = 360.0 - angleInDegrees;
	}

	return angleInDegrees;
}

QPointF PortHandler::coordinateOfCircular(int circularPortNumber, const QPointF &location) const
{
	StatCircular::CircularPort circularPort = transformPortForNodeSize(mCircularPorts[circularPortNumber]);
	const qreal a = circularPort.x;
	const qreal b = circularPort.y;
	const qreal r = circularPort.rx;

	const qreal newX = a + r * ((location.x() - a) / sqrt(qPow((location.x() - a), 2) + qPow((location.y() - b), 2)));
	const qreal newY = b + r * ((location.y() - b) / sqrt(qPow((location.x() - a), 2) + qPow((location.y() - b), 2)));
	return {newX, newY};
}

QLineF PortHandler::transformPortForNodeSize(const StatLine * const port) const
{
	return port->transformForContents(mNode->contentsRect());
}

StatCircular::CircularPort PortHandler::transformPortForNodeSize(const StatCircular * const port) const
{
	return port->transformForContents(mNode->contentsRect());
}

QPointF PortHandler::transformPortForNodeSize(const StatPoint * const port) const
{
	return port->transformForContents(mNode->contentsRect());
}

void PortHandler::connectTemporaryRemovedLinksToPort(const IdList &temporaryRemovedLinks, const QString &direction)
{
	for (const Id &edgeId : temporaryRemovedLinks) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(
				static_cast<EditorViewScene *>(mNode->scene())->getElem(edgeId)
				);

		if (edge == nullptr) {
			continue;
		}

		if (direction == "from") {
			const QPointF startPos = edge->mapFromItem(mNode
					, nearestPort(edge->mapToScene(edge->line().first()), edge->fromPortTypes()));
			edge->placeStartTo(startPos);
		} else {
			const QPointF endPos = edge->mapFromItem(mNode
					, nearestPort(edge->mapToScene(edge->line().last()), edge->toPortTypes()));
			edge->placeEndTo(endPos);
		}

		edge->connectToPort();
	}
}

const QPointF PortHandler::portPos(qreal id) const
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
	}

	if (portNum < mPointPorts.size() + mLinePorts.size() + mCircularPorts.size()) {
		return findPointById(id);
	} else {
		return QPointF(0, 0);
	}
}

const QPointF PortHandler::findPointById(qreal id) const
{
	int portNum = portNumber(id);
	StatCircular::CircularPort circularPort =
			transformPortForNodeSize(mCircularPorts.at(portNum - mPointPorts.size() - mLinePorts.size()));
	const qreal angle = (id - qFloor(id)) * 360.0;
	// (0, 0) -> center of port
	QTransform transform;
	transform.translate(circularPort.x, circularPort.y);
	transform.rotate(360.0 - angle);
	transform.translate(circularPort.rx, 0.0);
	QPointF newPoint(transform.map(QPointF(0.0, 0.0)));
	return newPoint;
}

int PortHandler::portNumber(qreal id)
{
	return qFloor(id);
}

const QPointF PortHandler::nearestPort(const QPointF &location, const QStringList &types) const
{
	// location in scene coords, so we map it to mNode.
	const QPointF locationInLocalCoords = mNode->mapFromScene(location);

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
		const qreal positionAtLineCoef = qMin(qMax(0., nearestPointOfLinePort(linePortRes.first, locationInLocalCoords))
				, mMaximumFractionPartValue);
		const QLineF sceneLine = transformPortForNodeSize(mLinePorts[linePortRes.first]);
		nearestPortPoint = sceneLine.pointAt(positionAtLineCoef);
	}

	QPair<int, qreal> const circularPortRes = nearestCircularPortNumberAndDistance(locationInLocalCoords, types);
	if (circularPortRes.second >= 0 &&
		(circularPortRes.second < minDistance || minDistance < 0))
	{
		minDistance = circularPortRes.second;
		nearestPortPoint = coordinateOfCircular(circularPortRes.first, locationInLocalCoords);
	}

	if (minDistance > -0.5) {
		// Moving to scene coords.
		return nearestPortPoint;
	}

	return location;
}

qreal PortHandler::pointPortId(const QPointF &location, const QStringList &types) const
{
	for (int pointPortNumber = 0; pointPortNumber < mPointPorts.count(); pointPortNumber++) {
		const StatPoint * const pointPort = mPointPorts.at(pointPortNumber);
		if (QRectF(transformPortForNodeSize(pointPort) - QPointF(kvadratik, kvadratik)
				, QSizeF(kvadratik * 2, kvadratik * 2)).contains(location) && types.contains(pointPort->type()))
		{
			return pointPortNumber;
		}
	}

	return nonexistentPortId;
}

qreal PortHandler::linePortId(const QPointF &location, const QStringList &types) const
{
	for (int linePortNumber = 0; linePortNumber < mLinePorts.count(); linePortNumber++) {
		const StatLine * const linePort = mLinePorts.at(linePortNumber);
		if (!types.contains(linePort->type())) {
			continue;
		}

		QPainterPathStroker ps;
		ps.setWidth(kvadratik - 5);

		QPainterPath path;
		const QLineF line = transformPortForNodeSize(linePort);
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

qreal PortHandler::circularPortId(const QPointF &location, const QStringList &types) const
{
	for (int circularPortNumber = 0; circularPortNumber < mCircularPorts.count(); circularPortNumber++) {
		const StatCircular * const circularPort = mCircularPorts.at(circularPortNumber);
		if (!types.contains(circularPort->type())) {
			continue;
		}

		QPainterPathStroker ps;
		ps.setWidth(kvadratik);

		QPainterPath path;
		StatCircular::CircularPort circular = transformPortForNodeSize(circularPort);

		path.addEllipse({circular.x, circular.y}, circular.rx, circular.ry);

		path = ps.createStroke(path);
		if (path.contains(location)) {
			return circularPortNumber + mPointPorts.size() + mLinePorts.size()
				+ (pointByCircularPortAngle(circularPortNumber, location) / 360.0);
		}
	}

	return nonexistentPortId;
}

QPair<int, qreal> PortHandler::nearestPointPortNumberAndDistance(const QPointF &location
		, const QStringList &types) const
{
	qreal minDistance = -1; // just smth negative

	int minDistancePointPortNumber = -1; // just smth negative
	for (int pointPortNumber = 0; pointPortNumber < mPointPorts.size(); pointPortNumber++) {
		if (!types.contains(mPointPorts.at(pointPortNumber)->type())) {
			continue;
		}

		const qreal currentDistance = distanceFromPointPort(pointPortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistancePointPortNumber = pointPortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistancePointPortNumber, minDistance);
}

QPair<int, qreal> PortHandler::nearestLinePortNumberAndDistance(const QPointF &location, const QStringList &types) const
{
	qreal minDistance = -1; // just smth negative

	int minDistanceLinePortNumber = -1; // just smth negative
	for (int linePortNumber = 0; linePortNumber < mLinePorts.size(); linePortNumber++) {
		if (!types.contains(mLinePorts.at(linePortNumber)->type())) {
			continue;
		}

		const qreal currentDistance = minDistanceFromLinePort(linePortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistanceLinePortNumber = linePortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistanceLinePortNumber, minDistance);
}

QPair<int, qreal> PortHandler::nearestCircularPortNumberAndDistance(const QPointF &location
		, const QStringList &types) const
{
	qreal minDistance = -1; // just smth negative

	int minDistanceCircularPortNumber = -1; // just smth negative
	for (int circularPortNumber = 0; circularPortNumber < mCircularPorts.size(); ++circularPortNumber) {
		if (!types.contains(mCircularPorts.at(circularPortNumber)->type())) {
			continue;
		}

		const qreal currentDistance = minDistanceFromCircularPort(circularPortNumber, location);
		if (currentDistance < minDistance || minDistance < 0) {
			minDistanceCircularPortNumber = circularPortNumber;
			minDistance = currentDistance;
		}
	}

	return qMakePair(minDistanceCircularPortNumber, minDistance);
}

qreal PortHandler::minDistanceFromCircularPort(int circularPortNumber, const QPointF &location) const
{
	StatCircular::CircularPort circularPort = transformPortForNodeSize(mCircularPorts[circularPortNumber]);
	const qreal x = circularPort.x;
	const qreal y = circularPort.y;
	const qreal r = circularPort.rx;
	const qreal minDistance = sqrt(qPow((location.x() - x), 2) + qPow((location.y() - y), 2)) - r;
	return minDistance;
}

qreal PortHandler::portId(const QPointF &location, const QStringList &types) const
{
	if (mPointPorts.empty() && mLinePorts.empty() && mCircularPorts.empty()) {
		return nonexistentPortId;
	}

	// Finding in point port locality
	const qreal locationPointPortId = pointPortId(location, types);
	if (locationPointPortId != nonexistentPortId) {
		return locationPointPortId;
	}

	// Finding in line port locality
	const qreal locationLinePortId = linePortId(location, types);
	if (locationLinePortId != nonexistentPortId) {
		return locationLinePortId;
	}

	const qreal locationCircularPortId = circularPortId(location, types);
	if (locationCircularPortId != nonexistentPortId) {
		return locationCircularPortId;
	}

	// Nearest ports
	const QPair<int, qreal> pointPortRes = nearestPointPortNumberAndDistance(location, types);
	const QPair<int, qreal> linePortRes = nearestLinePortNumberAndDistance(location, types);
	const QPair<int, qreal> circularPortRes = nearestCircularPortNumberAndDistance(location, types);

	// Second field is distance.
	// In case it is less than 0 there is no ports of appropriate kind.
	// First field is number of port in port list of appropriate kind.
	if (!(pointPortRes.second < 0)) {
		if ((pointPortRes.second < linePortRes.second || linePortRes.second < 0)
				&& (pointPortRes.second < circularPortRes.second || circularPortRes.second < 0))
		{
			return pointPortRes.first;
		}

		return nonexistentPortId;
	} else if (!(circularPortRes.second < 0)) {
		if (circularPortRes.second < linePortRes.second || linePortRes.second < 0) {
			const qreal nearestAngle = pointByCircularPortAngle(circularPortRes.first, location) / 360.0;

			return (circularPortRes.first + mLinePorts.size() + mPointPorts.size()) // Integral part of ID.
				+ nearestAngle; // Fractional part of ID.
			// More information about ID parts in *Useful information* before class declaration.
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
	return (mPointPorts.size() + mLinePorts.size() + mCircularPorts.size());
}

void PortHandler::connectLinksToPorts()
{
	const QList<QGraphicsItem *> items = mNode->scene()->items(mNode->boundingRect().translated(mNode->pos()));
	for (QGraphicsItem * const item : items) {
		EdgeElement * const edge = dynamic_cast<EdgeElement *>(item);
		if (edge && edge->isHanging()) {
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
		const QLineF portLine = mLinePorts.at(linePortId)->transformForContents(mNode->contentsRect());
		for (EdgeElement* edge : mNode->edgeList()) {
			QPair<qreal, qreal> edgePortId = edge->portIdOn(mNode);
			qreal currentPortId = -1.0;
			if (portNumber(edgePortId.first) == linePortId) {
				currentPortId = edgePortId.first;
			}
			if (portNumber(edgePortId.second) == linePortId) {
				currentPortId = edgePortId.second;
			}

			if (currentPortId != -1.0) {
				const EdgeArrangeCriteria arrangeCriteria = edge->arrangeCriteria(mNode, portLine);
				sortedEdges.insertMulti(arrangeCriteria, edge);
			}
		}

		const int n = sortedEdges.size();
		int i = 0;
		for (EdgeElement * const edge : sortedEdges) {
			const qreal newId = linePortId + (i + 1.0) / (n + 1);
			edge->moveConnection(mNode, newId);
			i++;
		}
	}
}

void PortHandler::drawPorts(QPainter *painter, const QRectF &contents, const QStringList &types)
{
	for (const StatPoint * const pointPort : mPointPorts) {
		if (types.contains(pointPort->type())) {
			pointPort->paint(painter, contents);
		}
	}

	for (const StatLine * const linePort : mLinePorts) {
		if (types.contains(linePort->type())) {
			linePort->paint(painter, contents);
		}
	}

	for (const StatCircular * const circularPort : mCircularPorts) {
		if (types.contains(circularPort->type())) {
			circularPort->paint(painter, contents);
		}
	}
}
