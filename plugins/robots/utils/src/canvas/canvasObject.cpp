/* Copyright 2015 CyberTech Labs Ltd.
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

#include "utils/canvas/canvasObject.h"

#include <QtGui/QPainter>

using namespace utils;

CanvasObject::CanvasObject(QObject *parent)
	: QObject(parent)
	, mPen(Qt::black, 1)
{
}

CanvasObject::CanvasObject(const QColor &color, int thickness, QObject *parent)
	: QObject(parent)
	, mPen(color, thickness)
{
}

QColor CanvasObject::color() const
{
	return mPen.color();
}

void CanvasObject::setColor(const QColor &color)
{
	mPen.setColor(color);
}

int CanvasObject::thickness() const
{
	return mPen.width();
}

void CanvasObject::setThickness(int size)
{
	mPen.setWidth(size);
}

void CanvasObject::paint(QPainter *painter)
{
	painter->setPen(mPen);
}
