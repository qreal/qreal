/* Copyright 2012-2016 CyberTech Labs Ltd., Anna Deripaska
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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/worldModel.h"
#include "twoDModel/engine/model/image.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"
#include "src/engine/items/curveItem.h"
#include "src/engine/items/rectangleItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/imageItem.h"
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
	: mXmlFactory(new QDomDocument)
	, mErrorReporter(nullptr)
{
}

void WorldModel::init(qReal::ErrorReporterInterface &errorReporter)
{
	mErrorReporter = &errorReporter;
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
	rayPath.arcTo(QRectF(-rangeInPixels, -rangeInPixels, 2 * rangeInPixels, 2 * rangeInPixels)
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

const QMap<QString, items::WallItem *> &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(items::WallItem *wall)
{
	const QString id = wall->id();
	if (mWalls.contains(id)) {
		mErrorReporter->addError(tr("Trying to add an item with a duplicate id: %1").arg(id));
		return; // probably better than having no way to delete those duplicate items on the scene
	}

	mWalls[id] = wall;
	emit wallAdded(wall);
}

void WorldModel::removeWall(items::WallItem *wall)
{
	mWalls.remove(wall->id());
	emit itemRemoved(wall);
}

const QMap<QString, items::ColorFieldItem *> &WorldModel::colorFields() const
{
	return mColorFields;
}

const QMap<QString, items::ImageItem *> &WorldModel::imageItems() const
{
	return mImages;
}

const QMap<QString, items::RegionItem *> &WorldModel::regions() const
{
	return mRegions;
}

const QList<QGraphicsLineItem *> &WorldModel::trace() const
{
	return mRobotTrace;
}

void WorldModel::addColorField(items::ColorFieldItem *colorField)
{
	const QString id = colorField->id();
	if (mColorFields.contains(id)) {
		mErrorReporter->addError(tr("Trying to add an item with a duplicate id: %1").arg(id));
		return;
	}

	mColorFields[id] = colorField;
	emit colorItemAdded(colorField);
}

void WorldModel::removeColorField(items::ColorFieldItem *colorField)
{
	mColorFields.remove(colorField->id());
	emit itemRemoved(colorField);
}

void WorldModel::addImage(items::ImageItem *image)
{
	const QString id = image->id();
	if (mImages.contains(id)) {
		mErrorReporter->addError(tr("Trying to add an item with a duplicate id: %1").arg(id));
		return;
	}

	mImages[id] = image;
	emit imageItemAdded(image);
}

void WorldModel::removeImage(items::ImageItem *image)
{
	mImages.remove(image->id());
	emit itemRemoved(image);
}

void WorldModel::clear()
{
	while (!mWalls.isEmpty()) {
		removeWall(mWalls.last());
	}

	while (!mColorFields.isEmpty()) {
		removeColorField(mColorFields.last());
	}

	while (!mImages.isEmpty()) {
		removeImage(mImages.last());
	}

	while (!mRegions.isEmpty()) {
		QGraphicsItem * const toRemove = mRegions.last();
		const QString toRemoveKey = mRegions.lastKey(); // possible fix of the crash in remove
		mRegions.remove(toRemoveKey);
		emit itemRemoved(toRemove);
	}

	clearRobotTrace();
	setBackground(Image(), QRect());
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

QRect WorldModel::deserializeRect(const QString &string) const
{
	const QStringList splittedStr = string.split(":");
	if (splittedStr.count() == 4) {
		const int x = splittedStr[0].toInt();
		const int y = splittedStr[1].toInt();
		const int w = splittedStr[2].toInt();
		const int h = splittedStr[3].toInt();
		return QRect(x, y, w, h);
	}

	return QRect();
}

void WorldModel::deserializeBackground(const QDomElement &backgroundElement)
{
	const Image image = Image::deserialize(backgroundElement);
	const QRect backgroundRect = deserializeRect(backgroundElement.attribute("backgroundRect"));
	setBackground(image, backgroundRect);
}

QDomElement WorldModel::serialize(QDomElement &parent) const
{
	QDomElement result = parent.ownerDocument().createElement("world");
	parent.appendChild(result);

	QDomElement background = parent.ownerDocument().createElement("background");
	mBackgroundImage.serialize(background);
	background.setAttribute("backgroundRect", QString("%1:%2:%3:%4").arg(
			QString::number(mBackgroundRect.x())
			, QString::number(mBackgroundRect.y())
			, QString::number(mBackgroundRect.width())
			, QString::number(mBackgroundRect.height())));
	result.appendChild(background);

	QDomElement walls = parent.ownerDocument().createElement("walls");
	result.appendChild(walls);
	for (items::WallItem * const wall : mWalls) {
		wall->serialize(walls);
	}

	QDomElement colorFields = parent.ownerDocument().createElement("colorFields");
	result.appendChild(colorFields);
	for (const items::ColorFieldItem *colorField : mColorFields) {
		colorField->serialize(colorFields);
	}

	QDomElement images = parent.ownerDocument().createElement("images");
	result.appendChild(images);
	for (items::ImageItem * const image : mImages) {
		image->serialize(parent);
	}

	QDomElement regions = parent.ownerDocument().createElement("regions");
	result.appendChild(regions);
	for (const items::RegionItem *region : mRegions) {
		QDomElement regionElement = parent.ownerDocument().createElement("region");
		region->serialize(regionElement);
		regions.appendChild(regionElement);
	}

	// Robot trace saving is disabled

	return result;
}

QDomElement WorldModel::serializeItem(const QString &id) const
{
	const graphicsUtils::AbstractItem *item = dynamic_cast<const graphicsUtils::AbstractItem *>(findId(id));
	if (!item) {
		return QDomElement();
	}

	QDomElement temporalParent = mXmlFactory->createElement("temporalParent");
	return item->serialize(temporalParent);
}

void WorldModel::deserialize(const QDomElement &element)
{
	if (element.isNull()) {
		/// @todo Report error
		return;
	}

	clear();

	deserializeBackground(element.firstChildElement("background"));

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
			createWall(wallNode);
		}
	}

	for (QDomElement colorFieldsNode = element.firstChildElement("colorFields"); !colorFieldsNode.isNull()
			; colorFieldsNode = colorFieldsNode.nextSiblingElement("colorFields")) {
		for (QDomElement elementNode = colorFieldsNode.firstChildElement(); !elementNode.isNull()
				; elementNode = elementNode.nextSiblingElement()) {
			createElement(elementNode);
		}
	}

	for (QDomElement imagesNode = element.firstChildElement("images"); !imagesNode.isNull()
			; imagesNode = imagesNode.nextSiblingElement("images")) {
		for (QDomElement imageNode = imagesNode.firstChildElement("image"); !imageNode.isNull()
				; imageNode = imageNode.nextSiblingElement("image")) {
			createImage(imageNode);
		}
	}

	for (QDomElement regionNode = element.firstChildElement("regions").firstChildElement("region")
			; !regionNode.isNull()
			; regionNode = regionNode.nextSiblingElement("region"))
	{
		createRegion(regionNode);
	}
}

QGraphicsObject *WorldModel::findId(const QString &id) const
{
	if (id.isEmpty()) {
		return nullptr;
	}

	if (mWalls.contains(id)) {
		return mWalls[id];
	}

	if (mColorFields.contains(id)) {
		return mColorFields[id];
	}

	if (mImages.contains(id)) {
		return mImages[id];
	}

	if (mRegions.contains(id)) {
		return mRegions[id];
	}

	return nullptr;
}

void WorldModel::setBackground(const Image &image, const QRect &rect)
{
	if (image != mBackgroundImage || rect != mBackgroundRect) {
		mBackgroundImage = image;
		mBackgroundRect = rect;
		emit backgroundChanged(mBackgroundImage, mBackgroundRect);
	}
}

Image &WorldModel::background()
{
	return mBackgroundImage;
}

QRect &WorldModel::backgroundRect()
{
	return mBackgroundRect;
}

void WorldModel::createElement(const QDomElement &element)
{
	if (element.tagName() == "rectangle") {
		createRectangle(element);
	} else if (element.tagName() == "ellipse") {
		createEllipse(element);
	} else if (element.tagName() == "line") {
		createLine(element);
	} else if (element.tagName() == "cubicBezier") {
		createCubicBezier(element);
	} else if (element.tagName() == "stylus") {
		createStylus(element);
	} else if (element.tagName() == "image") {
		createImage(element);
	} else if (element.tagName() == "wall") {
		createWall(element);
	} else if (element.tagName() == "region") {
		createRegion(element);
	} else if (element.tagName() == "background") {
		deserializeBackground(element);
	}
}

void WorldModel::createWall(const QDomElement &element)
{
	items::WallItem *wall = new items::WallItem(QPointF(), QPointF());
	wall->deserialize(element);
	addWall(wall);
}

void WorldModel::createLine(const QDomElement &element)
{
	items::LineItem *lineItem = new items::LineItem(QPointF(), QPointF());
	lineItem->deserialize(element);
	addColorField(lineItem);
}

void WorldModel::createRectangle(const QDomElement &element)
{
	items::RectangleItem *rectangleItem = new items::RectangleItem(QPointF(), QPointF());
	rectangleItem->deserialize(element);
	addColorField(rectangleItem);
}

void WorldModel::createEllipse(const QDomElement &element)
{
	items::EllipseItem *ellipseItem = new items::EllipseItem(QPointF(), QPointF());
	ellipseItem->deserialize(element);
	addColorField(ellipseItem);
}

void WorldModel::createCubicBezier(const QDomElement &element)
{
	items::CurveItem *curveItem = new items::CurveItem(QPointF(), QPointF());
	curveItem->deserialize(element);
	addColorField(curveItem);
}

void WorldModel::createStylus(const QDomElement &element)
{
	items::StylusItem *stylusItem = new items::StylusItem(0, 0);
	stylusItem->deserialize(element);
	addColorField(stylusItem);
}

void WorldModel::createImage(const QDomElement &element)
{
	items::ImageItem *image = new items::ImageItem(model::Image(), QRect());
	image->deserialize(element);
	addImage(image);
}

void WorldModel::createRegion(const QDomElement &element)
{
	const QString type = element.attribute("type", "ellipse").toLower();
	items::RegionItem *item = nullptr;
	if (type == "ellipse") {
		item = new items::EllipseRegion;
	} else if (type == "rectangle") {
		item = new items::RectangularRegion;
	} else if (type == "bound") {
		const QString id = element.attribute("boundItem");
		const QGraphicsObject *boundItem = findId(id);
		if (boundItem) {
			item = new items::BoundRegion(*boundItem, id);
			connect(item, &QObject::destroyed, this, [this, item]() { mRegions.remove(item->id()); });
			// Item itself will be deleted with its parent, see BoundRegion constructor.
		} /// @todo: else report error
	}

	if (item) {
		item->deserialize(element);
		mRegions[item->id()] = item;
		emit regionItemAdded(item);
	}
}

void WorldModel::removeItem(const QString &id)
{
	QGraphicsObject *item = findId(id);
	if (items::WallItem *wall = dynamic_cast<items::WallItem *>(item)) {
		removeWall(wall);
	} else if (items::ColorFieldItem *colorItem = dynamic_cast<items::ColorFieldItem *>(item)) {
		removeColorField(colorItem);
	} else if (items::ImageItem *image = dynamic_cast<items::ImageItem *>(item)) {
		removeImage(image);
	}
}
