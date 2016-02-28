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

#include <QtWidgets/QWidget>
#include <plugins/pluginManager/sdfRenderer.h>


class ShapeWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ShapeWidget(QWidget *parent = 0);
	ShapeWidget(int index, QWidget *parent = 0);
	void setShape(const QString &shape);
	void addSelection();
	void removeSelection();
	int getIndex();
	QString getShape();

	signals:
	void clicked();

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);

private:
	qReal::SdfRenderer mRenderer;
	QString mShape;
	bool mSelected;
	int mIndex;
};
