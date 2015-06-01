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

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QPair>

namespace qReal {
namespace gestures {

static const qreal e = 10;

static const int gridSize = 81;//gridSize = 100 recognition = 833 gestures

typedef QList<QPointF> PointVector;

typedef QList<PointVector> PathVector;

typedef QPair<QString, PathVector> Entity;

typedef QList<Entity> EntityVector;

typedef QPair<qreal, qreal> SquarePos;

typedef QList<SquarePos> Key;

typedef QPair<QString, PointVector> PathObject;

typedef QPair<QString, Key> IdealGesture;

typedef QList<IdealGesture> IdealGestures;

typedef QPair<QString, QList<QPoint> > GestureObject;

typedef QPair<PathVector, QStringList> UsersGestures;

struct KeyObjectItem
{
public:
	KeyObjectItem(const QString &object, const QList<QPoint> &path, const QString &key)
	{
		this->object = object;
		this->correctPath = path;
		this->key = key;
	}
	KeyObjectItem(){}
	QString object;
	QList<QPoint> correctPath;
	QString key;
};

typedef QList<KeyObjectItem> KeyObjectItems;

struct Object
{
public:
	Object(const QString &name, QList<QList<QPoint> > const &path)
	{
		this->name = name;
		this->path = path;
	}

	QString name;
	QList<QList<QPoint> > path;
};

typedef QList<Object> Objects;

}
}
