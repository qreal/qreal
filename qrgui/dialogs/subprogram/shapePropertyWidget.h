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

class ShapePropertyWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ShapePropertyWidget(QWidget *parent = 0);
	void initShapes(const QStringList &shapes, const QString &currentShape, bool backround);
	QString getSelectedShape();

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void shapeClicked();

private:
	void addShape(int &index, qreal &x, const QString &shape, const QString &currentShape, bool &findedCurrentShape);

	qreal mWidthOfGrid;
	graphicsUtils::GridDrawer mGridDrawer;
	QList<ShapeWidget *> mShapes;
	int mSelectedShapeIndex;
};

}
}
