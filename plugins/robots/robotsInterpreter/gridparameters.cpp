#include <QtWidgets/QSlider>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QString>
#include "GridParameters.h"
#include "../../../qrkernel/settingsManager.h"

GridParameters::GridParameters(QWidget *parent)
	: QGroupBox(parent)
{
	mShowGridCheckBox = new QCheckBox(this);
	mShowGridCheckBox->setText(tr("Show grid"));
	mShowGridCheckBox->setGeometry(QRect(10, 20, 70, 17));

	mCellSize = new QSlider(this);
	mCellSize->setGeometry(QRect(10, 50, 131, 19));
	mCellSize->setOrientation(Qt::Horizontal);
	mCellSize->setMinimum(50);
	mCellSize->setMaximum(200);
	mCellSize->setTickInterval(10);
	mCellSize->setEnabled(false);

	mCellSizeLabel = new QLabel(this);
	mCellSizeLabel->setText(tr("Cell size"));
	mCellSizeLabel->setGeometry(QRect(50, 40, 46, 13));

	connect(mShowGridCheckBox, SIGNAL(toggled(bool)), mCellSize, SLOT(setEnabled(bool)));
	connect(mShowGridCheckBox, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mCellSize, SIGNAL(valueChanged(int)), this, SLOT(setCellSize(int)));
	mShowGridCheckBox->setTristate(false);
	qReal::SettingsManager::setValue("2dShowGrid", false);
	qReal::SettingsManager::setValue("2dGridCellSize", 50);
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
