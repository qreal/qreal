#include "robotSettingsPage.h"
#include "ui_robotSettingsPage.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>
#include <qrutils/graphicsWatcher/sensorsGraph.h>

using namespace qReal::interpreters::robots;

PreferencesRobotSettingsPage::PreferencesRobotSettingsPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesRobotSettingsPage)
		, mSensorsWidget(new details::SensorsConfigurationWidget(false))
{
	mIcon = QIcon(":/icons/preferences/robot.png");
	mUi->setupUi(this);

	bool const enableTrik = SettingsManager::value("enableTrik", false).toBool();

	if (!enableTrik) {
		mUi->tcpRadioButton->setVisible(false);
		mUi->trikModelRadioButton->setVisible(false);
	}

	connect(mUi->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));
	connect(mUi->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));
	connect(mUi->nxtModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));
	connect(mUi->trikModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));
	connect(mUi->usbRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));
	connect(mUi->bluetoothRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));
	connect(mUi->tcpRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSomethingChanged()));

	connect(mUi->manualComPortCheckbox, SIGNAL(toggled(bool)), this, SLOT(manualComPortCheckboxChecked(bool)));

	connect(mUi->textVisibleCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(textVisibleChanged(bool)));
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
	}

	refreshValuesOnUi();

	QVBoxLayout *sensorsLayout = new QVBoxLayout;
	sensorsLayout->addWidget(mSensorsWidget);
	mUi->sensorsSettingsGroupBox->setLayout(sensorsLayout);
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

void PreferencesRobotSettingsPage::initRobotModelType(enums::robotModelType::robotModelTypeEnum type)
{
	switch (type)
	{
	case enums::robotModelType::null:
		mUi->nullModelRadioButton->setChecked(true);
		break;
	case enums::robotModelType::twoD:
		mUi->d2ModelRadioButton->setChecked(true);
		break;
	case enums::robotModelType::nxt:
		mUi->nxtModelRadioButton->setChecked(true);
		break;
	case enums::robotModelType::trik:
		mUi->trikModelRadioButton->setChecked(true);
		break;
	}

	onSomethingChanged();
}

void PreferencesRobotSettingsPage::initTypeOfCommunication(QString const &type)
{
	if (type == "bluetooth") {
		mUi->bluetoothRadioButton->setChecked(true);
	} else if (type == "usb") {
		mUi->usbRadioButton->setChecked(true);
	} else {
		mUi->tcpRadioButton->setChecked(true);
	}

	onSomethingChanged();
}

enums::robotModelType::robotModelTypeEnum PreferencesRobotSettingsPage::selectedRobotModel() const
{
	if (mUi->nullModelRadioButton->isChecked()) {
		return enums::robotModelType::null;
	} else if (mUi->d2ModelRadioButton->isChecked()) {
		return enums::robotModelType::twoD;
	} else if (mUi->trikModelRadioButton->isChecked()) {
		return enums::robotModelType::trik;
	} else {
		return enums::robotModelType::nxt;
	}
}

void PreferencesRobotSettingsPage::refreshValuesOnUi()
{
	mUi->manualComPortCheckbox->setChecked(SettingsManager::value("manualComPortCheckboxChecked").toBool());

	mUi->sensorUpdateSpinBox->setValue(
			SettingsManager::value("sensorUpdateInterval"
					, utils::sensorsGraph::SensorsGraph::readSensorDefaultInterval).toInt()
	);
	mUi->autoScalingSpinBox->setValue(
			SettingsManager::value("autoscalingInterval"
					, utils::sensorsGraph::SensorsGraph::autoscalingDefault).toInt()
	);
	mUi->textUpdaterSpinBox->setValue(
			SettingsManager::value("textUpdateInterval"
					, utils::sensorsGraph::SensorsGraph::textUpdateDefault).toInt()
	);

	enums::robotModelType::robotModelTypeEnum typeOfRobotModel =
			static_cast<enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
	initRobotModelType(typeOfRobotModel);

	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication").toString();
	initTypeOfCommunication(typeOfCommunication);

	mUi->textVisibleCheckBox->setChecked(SettingsManager::value("showTitlesForRobots").toBool());
	mUi->tcpServerLineEdit->setText(SettingsManager::value("tcpServer").toString());
	mUi->tcpPortSpinBox->setValue(SettingsManager::value("tcpPort").toInt());
	mUi->runningAfterUploadingComboBox->setCurrentIndex(SettingsManager::value("nxtFlashToolRunPolicy").toInt());
}

int PreferencesRobotSettingsPage::sensorUpdateInterval() const
{
	return mUi->sensorUpdateSpinBox->value();
}

int PreferencesRobotSettingsPage::autoscalingInterval() const
{
	return mUi->autoScalingSpinBox->value();
}

int PreferencesRobotSettingsPage::textUpdateInterval() const
{
	return mUi->textUpdaterSpinBox->value();
}

bool PreferencesRobotSettingsPage::textVisible() const
{
	return mUi->textVisibleCheckBox->checkState() == Qt::Checked;
}

void PreferencesRobotSettingsPage::changeTextVisibleOnSettingPage(bool isChecked)
{
	mUi->textVisibleCheckBox->setChecked(isChecked);
}

QString PreferencesRobotSettingsPage::selectedCommunication() const
{
	return mUi->bluetoothRadioButton->isChecked() ? "bluetooth"
			: mUi->usbRadioButton->isChecked() ? "usb" : "tcp";
}

void PreferencesRobotSettingsPage::onSomethingChanged()
{
	if (mUi->trikModelRadioButton->isChecked()) {
		mUi->tcpRadioButton->setChecked(true);
	}

	if (mUi->nxtModelRadioButton->isChecked() && mUi->tcpRadioButton->isChecked()) {
		mUi->usbRadioButton->setChecked(true);
	}

	bool const nxtModelChecked = mUi->nxtModelRadioButton->isChecked();
	bool const trikModelChecked = mUi->trikModelRadioButton->isChecked();
	bool const bluetoohChecked = mUi->bluetoothRadioButton->isChecked() && nxtModelChecked;
	bool const tcpChecked = mUi->tcpRadioButton->isChecked() && trikModelChecked;

	mUi->communicationTypeGroupBox->setEnabled(nxtModelChecked);
	mUi->bluetoothSettingsGroupBox->setVisible(bluetoohChecked);
	mUi->tcpSettingsGroupBox->setVisible(tcpChecked);

	mUi->bluetoothRadioButton->setEnabled(nxtModelChecked);
	mUi->usbRadioButton->setEnabled(nxtModelChecked);
	mUi->tcpRadioButton->setEnabled(trikModelChecked);
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
	SettingsManager::setValue("sensorUpdateInterval", sensorUpdateInterval());
	SettingsManager::setValue("autoscalingInterval", autoscalingInterval());
	SettingsManager::setValue("textUpdateInterval", textUpdateInterval());
	SettingsManager::setValue("tcpServer", mUi->tcpServerLineEdit->text());
	SettingsManager::setValue("tcpPort", mUi->tcpPortSpinBox->value());
	SettingsManager::setValue("nxtFlashToolRunPolicy", mUi->runningAfterUploadingComboBox->currentIndex());
	mSensorsWidget->save();
	emit saved();
}

void PreferencesRobotSettingsPage::restoreSettings()
{
	refreshValuesOnUi();
}

void PreferencesRobotSettingsPage::refreshPorts()
{
	mSensorsWidget->refresh();
}
