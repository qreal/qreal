/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include "hintAPI.h"

#include <QtCore/QtMath>

#include <qrutils/mathUtils/math.h>

#include "scriptAPI.h"
#include "arrow.h"
#include "hintReporter.h"

using namespace qReal;
using namespace gui;
using namespace mathUtils;

HintAPI::HintAPI()
{
}

void HintAPI::addHint(const QString &message, int lifeTime, QWidget *parent)
{
	// The message will show and dispose itself.
	new HintReporter(parent, message, lifeTime);
}

void HintAPI::arrowToWidget(QWidget *target, qreal angle, int lifeTime, QWidget *parent)
{
	const int xcoord = target->mapToGlobal(QPoint(0, 0)).x();
	const int ycoord = target->mapToGlobal(QPoint(0, 0)).y();

	const int sourcePointDeviation = 50;

	const qreal angleInRads = angle * pi / 180;

	const QPoint sourcePoint(xcoord + sourcePointDeviation * qCos(angleInRads)
			, ycoord + sourcePointDeviation * qSin(angleInRads));
	const QPoint destPoint(xcoord, ycoord);

	// The arrow will show and dispose itself.
	new Arrow(sourcePoint, destPoint, lifeTime, parent);
}
