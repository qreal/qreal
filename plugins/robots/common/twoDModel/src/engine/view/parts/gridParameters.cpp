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

#include "gridParameters.h"

#include <QtWidgets/QHBoxLayout>
#include <QtCore/QString>

#include <qrkernel/settingsManager.h>

using namespace twoDModel::view;

GridParameters::GridParameters(QWidget *parent)
	: QFrame(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);

	mShowGridCheckBox = new QCheckBox;
	mShowGridCheckBox->setText(tr("Grid"));
	mShowGridCheckBox->setTristate(false);

	mCellSize = new QSlider;
	mCellSize->setOrientation(Qt::Horizontal);
	mCellSize->setMinimum(50);
	mCellSize->setMaximum(200);
	mCellSize->setTickInterval(10);
	mCellSize->setEnabled(false);

	layout->addWidget(mShowGridCheckBox);
	layout->addWidget(mCellSize);
	layout->setContentsMargins(5, 5, 5, 5);

	connect(mShowGridCheckBox, SIGNAL(toggled(bool)), mCellSize, SLOT(setEnabled(bool)));
	connect(mShowGridCheckBox, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mCellSize, SIGNAL(valueChanged(int)), this, SLOT(setCellSize(int)));

	const bool showGrid = qReal::SettingsManager::value("2dShowGrid").toBool();
	const int gridSize = qReal::SettingsManager::value("2dGridCellSize").toInt();
	mShowGridCheckBox->setChecked(showGrid);
	mCellSize->setValue(gridSize);

	setLayout(layout);

}

GridParameters::~GridParameters()
{
}

void GridParameters::showGrid(bool isGridEnabled)
{
	qReal::SettingsManager::setValue("2dShowGrid", isGridEnabled);
	emit parametersChanged();
}

void GridParameters::setCellSize(int cellSizeValue)
{
	qReal::SettingsManager::setValue("2dGridCellSize", cellSizeValue);
	emit parametersChanged();
}
