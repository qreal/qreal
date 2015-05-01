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

#include <QtCore/QMap>
#include <QtWidgets/QGraphicsScene>

namespace twoDModel {

namespace model {
class WorldModel;
}

namespace view {

/// A scene that maintains a copy of the visible world for rendering some pieces of that (for sensors, for example).
class FakeScene : public QGraphicsScene
{
public:
	explicit FakeScene(const model::WorldModel &world);

	/// Renders a given piece of the scene and returns resulting image.
	QImage render(const QRectF &piece);

private:
	void addClone(QGraphicsItem * const original, QGraphicsItem * const cloned);
	void deleteItem(QGraphicsItem * const original);

	QMap<QGraphicsItem *, QGraphicsItem *> mClonedItems;
};

}
}
