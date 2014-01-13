#include "robotsSettingsPage.h"
#include "ui_robotsSettingsPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsWatcher/sensorsGraph.h>

using namespace interpreterCore;
using namespace qReal;

RobotsSettingsPage::RobotsSettingsPage(KitPluginManager &kitPluginManager, QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesRobotSettingsPage)
		, mKitPluginManager(kitPluginManager)
		, mKitRadioButtons(new QButtonGroup(this))
{
	mIcon = QIcon(":/icons/preferences/robot.png");
	mUi->setupUi(this);

	QList<QString> const kitNames = mKitPluginManager.kitIds();

	if (kitNames.isEmpty()) {
		QLabel * const label = new QLabel();
		label->setText(tr("No constructor kit plugins loaded"));
		static_cast<QVBoxLayout *>(mUi->constructorKitGroupBox->layout())->insertWidget(0, label);
		mUi->typeOfModelGroupBox->setVisible(false);
		return;
	}

	if (kitNames.count() == 1) {
		mUi->constructorKitGroupBox->setVisible(false);
	} else {
		initMultipleRadioButtons();
	}

	mUi->typeOfModelGroupBox->setVisible(false);

	mKitPluginManager.selectKit(kitNames[0]);

	QWidget * const extensionWidget = mKitPluginManager.selectedKit().settingsWidget();
	if (extensionWidget) {
		static_cast<QVBoxLayout *>(mUi->settingsExtensionFrame->layout())->insertWidget(0, extensionWidget);
	}
}

RobotsSettingsPage::~RobotsSettingsPage()
{
	delete mUi;
}

void RobotsSettingsPage::save()
{
	SettingsManager::setValue("kitId", selectedKit());
	// SettingsManager::setValue("bluetoothPortName", selectedPortName());
	// SettingsManager::setValue("valueOfCommunication", selectedCommunication());
	SettingsManager::setValue("enableNoiseOfSensors", mUi->enableSensorNoiseCheckBox->isChecked());
	SettingsManager::setValue("enableNoiseOfEngines", mUi->enableEnginesNoiseCheckBox->isChecked());
	SettingsManager::setValue("approximationLevel", mUi->approximationLevelSpinBox->value());
	SettingsManager::setValue("sensorUpdateInterval", mUi->sensorUpdateSpinBox->value());
	SettingsManager::setValue("autoscalingInterval", mUi->autoScalingSpinBox->value());
	SettingsManager::setValue("textUpdateInterval", mUi->textUpdaterSpinBox->value());
	SettingsManager::setValue("nxtFlashToolRunPolicy", mUi->runningAfterUploadingComboBox->currentIndex());
//	mSensorsWidget->save();
	emit saved();
}

void RobotsSettingsPage::restoreSettings()
{
//	enums::robotModelType::robotModelTypeEnum typeOfRobotModel =
//			static_cast<enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
//	initRobotModelType(typeOfRobotModel);

//	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication").toString();
//	initTypeOfCommunication(typeOfCommunication);

	mUi->enableSensorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfSensors").toBool());
	mUi->enableEnginesNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfEngines").toBool());
	mUi->approximationLevelSpinBox->setValue(SettingsManager::value("approximationLevel").toInt());

	int const sensorsUpdateDefault = utils::sensorsGraph::SensorsGraph::readSensorDefaultInterval;
	int const autoscalingDefault = utils::sensorsGraph::SensorsGraph::autoscalingDefault;
	int const textUpdateDefault = utils::sensorsGraph::SensorsGraph::textUpdateDefault;
	mUi->sensorUpdateSpinBox->setValue(SettingsManager::value("sensorUpdateInterval", sensorsUpdateDefault).toInt());
	mUi->autoScalingSpinBox->setValue(SettingsManager::value("autoscalingInterval", autoscalingDefault).toInt());
	mUi->textUpdaterSpinBox->setValue(SettingsManager::value("textUpdateInterval", textUpdateDefault).toInt());

	mUi->runningAfterUploadingComboBox->setCurrentIndex(SettingsManager::value("nxtFlashToolRunPolicy").toInt());
}

void RobotsSettingsPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange: {
		mUi->retranslateUi(this);
		break;
	}
	default:
		break;
	}
}

void RobotsSettingsPage::initMultipleRadioButtons()
{
	for (QString const &kitName : mKitPluginManager.kitIds()) {
		QRadioButton * const kitRadioButton = new QRadioButton;
		kitRadioButton->setText(kitName);
		if (mUi->constructorKitGroupBox->layout()->count() == 1) {
			kitRadioButton->setChecked(true);
		}

		static_cast<QVBoxLayout *>(mUi->constructorKitGroupBox->layout())->insertWidget(
				mUi->constructorKitGroupBox->layout()->count() - 1, kitRadioButton);
		mKitRadioButtons->addButton(kitRadioButton);
	}
}

QString RobotsSettingsPage::selectedKit() const
{
	if (!mUi->constructorKitGroupBox->isVisible()) {
		return mKitPluginManager.kitIds()[0];
	}

	// TODO: Constructor kit selection based on radio buttons.
	return mKitPluginManager.kitIds()[0];
}
