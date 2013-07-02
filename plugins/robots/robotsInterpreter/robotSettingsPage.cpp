#include "robotSettingsPage.h"
#include "ui_robotSettingsPage.h"

#include "../../../qrkernel/settingsManager.h"
#include "../../../plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h"

using namespace qReal::interpreters::robots;

PreferencesRobotSettingsPage::PreferencesRobotSettingsPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesRobotSettingsPage)
		, mSensorsWidget(new details::SensorsConfigurationWidget(false))
{
	mIcon = QIcon(":/icons/preferences/robot.png");
	mUi->setupUi(this);
	connect(mUi->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mUi->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mUi->bluetoothRadioButton, SIGNAL(toggled(bool)), this, SLOT(bluetoothCommunucationToggled()));
	connect(mUi->manualComPortCheckbox, SIGNAL(toggled(bool)), this, SLOT(manualComPortCheckboxChecked(bool)));

	connect(mUi->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(refreshCommunicationGroup()));
	connect(mUi->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(refreshCommunicationGroup()));
	connect(mUi->realModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(refreshCommunicationGroup()));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	QString const defaultPortName = SettingsManager::value("bluetoothPortName").toString();

	if (ports.isEmpty()) {
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

		foreach (QextPortInfo info, ports) {
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
	}

	mUi->manualComPortCheckbox->setChecked(SettingsManager::value("manualComPortCheckboxChecked").toBool());
	mUi->enableSensorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfSensors").toBool());
	mUi->enableMotorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfMotors").toBool());
	mUi->approximationLevelSpinBox->setValue(SettingsManager::value("approximationLevel").toInt());

	QVBoxLayout *sensorsLayout = new QVBoxLayout;
	sensorsLayout->addWidget(mSensorsWidget);
	mUi->sensorsSettingsGroupBox->setLayout(sensorsLayout);

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
	initRobotModelType(typeOfRobotModel);

	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication").toString();
	initTypeOfCommunication(typeOfCommunication);

	mUi->textVisibleCheckBox->setChecked(SettingsManager::value("showTitlesForRobots").toBool());
}

PreferencesRobotSettingsPage::~PreferencesRobotSettingsPage()
{
	delete mUi;
}

void PreferencesRobotSettingsPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
		case QEvent::LanguageChange: {
			mUi->retranslateUi(this);
			mSensorsWidget->retranslateUi();
			break;
		}
		default:
			break;
	}
}

void PreferencesRobotSettingsPage::rereadNoiseSettings()
{
	mUi->enableSensorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfSensors").toBool());
	mUi->enableMotorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfMotors").toBool());
}


void PreferencesRobotSettingsPage::initRobotModelType(robotModelType::robotModelTypeEnum type)
{
	if (type == robotModelType::null) {
		mUi->nullModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else if (type == robotModelType::unreal) {
		mUi->d2ModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else {
		mUi->realModelRadioButton->setChecked(true);
	}
}

void PreferencesRobotSettingsPage::initTypeOfCommunication(QString const &type)
{
	if (type == "bluetooth") {
		mUi->bluetoothRadioButton->setChecked(true);
	} else {
		mUi->usbRadioButton->setChecked(true);
	}
}

robotModelType::robotModelTypeEnum PreferencesRobotSettingsPage::selectedRobotModel() const
{
	if (mUi->nullModelRadioButton->isChecked()) {
		return robotModelType::null;
	} else if (mUi->d2ModelRadioButton->isChecked()) {
		return robotModelType::unreal;
	} else {
		return robotModelType::real;
	}
}

int PreferencesRobotSettingsPage::approximationLevel() const
{
	return mUi->approximationLevelSpinBox->value();
}

bool PreferencesRobotSettingsPage::enableSensorNoise() const
{
	return mUi->enableSensorNoiseCheckBox->checkState() == Qt::Checked;
}

bool PreferencesRobotSettingsPage::enableMotorNoise() const
{
	return mUi->enableMotorNoiseCheckBox->checkState() == Qt::Checked;
}

bool PreferencesRobotSettingsPage::textVisible() const
{
	return mUi->textVisibleCheckBox->checkState() == Qt::Checked;
}

QString PreferencesRobotSettingsPage::selectedCommunication() const
{
	return mUi->bluetoothRadioButton->isChecked()
			? "bluetooth"
			: "usb";
}

void PreferencesRobotSettingsPage::activatedUnrealModel(bool checked)
{
	mUi->bluetoothSettingsGroupBox->setEnabled(!checked && mUi->bluetoothRadioButton->isChecked());
	mUi->noiseSensorsGroupBox->setEnabled(checked);
}

void PreferencesRobotSettingsPage::bluetoothCommunucationToggled()
{
	activatedUnrealModel(!mUi->realModelRadioButton->isChecked());
}

void PreferencesRobotSettingsPage::manualComPortCheckboxChecked(bool state)
{
	SettingsManager::setValue("manualComPortCheckboxChecked", state);
	QString const defaultPortName = SettingsManager::value("bluetoothPortName").toString();

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

QString PreferencesRobotSettingsPage::selectedPortName() const
{
	if (!isVisible()) {
		return SettingsManager::value("bluetoothPortName").toString();
	}

	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}

void PreferencesRobotSettingsPage::save()
{
	SettingsManager::setValue("robotModel", selectedRobotModel());
	SettingsManager::setValue("bluetoothPortName", selectedPortName());
	SettingsManager::setValue("valueOfCommunication", selectedCommunication());
	SettingsManager::setValue("showTitlesForRobots", textVisible());
	SettingsManager::setValue("enableNoiseOfSensors", enableSensorNoise());
	SettingsManager::setValue("enableNoiseOfMotors", enableMotorNoise());
	SettingsManager::setValue("approximationLevel", approximationLevel());
	mSensorsWidget->save();
	emit saved();
}
void PreferencesRobotSettingsPage::restoreSettings()
{
	mUi->manualComPortCheckbox->setChecked(SettingsManager::value("manualComPortCheckboxChecked").toBool());
	mUi->enableSensorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfSensors").toBool());
	mUi->enableMotorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfMotors").toBool());
	mUi->approximationLevelSpinBox->setValue(SettingsManager::value("approximationLevel").toInt());

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
	initRobotModelType(typeOfRobotModel);

	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication").toString();
	initTypeOfCommunication(typeOfCommunication);

	mUi->textVisibleCheckBox->setChecked(SettingsManager::value("showTitlesForRobots").toBool());
}

void PreferencesRobotSettingsPage::refreshPorts()
{
	mSensorsWidget->refresh();
}

void PreferencesRobotSettingsPage::refreshCommunicationGroup()
{
	bool const communicationEnabled = mUi->realModelRadioButton->isChecked();
	mUi->communicationTypeGroupBox->setEnabled(communicationEnabled);
}
