#include <QtWidgets/QSlider>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QString>
#include "gridParameters.h"
#include "../../../qrkernel/settingsManager.h"

GridParameters::GridParameters(QWidget *parent)
	: QGroupBox(parent)
{
	QGridLayout *layout = new QGridLayout(this);

	mShowGridCheckBox = new QCheckBox(this);
	mShowGridCheckBox->setText(tr("Show grid"));

	mCellSize = new QSlider(this);
	mCellSize->setOrientation(Qt::Horizontal);
	mCellSize->setMinimum(50);
	mCellSize->setMaximum(200);
	mCellSize->setTickInterval(10);
	mCellSize->setEnabled(false);

	mCellSizeLabel = new QLabel(this);
	mCellSizeLabel->setText(tr("Cell size"));

	layout->addWidget(mShowGridCheckBox, 0, 0);
	layout->addWidget(mCellSizeLabel, 1, 0);
	layout->addWidget(mCellSize, 1, 1);
	layout->setContentsMargins(5, 5, 5, 5);

	connect(mShowGridCheckBox, SIGNAL(toggled(bool)), mCellSize, SLOT(setEnabled(bool)));
	connect(mShowGridCheckBox, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mCellSize, SIGNAL(valueChanged(int)), this, SLOT(setCellSize(int)));

	mShowGridCheckBox->setTristate(false);
	qReal::SettingsManager::setValue("2dShowGrid", false);
	qReal::SettingsManager::setValue("2dGridCellSize", 50);

	setLayout(layout);
}

GridParameters:: ~GridParameters()
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
