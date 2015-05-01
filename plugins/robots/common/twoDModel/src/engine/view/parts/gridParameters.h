/* Copyright 2012-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include <QtWidgets/QSlider>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>

namespace twoDModel {
namespace view {

class GridParameters : public QFrame
{
	Q_OBJECT

public:
	explicit GridParameters(QWidget *parent = 0);
	~GridParameters();

public slots:
	void showGrid(bool isGridEnabled);
	void setCellSize(int cellSizeValue);

signals:
	void parametersChanged();

private:
	QSlider *mCellSize;
	QCheckBox *mShowGridCheckBox;
};

}
}
