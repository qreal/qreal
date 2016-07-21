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

#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPainterPath>
#include <QtGui/QPolygon>
#include <QtWidgets/QGraphicsLineItem>
#include <QtXml/QDomDocument>

#include "twoDModel/twoDModelDeclSpec.h"

class QGraphicsItem;

namespace twoDModel {

namespace items {
class WallItem;
class ColorFieldItem;
class RegionItem;
}

namespace model {

class TWO_D_MODEL_EXPORT WorldModel : public QObject
{
	Q_OBJECT

public:
	WorldModel();

	/// Measures the distance between robot and wall
	int sonarReading(const QPointF &position, qreal direction) const;

	/// Returns area which is seen by sonar sensor.
	QPainterPath sonarScanningRegion(const QPointF &position, qreal direction, int range = 255) const;

	/// Returns area which is seen by sonar sensor with zero rotation.
	QPainterPath sonarScanningRegion(const QPointF &position, int range = 255) const;

	/// Checks if the given path intersects some wall.
	bool checkCollision(const QPainterPath &path) const;

	/// Returns a list of walls in the world model.
	const QList<items::WallItem *> &walls() const;

	/// Returns a list of color field items in the world model.
	const QList<items::ColorFieldItem *> &colorFields() const;

	/// Returns a list of trace items on the floor.
	const QList<QGraphicsLineItem *> &trace() const;

	int wallsCount() const;
	items::WallItem *wallAt(int index) const;
	void addWall(items::WallItem *wall);
	void removeWall(items::WallItem *wall);

	void addColorField(items::ColorFieldItem *colorField);
	void removeColorField(items::ColorFieldItem *colorField);

	void clear();

	/// Appends one more segment of the given to the robot`s trace.
	void appendRobotTrace(const QPen &pen, const QPointF &begin, const QPointF &end);

	/// Removes all the segments from the current robot`s trace.
	void clearRobotTrace();

	/// Saves world to XML.
	QDomElement serialize(QDomDocument &document) const;
	void deserialize(const QDomElement &element);

	/// Searches on the scene item with the given id. Returns nullptr if not found.
	QGraphicsObject *findId(const QString &id) const;

signals:
	/// Emitted each time when model is appended with some new wall.
	void wallAdded(items::WallItem *item);

	/// Emitted each time when model is appended with some new color field item.
	void colorItemAdded(items::ColorFieldItem *item);

	/// Emitted each time when model is appended with some new item.
	void regionItemAdded(items::RegionItem *item);

	/// Emitted each time when model is appended with some new item.
	void traceItemAdded(QGraphicsLineItem *item);

	/// Emitted each time when some item was removed from the 2D model world.
	void itemRemoved(QGraphicsItem *item);

	/// Emitted when robot trace is non-empty any more or was cleared from the floor.
	void robotTraceAppearedOrDisappeared(bool appeared);

private:
	/// Returns true if ray intersects some wall.
	bool checkSonarDistance(const int distance, const QPointF &position
			, const qreal direction, const QPainterPath &wallPath) const;
	QPainterPath buildWallPath() const;

	QList<items::WallItem *> mWalls;
	QList<items::ColorFieldItem *> mColorFields;
	QList<QGraphicsLineItem *> mRobotTrace;
	QList<items::RegionItem *> mRegions;
};

}
}

Q_DECLARE_METATYPE(QGraphicsLineItem *)
