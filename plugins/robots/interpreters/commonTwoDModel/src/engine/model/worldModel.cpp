#include <QtGui/QTransform>
#include <QtCore/QStringList>

#include "constants.h"
#include "worldModel.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"

using namespace twoDModel;
using namespace model;

#ifdef D2_MODEL_FRAMES_DEBUG
#include <QtWidgets/QGraphicsPathItem>
QGraphicsPathItem *debugPath = nullptr;
#endif

WorldModel::WorldModel()
{
}

int WorldModel::sonarReading(QPointF const &position, qreal direction) const
{
	int maxSonarRangeCms = 255;
	int minSonarRangeCms = 0;
	int currentRangeInCm = (minSonarRangeCms + maxSonarRangeCms) / 2;

	QPainterPath const wallPath = buildWallPath();
	if (!checkSonarDistance(maxSonarRangeCms, position, direction, wallPath)) {
		return maxSonarRangeCms;
	}

	for ( ; minSonarRangeCms < maxSonarRangeCms;
			currentRangeInCm = (minSonarRangeCms + maxSonarRangeCms) / 2)
	{
		if (checkSonarDistance(currentRangeInCm, position, direction, wallPath)) {
			maxSonarRangeCms = currentRangeInCm;
		} else {
			minSonarRangeCms = currentRangeInCm + 1;
		}
	}

	return currentRangeInCm;
}

bool WorldModel::checkSonarDistance(int const distance, QPointF const &position
		, qreal const direction, QPainterPath const &wallPath) const
{
	QPainterPath const rayPath = sonarScanningRegion(position, direction, distance);
	return rayPath.intersects(wallPath);
}

QPainterPath WorldModel::sonarScanningRegion(QPointF const &position, int range) const
{
	return sonarScanningRegion(position, 0, range);
}

QPainterPath WorldModel::sonarScanningRegion(QPointF const &position, qreal direction, int range) const
{
	qreal const rayWidthDegrees = 10.0;
	qreal const rangeInPixels = range * pixelsInCm;

	QPainterPath rayPath;
	rayPath.arcTo(QRect(-rangeInPixels, -rangeInPixels
			, 2 * rangeInPixels, 2 * rangeInPixels)
			, -direction - rayWidthDegrees, 2 * rayWidthDegrees);
	rayPath.closeSubpath();
	QTransform const sensorPositionTransform = QTransform().translate(position.x(), position.y());
	return sensorPositionTransform.map(rayPath);
}

bool WorldModel::checkCollision(QPainterPath const &path) const
{
#ifdef D2_MODEL_FRAMES_DEBUG
	delete debugPath;
	QPainterPath commonPath = buildWallPath();
	commonPath.addPath(path);
	debugPath = new QGraphicsPathItem(commonPath);
	debugPath->setBrush(Qt::red);
	debugPath->setPen(QPen(QColor(Qt::blue)));
	debugPath->setZValue(100);

	QGraphicsScene * const scene = mWalls.isEmpty()
			? (mColorFields.isEmpty() ? nullptr : mColorFields[0]->scene())
			: mWalls[0]->scene();
	if (scene) {
		scene->addItem(debugPath);
		scene->update();
	}
#endif

	return buildWallPath().intersects(path);
}

QList<items::WallItem *> const &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(items::WallItem *wall)
{
	mWalls.append(wall);
	emit wallAdded(wall);
}

void WorldModel::removeWall(items::WallItem *wall)
{
	mWalls.removeOne(wall);
	emit itemRemoved(wall);
}

QList<items::ColorFieldItem *> const &WorldModel::colorFields() const
{
	return mColorFields;
}

int WorldModel::wallsCount() const
{
	return mWalls.count();
}

items::WallItem *WorldModel::wallAt(int index) const
{
	return mWalls[index];
}

void WorldModel::addColorField(items::ColorFieldItem *colorField)
{
	mColorFields.append(colorField);
	emit colorItemAdded(colorField);
}

void WorldModel::removeColorField(items::ColorFieldItem *colorField)
{
	mColorFields.removeOne(colorField);
	emit itemRemoved(colorField);
}

void WorldModel::clear()
{
	while (!mWalls.isEmpty()) {
		removeWall(mWalls.last());
	}

	while (!mColorFields.isEmpty()) {
		removeColorField(mColorFields.last());
	}

	clearRobotTrace();
}

void WorldModel::appendRobotTrace(QPen const &pen, QPointF const &begin, QPointF const &end)
{
	if (pen.color() == QColor(Qt::transparent)) {
		return;
	}

	QGraphicsLineItem * const traceItem = new QGraphicsLineItem(QLineF(begin, end));
	traceItem->setPen(pen);

	if (mRobotTrace.isEmpty()) {
		emit robotTraceAppearedOrDisappeared(true);
	}

	mRobotTrace << traceItem;
	emit otherItemAdded(traceItem);
}

void WorldModel::clearRobotTrace()
{
	while (!mRobotTrace.isEmpty()) {
		QGraphicsLineItem * const toRemove = mRobotTrace.first();
		mRobotTrace.removeOne(toRemove);
		emit itemRemoved(toRemove);
	}

	emit robotTraceAppearedOrDisappeared(false);
}

QPainterPath WorldModel::buildWallPath() const
{
	/// @todo Maintain a cache for this.
	QPainterPath wallPath;

	for (items::WallItem *wall : mWalls) {
		wallPath.addPath(wall->path());
	}

	return wallPath;
}

QDomElement WorldModel::serialize(QDomDocument &document, QPointF const &topLeftPicture) const
{
	QDomElement result = document.createElement("world");

	QDomElement trace = document.createElement("trace");
	result.appendChild(trace);
	for (QGraphicsLineItem *line : mRobotTrace) {
		QDomElement traceSegment = document.createElement("segment");
		traceSegment.setAttribute("x1", line->line().x1());
		traceSegment.setAttribute("x2", line->line().x2());
		traceSegment.setAttribute("y1", line->line().y1());
		traceSegment.setAttribute("y2", line->line().y2());
		traceSegment.setAttribute("color", line->pen().color().name());
		traceSegment.setAttribute("width", line->pen().width());
		trace.appendChild(traceSegment);
	}

	QDomElement walls = document.createElement("walls");
	result.appendChild(walls);
	for (items::WallItem * const wall : mWalls) {
		QDomElement wallNode = wall->serialize(document, topLeftPicture.toPoint());
		walls.appendChild(wallNode);
	}

	QDomElement colorFields = document.createElement("colorFields");
	result.appendChild(colorFields);
	for (items::ColorFieldItem *colorField : mColorFields) {
		QDomElement colorFiedlNode = colorField->serialize(document, topLeftPicture.toPoint());
		colorFields.appendChild(colorFiedlNode);
	}

	return result;
}

void WorldModel::deserialize(QDomElement const &element)
{
	if (element.isNull()) {
		/// @todo Report error
		return;
	}

	clear();

	for (QDomElement traceNode = element.firstChildElement("trace"); !traceNode.isNull();
			traceNode = traceNode.nextSiblingElement("trace")) {
		for (QDomElement segmentNode = traceNode.firstChildElement("segment"); !segmentNode.isNull();
				segmentNode = segmentNode.nextSiblingElement("segment")) {
			QPointF const from(segmentNode.attribute("x1").toDouble(), segmentNode.attribute("y1").toDouble());
			QPointF const to(segmentNode.attribute("x2").toDouble(), segmentNode.attribute("y2").toDouble());
			QPen pen;
			pen.setColor(QColor(segmentNode.attribute("color")));
			pen.setWidth(segmentNode.attribute("width").toInt());
			appendRobotTrace(pen, from, to);
		}
	}

	for (QDomElement wallsNode = element.firstChildElement("walls"); !wallsNode.isNull();
			wallsNode = wallsNode.nextSiblingElement("walls")) {
		for (QDomElement wallNode = wallsNode.firstChildElement("wall"); !wallNode.isNull();
				wallNode = wallNode.nextSiblingElement("wall")) {
			items::WallItem *wall = new items::WallItem(QPointF(0, 0), QPointF(0, 0));
			wall->deserialize(wallNode);
			addWall(wall);
		}
	}

	for (QDomElement colorFieldsNode = element.firstChildElement("colorFields"); !colorFieldsNode.isNull();
			colorFieldsNode = colorFieldsNode.nextSiblingElement("colorFields")) {
		for (QDomElement ellipseNode = colorFieldsNode.firstChildElement("ellipse"); !ellipseNode.isNull();
				ellipseNode = ellipseNode.nextSiblingElement("ellipse")) {
			items::EllipseItem *ellipseItem = new items::EllipseItem(QPointF(0, 0), QPointF(0, 0));
			ellipseItem->deserialize(ellipseNode);
			addColorField(ellipseItem);
		}

		for (QDomElement lineNode = colorFieldsNode.firstChildElement("line"); !lineNode.isNull();
				lineNode = lineNode.nextSiblingElement("line")) {
			items::LineItem* lineItem = new items::LineItem(QPointF(0, 0), QPointF(0, 0));
			lineItem->deserialize(lineNode);
			addColorField(lineItem);
		}

		for (QDomElement stylusNode = colorFieldsNode.firstChildElement("stylus"); !stylusNode.isNull();
				stylusNode = stylusNode.nextSiblingElement("stylus")) {
			items::StylusItem *stylusItem = new items::StylusItem(0, 0);
			stylusItem->deserialize(stylusNode);
			addColorField(stylusItem);
		}
	}
}
