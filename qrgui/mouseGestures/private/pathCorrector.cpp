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

#include "pathCorrector.h"

#include <math.h>
#include <QtCore/QPointF>

const qreal mouseSpeed = 5;

using namespace qReal::gestures;

QList<QPoint> PathCorrector::getMousePath(QList<QPoint> const &path)
{
	QList<QPoint> mousePath;
	if (path.isEmpty())
		return mousePath;
	mousePath.append(path[0]);
	for (int i = 1; i < path.size(); i++) {
		QPoint currentPoint = path[i];
		QPoint previousPoint = path[i - 1];
		int length = currentPoint.x() - previousPoint.x();
		int width = currentPoint.y() - previousPoint.y();
		int number = static_cast<int>((sqrt(pow(static_cast<qreal>(length), 2)
											+ pow(static_cast<qreal>(width), 2))) / mouseSpeed + 1);
		for (int j = 1; j <= number; j++) {
			QPoint point(int(previousPoint.x() + length * j / number)
					, int(previousPoint.y() + width * j / number));
			mousePath.append(point);
		}
	}
	return mousePath;
}

QList<QPoint> PathCorrector::correctPath(QList<QPoint> const &path)
{
	QList<QPoint> newPath;
	if (path.isEmpty())
		return newPath;
	QPoint previousPoint = path[0];
	for (int i = 1; i < path.size(); i++) {
		QPoint currentPoint = path[i];
		qreal speed = (currentPoint - previousPoint).manhattanLength();
		qreal b = sense * (1 - 1 / exp(speedKoef * speed));
		previousPoint.setX((int) (b * currentPoint.x() + (1 - b) * previousPoint.x()));
		previousPoint.setY((int) (b * currentPoint.y() + (1 - b) * previousPoint.y()));
		newPath.push_back(previousPoint);
	}
	return newPath;
}
