#include "exerciseExportDialog.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore::ui;
using namespace kitBase;

ExerciseExportDialog::ExerciseExportDialog(QWidget *parent)
	: QDialog(parent)
{
	setModal(true);
	setWindowTitle(tr("Select non-modifiable parts of exercize"));

	mWorldReadOnlyCheckBox.reset(new QCheckBox(tr("2d model world is read only")));
	mSensorsReadOnlyCheckBox.reset(new QCheckBox(tr("2d model sensor positions are read only")));
	mRobotPositionReadOnlyCheckBox.reset(new QCheckBox(tr("2d model robot position is read only")));
	mRobotSetupReadOnlyCheckBox.reset(new QCheckBox(tr("Motors to wheels binding is read only")));
	mSimulationSettingsReadOnlyCheckBox.reset(new QCheckBox(tr("2d model simulation settings are read only")));

	mWorldReadOnlyCheckBox->setChecked(qReal::SettingsManager::value("worldReadOnlyDefault").toBool());
	mSensorsReadOnlyCheckBox->setChecked(qReal::SettingsManager::value("sensorsReadOnlyDefault").toBool());
	mRobotPositionReadOnlyCheckBox->setChecked(qReal::SettingsManager::value("robotPositionReadOnlyDefault").toBool());
	mRobotSetupReadOnlyCheckBox->setChecked(qReal::SettingsManager::value("robotSetupReadOnlyDefault").toBool());
	mSimulationSettingsReadOnlyCheckBox->setChecked(
			qReal::SettingsManager::value("simulationSettingsReadOnlyDefault").toBool());

	QVBoxLayout * const mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(mWorldReadOnlyCheckBox.data());
	mainLayout->addWidget(mSensorsReadOnlyCheckBox.data());
	mainLayout->addWidget(mRobotPositionReadOnlyCheckBox.data());
	mainLayout->addWidget(mRobotSetupReadOnlyCheckBox.data());
	mainLayout->addWidget(mSimulationSettingsReadOnlyCheckBox.data());

	QHBoxLayout * const buttonsLayout = new QHBoxLayout(this);

	mainLayout->addLayout(buttonsLayout);

	QPushButton * const okButton = new QPushButton(tr("Ok"), this);
	okButton->setDefault(true);
	connect(okButton, &QPushButton::clicked, [this] () { accept(); });

	QPushButton * const cancelButton = new QPushButton(tr("Cancel"), this);
	connect(cancelButton, &QPushButton::clicked, [this] () { reject(); });

	buttonsLayout->addWidget(okButton);
	buttonsLayout->addWidget(cancelButton);
}

ExerciseExportDialog::~ExerciseExportDialog()
{
	qReal::SettingsManager::setValue("worldReadOnlyDefault", mWorldReadOnlyCheckBox->isChecked());
	qReal::SettingsManager::setValue("sensorsReadOnlyDefault", mSensorsReadOnlyCheckBox->isChecked());
	qReal::SettingsManager::setValue("robotPositionReadOnlyDefault", mRobotPositionReadOnlyCheckBox->isChecked());
	qReal::SettingsManager::setValue("robotSetupReadOnlyDefault", mRobotSetupReadOnlyCheckBox->isChecked());
	qReal::SettingsManager::setValue("simulationSettingsReadOnlyDefault"
			, mSimulationSettingsReadOnlyCheckBox->isChecked());
}

ReadOnlyFlags ExerciseExportDialog::readOnlyFlags() const
{
	ReadOnlyFlags result;

	const auto setFlag = [this, &result] (ReadOnly::ReadOnlyEnum flag, const QScopedPointer<QCheckBox> &box) {
		result |= box->isChecked() ? flag : ReadOnly::ReadOnlyEnum::None;
	};

	setFlag(ReadOnly::World, mWorldReadOnlyCheckBox);
	setFlag(ReadOnly::Sensors, mSensorsReadOnlyCheckBox);
	setFlag(ReadOnly::RobotPosition, mRobotPositionReadOnlyCheckBox);
	setFlag(ReadOnly::RobotSetup, mRobotSetupReadOnlyCheckBox);
	setFlag(ReadOnly::SimulationSettings, mSimulationSettingsReadOnlyCheckBox);

	return result;
}
