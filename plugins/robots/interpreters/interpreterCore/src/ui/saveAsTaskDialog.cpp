#include "saveAsTaskDialog.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

using namespace interpreterCore::ui;
using namespace interpreterBase;

SaveAsTaskDialog::SaveAsTaskDialog(QWidget *parent)
	: QDialog(parent)
{
	setModal(true);
	setWindowTitle(tr("Select non-modifiable parts of exercize"));

	mWorldReadOnlyCheckBox.reset(new QCheckBox(tr("2d model world is read only")));
	mSensorsReadOnlyCheckBox.reset(new QCheckBox(tr("2d model sensor positions are read only")));
	mRobotPositionReadOnlyCheckBox.reset(new QCheckBox(tr("2d model robot position is read only")));
	mRobotConfigurationReadOnlyCheckBox.reset(new QCheckBox(tr("Robot configuration is read only")));
	mSimulationSettingsReadOnlyCheckBox.reset(new QCheckBox(tr("2d model simulation settings are read only")));

	QVBoxLayout * const mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(mWorldReadOnlyCheckBox.data());
	mainLayout->addWidget(mSensorsReadOnlyCheckBox.data());
	mainLayout->addWidget(mRobotPositionReadOnlyCheckBox.data());
	mainLayout->addWidget(mRobotConfigurationReadOnlyCheckBox.data());
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

SaveAsTaskDialog::~SaveAsTaskDialog()
{
}

ReadOnlyFlags SaveAsTaskDialog::readOnlyFlags() const
{
	ReadOnlyFlags result;

	const auto setFlag = [this, &result] (ReadOnly::ReadOnlyEnum flag, const QScopedPointer<QCheckBox> &box) {
		result.setFlag(flag, box->isChecked());
	};

	setFlag(ReadOnly::World, mWorldReadOnlyCheckBox);
	setFlag(ReadOnly::Sensors, mSensorsReadOnlyCheckBox);
	setFlag(ReadOnly::RobotPosition, mRobotPositionReadOnlyCheckBox);
	setFlag(ReadOnly::RobotConfiguration, mRobotConfigurationReadOnlyCheckBox);
	setFlag(ReadOnly::SimulationSettings, mSimulationSettingsReadOnlyCheckBox);

	return result;
}

