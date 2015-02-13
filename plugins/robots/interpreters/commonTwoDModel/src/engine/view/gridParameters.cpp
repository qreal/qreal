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
