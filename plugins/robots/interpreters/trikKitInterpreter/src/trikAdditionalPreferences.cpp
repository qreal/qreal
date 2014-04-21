#include "trikAdditionalPreferences.h"
#include "ui_trikAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>

using namespace trikKitInterpreter;
using namespace qReal;

TrikAdditionalPreferences::TrikAdditionalPreferences(QString const &realRobotName, QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::TrikAdditionalPreferences)
	, mRealRobotName(realRobotName)
{
	mUi->setupUi(this);
	connect(mUi->bluetoothRadioButton, &QRadioButton::toggled
			, mUi->bluetoothSettingsGroupBox, &QWidget::setVisible);
	connect(mUi->manualComPortCheckbox, &QCheckBox::toggled
			, this, &TrikAdditionalPreferences::manualComPortCheckboxChecked);
}

TrikAdditionalPreferences::~TrikAdditionalPreferences()
{
	delete mUi;
}

void TrikAdditionalPreferences::save()
{
	SettingsManager::setValue("NxtBluetoothPortName", selectedPortName());
	SettingsManager::setValue("NxtValueOfCommunication", selectedCommunication());
	SettingsManager::setValue("NxtManualComPortCheckboxChecked", mUi->manualComPortCheckbox->isChecked());
	emit settingsChanged();
}

void TrikAdditionalPreferences::restoreSettings()
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

void TrikAdditionalPreferences::onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface * const robotModel)
{
	bool const isReal = robotModel->name() == mRealRobotName;
	mUi->communicationTypeGroupBox->setVisible(isReal);
	mUi->bluetoothSettingsGroupBox->setVisible(mUi->bluetoothRadioButton->isChecked() && isReal);
}

void TrikAdditionalPreferences::manualComPortCheckboxChecked(bool state)
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

QString TrikAdditionalPreferences::selectedCommunication() const
{
	return mUi->bluetoothRadioButton->isChecked() ? "bluetooth"
			: mUi->usbRadioButton->isChecked() ? "usb" : "";
}

QString TrikAdditionalPreferences::selectedPortName() const
{
	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}
