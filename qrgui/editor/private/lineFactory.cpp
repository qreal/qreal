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

#include "editor/private/lineFactory.h"

#include "editor/private/brokenLine.h"
#include "editor/private/squareLine.h"
#include "editor/private/curveLine.h"

using namespace qReal;
using namespace qReal::gui::editor;

LineFactory::LineFactory(EdgeElement *edge)
		: mEdge(edge)
{
}

LineHandler * LineFactory::createHandler(const linkShape::LinkShape type) const
{
	switch(type) {
	case linkShape::broken:
		return new BrokenLine(mEdge);
	case linkShape::curve:
		return new CurveLine(mEdge);
	default:
		return new SquareLine(mEdge);
	}
}

QMenu * LineFactory::shapeTypeMenu() const
{
	QMenu * const menu = new QMenu();

	QAction * const brokenLine = menu->addAction(tr("Broken"));
	connect(brokenLine, SIGNAL(triggered()), this, SLOT(setBrokenLine()));

	QAction * const squareLine = menu->addAction(tr("Square"));
	connect(squareLine, SIGNAL(triggered()), this, SLOT(setSquareLine()));

	QAction * const curveLine = menu->addAction(tr("Curve"));
	connect(curveLine, SIGNAL(triggered()), this, SLOT(setCurveLine()));

	return menu;
}

QString LineFactory::shapeToString(const linkShape::LinkShape shapeType)
{
	switch (shapeType) {
	case linkShape::broken:
		return "broken";
	case linkShape::curve:
		return "curve";
	default:
		return "square";
	}
}

linkShape::LinkShape LineFactory::stringToShape(const QString &string)
{
	if (string == "broken") {
		return linkShape::broken;
	} else if (string == "square") {
		return linkShape::square;
	} else if (string == "curve") {
		return linkShape::curve;
	} else {
		return static_cast<linkShape::LinkShape>(SettingsManager::value("LineType").toInt());
	}
}

void LineFactory::setSquareLine() const
{
	mEdge->changeShapeType(linkShape::square);
}

void LineFactory::setBrokenLine() const
{
	mEdge->changeShapeType(linkShape::broken);
}

void LineFactory::setCurveLine() const
{
	mEdge->changeShapeType(linkShape::curve);
}
