#include "nxtAdditionalPreferences.h"
#include "ui_nxtAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>

using namespace nxtKitInterpreter;
using namespace qReal;

NxtAdditionalPreferences::NxtAdditionalPreferences(QString const &realRobotName, QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::NxtAdditionalPreferences)
	, mRealRobotName(realRobotName)
{
	mUi->setupUi(this);
	connect(mUi->bluetoothRadioButton, &QRadioButton::toggled
			, mUi->bluetoothSettingsGroupBox, &QWidget::setVisible);
	connect(mUi->manualComPortCheckbox, &QCheckBox::toggled
			, this, &NxtAdditionalPreferences::manualComPortCheckboxChecked);
}

NxtAdditionalPreferences::~NxtAdditionalPreferences()
{
	delete mUi;
}

void NxtAdditionalPreferences::save()
{
	SettingsManager::setValue("NxtBluetoothPortName", selectedPortName());
	SettingsManager::setValue("NxtValueOfCommunication", selectedCommunication());
	SettingsManager::setValue("NxtManualComPortCheckboxChecked", mUi->manualComPortCheckbox->isChecked());
}

void NxtAdditionalPreferences::restoreSettings()
{
	QList<QextPortInfo> const ports = QextSerialEnumerator::getPorts();
	QString const defaultPortName = SettingsManager::value("NxtBluetoothPortName").toString();
	mUi->comPortComboBox->clear();

	foreach (QextPortInfo const &info, ports) {
		QRegExp const portNameRegexp("COM\\d+", Qt::CaseInsensitive);
		if (portNameRegexp.indexIn(info.portName) != -1) {
			QString const portName = portNameRegexp.cap();
			mUi->comPortComboBox->addItem(portName);
		}
	}
	int const defaultIndex = mUi->comPortComboBox->findText(defaultPortName);
	if (defaultIndex != -1) {
		mUi->comPortComboBox->setCurrentIndex(defaultIndex);
	}

	if (mUi->comPortComboBox->count() == 0) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->manualComPortCheckbox->hide();
		mUi->noComPortsFoundLabel->show();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mUi->comPortComboBox->show();
		mUi->comPortLabel->show();
		mUi->manualComPortCheckbox->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();
		mUi->noComPortsFoundLabel->hide();
		mUi->manualComPortCheckbox->setChecked(false);
		mUi->manualComPortCheckbox->setChecked(SettingsManager::value("NxtManualComPortCheckboxChecked").toBool());
	}

	QString const typeOfCommunication = SettingsManager::value("NxtValueOfCommunication").toString();
	if (typeOfCommunication == "bluetooth") {
		mUi->bluetoothRadioButton->setChecked(true);
	} else if (typeOfCommunication == "usb") {
		mUi->usbRadioButton->setChecked(true);
	} else {
		mUi->bluetoothRadioButton->setChecked(false);
		mUi->usbRadioButton->setChecked(false);
	}
}

void NxtAdditionalPreferences::onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface * const robotModel)
{
	bool const isReal = robotModel->name() == mRealRobotName;
	mUi->communicationTypeGroupBox->setVisible(isReal);
	mUi->bluetoothSettingsGroupBox->setVisible(mUi->bluetoothRadioButton->isChecked() && isReal);
}

void NxtAdditionalPreferences::manualComPortCheckboxChecked(bool state)
{
	QString const defaultPortName = SettingsManager::value("NxtBluetoothPortName").toString();

	if (state) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mUi->comPortComboBox->show();
		mUi->comPortLabel->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();
		mUi->noComPortsFoundLabel->hide();
	}
}

QString NxtAdditionalPreferences::selectedCommunication() const
{
	return mUi->bluetoothRadioButton->isChecked() ? "bluetooth"
			: mUi->usbRadioButton->isChecked() ? "usb" : "";
}

QString NxtAdditionalPreferences::selectedPortName() const
{
	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}
