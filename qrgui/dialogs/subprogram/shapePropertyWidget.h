/* Copyright 2015-2016 Kogutich Denis
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

#include "shapeWidget.h"

#include <qrutils/graphicsUtils/gridDrawer.h>
#include <QtWidgets/QWidget>

namespace qReal {
namespace gui {

/// Displays a set of avaliable SDF shapes and provides user a possibility to select one.
class ShapePropertyWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ShapePropertyWidget(QWidget *parent = nullptr);

	/// Configures widget instance to use the set of \a shapes.
	/// @param currentShape Currently selected shape.
	/// @param background If false then default image is not appended into the collection of displayed images.
	void initShapes(const QStringList &shapes, const QString &currentShape, bool background);

	/// Returns currently selected by user shape.
	QString selectedShape() const;

	/// Select shape at index.
	void selectShape(int index);

private slots:
	void shapeClicked();

private:
	void paintEvent(QPaintEvent *event) override;

	void addShape(int &index, qreal &x, const QString &shape, const QString &currentShape, bool &foundCurrentShape);

	qreal mWidthOfGrid;
	graphicsUtils::GridDrawer mGridDrawer;
	QList<ShapeWidget *> mShapes;
	int mSelectedShapeIndex;
};

}
}
