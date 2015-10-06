/* Copyright 2007-2015 QReal Research Group
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

#include <QtGui/QTransform>
#include <QtCore/QStringList>

#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"
#include "src/engine/items/curveItem.h"
#include "src/engine/items/rectangleItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/regions/ellipseRegion.h"
#include "src/engine/items/regions/rectangularRegion.h"
#include "src/engine/items/regions/boundRegion.h"

using namespace twoDModel;
using namespace model;

#ifdef D2_MODEL_FRAMES_DEBUG
#include <QtWidgets/QGraphicsPathItem>
QGraphicsPathItem *debugPath = nullptr;
#endif

WorldModel::WorldModel()
{
}

int WorldModel::sonarReading(const QPointF &position, qreal direction) const
{
	int maxSonarRangeCms = 255;
	int minSonarRangeCms = 0;
	int currentRangeInCm = (minSonarRangeCms + maxSonarRangeCms) / 2;

	const QPainterPath wallPath = buildWallPath();
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

bool WorldModel::checkSonarDistance(const int distance, const QPointF &position
		, const qreal direction, const QPainterPath &wallPath) const
{
	const QPainterPath rayPath = sonarScanningRegion(position, direction, distance);
	return rayPath.intersects(wallPath);
}

QPainterPath WorldModel::sonarScanningRegion(const QPointF &position, int range) const
{
	return sonarScanningRegion(position, 0, range);
}

QPainterPath WorldModel::sonarScanningRegion(const QPointF &position, qreal direction, int range) const
{
	const qreal rayWidthDegrees = 10.0;
	const qreal rangeInPixels = range * pixelsInCm;

	QPainterPath rayPath;
	rayPath.arcTo(QRect(-rangeInPixels, -rangeInPixels
			, 2 * rangeInPixels, 2 * rangeInPixels)
			, -direction - rayWidthDegrees, 2 * rayWidthDegrees);
	rayPath.closeSubpath();
	const QTransform sensorPositionTransform = QTransform().translate(position.x(), position.y());
	return sensorPositionTransform.map(rayPath);
}

bool WorldModel::checkCollision(const QPainterPath &path) const
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

const QList<QGraphicsLineItem *> &WorldModel::trace() const
{
	return mRobotTrace;
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

	while (!mRegions.isEmpty()) {
		QGraphicsItem * const toRemove = mRegions.last();
		mRegions.removeLast();
		emit itemRemoved(toRemove);
	}

	clearRobotTrace();
}

void WorldModel::appendRobotTrace(const QPen &pen, const QPointF &begin, const QPointF &end)
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
	emit traceItemAdded(traceItem);
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

QDomElement WorldModel::serialize(QDomDocument &document) const
{
	QDomElement result = document.createElement("world");

	QDomElement walls = document.createElement("walls");
	result.appendChild(walls);
	for (items::WallItem * const wall : mWalls) {
		QDomElement wallNode = document.createElement("wall");
		wall->serialize(wallNode);
		walls.appendChild(wallNode);
	}

	QDomElement colorFields = document.createElement("colorFields");
	result.appendChild(colorFields);
	for (items::ColorFieldItem * const colorField : mColorFields) {
		QDomElement colorFiedlNode = colorField->serialize(document, QPointF());
		colorFields.appendChild(colorFiedlNode);
	}

	QDomElement regions = document.createElement("regions");
	result.appendChild(regions);
	for (items::RegionItem * const region : mRegions) {
		QDomElement regionElement = document.createElement("region");
		region->serialize(regionElement);
		regions.appendChild(regionElement);
	}

	// Robot trace saving is disabled

	return result;
}

void WorldModel::deserialize(const QDomElement &element)
{
	if (element.isNull()) {
		/// @todo Report error
		return;
	}

	clear();

	for (QDomElement traceNode = element.firstChildElement("trace"); !traceNode.isNull()
			; traceNode = traceNode.nextSiblingElement("trace")) {
		for (QDomElement segmentNode = traceNode.firstChildElement("segment"); !segmentNode.isNull()
				; segmentNode = segmentNode.nextSiblingElement("segment")) {
			const QPointF from(segmentNode.attribute("x1").toDouble(), segmentNode.attribute("y1").toDouble());
			const QPointF to(segmentNode.attribute("x2").toDouble(), segmentNode.attribute("y2").toDouble());
			QPen pen;
			pen.setColor(QColor(segmentNode.attribute("color")));
			pen.setWidth(segmentNode.attribute("width").toInt());
			appendRobotTrace(pen, from, to);
		}
	}

	for (QDomElement wallsNode = element.firstChildElement("walls"); !wallsNode.isNull()
			; wallsNode = wallsNode.nextSiblingElement("walls")) {
		for (QDomElement wallNode = wallsNode.firstChildElement("wall"); !wallNode.isNull()
				; wallNode = wallNode.nextSiblingElement("wall")) {
			items::WallItem *wall = new items::WallItem(QPointF(0, 0), QPointF(0, 0));
			wall->deserialize(wallNode);
			addWall(wall);
		}
	}

	for (QDomElement colorFieldsNode = element.firstChildElement("colorFields"); !colorFieldsNode.isNull()
			; colorFieldsNode = colorFieldsNode.nextSiblingElement("colorFields")) {

		for (QDomElement rectangleNode = colorFieldsNode.firstChildElement("rectangle"); !rectangleNode.isNull()
				; rectangleNode = rectangleNode.nextSiblingElement("rectangle")) {
			items::RectangleItem *rectangleItem = new items::RectangleItem(QPointF(0, 0), QPointF(0, 0));
			rectangleItem->deserialize(rectangleNode);
			addColorField(rectangleItem);
		}

		for (QDomElement ellipseNode = colorFieldsNode.firstChildElement("ellipse"); !ellipseNode.isNull()
				; ellipseNode = ellipseNode.nextSiblingElement("ellipse")) {
			items::EllipseItem *ellipseItem = new items::EllipseItem(QPointF(0, 0), QPointF(0, 0));
			ellipseItem->deserialize(ellipseNode);
			addColorField(ellipseItem);
		}

		for (QDomElement lineNode = colorFieldsNode.firstChildElement("line"); !lineNode.isNull()
				; lineNode = lineNode.nextSiblingElement("line")) {
			items::LineItem* lineItem = new items::LineItem(QPointF(0, 0), QPointF(0, 0));
			lineItem->deserialize(lineNode);
			addColorField(lineItem);
		}

		for (QDomElement curveNode = colorFieldsNode.firstChildElement("cubicBezier"); !curveNode.isNull()
				; curveNode = curveNode.nextSiblingElement("cubicBezier")) {
			items::CurveItem *curveItem = new items::CurveItem(QPointF(0, 0), QPointF(0, 0));
			curveItem->deserialize(curveNode);
			addColorField(curveItem);
		}

		for (QDomElement stylusNode = colorFieldsNode.firstChildElement("stylus"); !stylusNode.isNull()
				; stylusNode = stylusNode.nextSiblingElement("stylus")) {
			items::StylusItem *stylusItem = new items::StylusItem(0, 0);
			stylusItem->deserialize(stylusNode);
			addColorField(stylusItem);
		}
	}

	for (QDomElement regionNode = element.firstChildElement("regions").firstChildElement("region")
			; !regionNode.isNull()
			; regionNode = regionNode.nextSiblingElement("region"))
	{
		const QString type = regionNode.attribute("type", "ellipse").toLower();
		items::RegionItem *item = nullptr;
		if (type == "ellipse") {
			item = new items::EllipseRegion;
		} else if (type == "rectangle") {
			item = new items::RectangularRegion;
		} else if (type == "bound") {
			const QString id = regionNode.attribute("boundItem");
			const QGraphicsObject *boundItem = findId(id);
			if (boundItem) {
				item = new items::BoundRegion(*boundItem, id);
				connect(item, &QObject::destroyed, this, [this, item]() { mRegions.removeAll(item); });
			} /// @todo: else report error
		}

		if (item) {
			item->deserialize(regionNode);
			mRegions.append(item);
			emit regionItemAdded(item);
		}
	}
}

QGraphicsObject *WorldModel::findId(const QString &id) const
{
	if (id.isEmpty()) {
		return nullptr;
	}

	for (items::WallItem * const wall : mWalls) {
		if (wall->id() == id) {
			return wall;
		}
	}

	for (items::ColorFieldItem * const field : mColorFields) {
		if (field->id() == id) {
			return field;
		}
	}

	for (items::RegionItem * const region : mRegions) {
		if (region->id() == id) {
			return region;
		}
	}

	return nullptr;
}
