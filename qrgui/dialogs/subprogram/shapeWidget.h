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

namespace qReal {
namespace gui {

/// Displays an SDF shape.
class ShapeWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ShapeWidget(int index, QWidget *parent = nullptr);

	/// Returns index of this widget in the list of available shapes.
	int index() const;

	/// Returns the shape XML drawn by this widget.
	QString shape() const;

	/// Sets shape XML drawn by this widget.
	void setShape(const QString &shape);

	/// Redraws this widget with the graphical effect of selection.
	void addSelection();

	/// Redraws this widget without the graphical effect of selection.
	void removeSelection();

signals:
	/// Emitted when user clicks this widget.
	void clicked();

private:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

	qReal::SdfRenderer mRenderer;
	QString mShape;
	bool mSelected;
	int mIndex;
};

}
}
