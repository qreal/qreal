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

#pragma once

#include <QtCore/QLineF>
#include <QtCore/QString>

namespace qReal {

static const QString NON_TYPED = "NonTyped";

/// Provides an information about the point port.
/// Point port is a point on node element where edges (one or many) can be connected.
struct PointPortInfo
{
public:
	PointPortInfo(const QPointF &point, bool scalesX, bool scalesY, int initWidth, int initHeight, const QString &type)
		: point(point), scalesX(scalesX), scalesY(scalesY), initWidth(initWidth), initHeight(initHeight), type(type)
	{
	}

	/// Coordinates of port. Both x and y are float numbers from 0 to 1 that specify position on element.
	QPointF point;

	/// If true port will float on parent`s x-axis when parent is resized.
	bool scalesX;

	/// If true port will float on parent`s y-axis when parent is resized.
	bool scalesY;

	/// If scalesX is true then this value will be used for port position scaling on parent`s x-axis:
	///  x = initWidth * point.x().
	int initWidth;

	/// If scalesY is true then this value will be used for port position scaling on parent`s y-axis:
	/// y = initHeight * point.y().
	int initHeight;

	/// Type of the port. Filters out edges that can be connected to this port.
	QString type;
};

/// Provides an information about the line port.
/// Line port is a segment on the node element where edges (one or many) can be connected.
struct LinePortInfo
{
public:
	LinePortInfo(const QLineF &line, bool scalesX1, bool scalesY1, bool scalesX2, bool scalesY2
			, int initWidth, int initHeight, const QString &type)
		: line(line), scalesX1(scalesX1), scalesY1(scalesY1), scalesX2(scalesX2), scalesY2(scalesY2)
		, initWidth(initWidth), initHeight(initHeight), type(type)
	{
	}

	/// Coordinates of line port`s begin and end. Both x and y of each end are float numbers from 0 to 1
	/// that specify position on element relatively to its sizes (or initWidth and initHeight).
	QLineF line;

	/// If true port will float on parent`s x-axis when parent is resized.
	bool scalesX1;

	/// If true port will float on parent`s y-axis when parent is resized.
	bool scalesY1;

	/// If true port will float on parent`s x-axis when parent is resized.
	bool scalesX2;

	/// If true port will float on parent`s y-axis when parent is resized.
	bool scalesY2;

	/// If scalesX is true then this value will be used for port position scaling on parent`s x-axis:
	///  x1 = initWidth * line.x1(), x2 = initWidth * line.x2().
	int initWidth;

	/// If scalesY is true then this value will be used for port position scaling on parent`s y-axis:
	/// y = initHeight * point.y().
	int initHeight;

	/// Type of the port. Filters out edges that can be connected to this port.
	QString type;
};

}
