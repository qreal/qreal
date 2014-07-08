#include "robotsSettingsPage.h"
#include "ui_robotsSettingsPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsWatcher/sensorsGraph.h>
#include <interpreterBase/additionalPreferences.h>
#include <interpreterBase/robotModel/robotModelUtils.h>
#include "managers/robotModelManager.h"

using namespace interpreterCore::ui;
using namespace interpreterBase;
using namespace qReal;

RobotsSettingsPage::RobotsSettingsPage(
		KitPluginManager &kitPluginManager
		, RobotModelManager &robotModelManager
		, QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesRobotSettingsPage)
	, mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
{
	mIcon = QIcon(":/icons/preferences/robot.svg");
	mUi->setupUi(this);

	initializeAdditionalWidgets();
	initializeKitRadioButtons();

	mUi->devicesConfigurer->connectDevicesConfigurationProvider(this);
	mUi->devicesConfigurer->loadRobotModels(mKitPluginManager.allRobotModels());
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mUi->devicesConfigurer, &DevicesConfigurationWidget::selectRobotModel);

	restoreSettings();
	saveSelectedRobotModel();
}

RobotsSettingsPage::~RobotsSettingsPage()
{
	delete mUi;
}

void RobotsSettingsPage::initializeAdditionalWidgets()
{
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			AdditionalPreferences * const kitPreferences = kitPlugin->settingsWidget();
			if (kitPreferences) {
				mUi->settingsExtensionFrame->layout()->addWidget(kitPreferences);
			}
		}
	}
}

void RobotsSettingsPage::initializeKitRadioButtons()
{
	QLabel * const emptyCaseLabel = new QLabel(tr("No constructor kit plugins loaded"), this);
	mKitButtons = new QButtonGroup(this);
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			QString const &kitFriendlyName = kitPlugin->friendlyKitName();
			if (!kitFriendlyName.isEmpty()) {
				QRadioButton * const kitRadioButton = new QRadioButton(kitFriendlyName, this);
				kitRadioButton->hide();
				kitRadioButton->setObjectName(kitId);
				mKitButtons->addButton(kitRadioButton);
				connect(kitRadioButton, &QRadioButton::toggled, this, &RobotsSettingsPage::onKitRadioButtonToggled);
				mKitRobotModels[kitRadioButton] = initializeRobotModelsButtons(kitId, kitRadioButton);
				break;
			}
		}
	}

	showRadioButtonGroup(mUi->constructorKitGroupBox, mKitButtons, emptyCaseLabel);
}

QButtonGroup *RobotsSettingsPage::initializeRobotModelsButtons(QString const &kitId, QRadioButton * const kitButton)
{
	QButtonGroup * const result = new QButtonGroup(kitButton);
	for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
		for (auto &robotModel : kitPlugin->robotModels()) {
			QRadioButton * const button = new QRadioButton(robotModel->friendlyName(), this);
			button->setObjectName(kitId + robotModel->name());
			button->hide();
			mButtonsToRobotModelsMapping[button] = robotModel;
			connect(button, &QRadioButton::toggled, this, &RobotsSettingsPage::onRobotModelRadioButtonToggled);
			result->addButton(button);
		}
	}

	return result;
}

void RobotsSettingsPage::save()
{
	saveSelectedRobotModel();
	SettingsManager::setValue("enableNoiseOfSensors", mUi->enableSensorNoiseCheckBox->isChecked());
	SettingsManager::setValue("enableNoiseOfEngines", mUi->enableEnginesNoiseCheckBox->isChecked());
	SettingsManager::setValue("approximationLevel", mUi->approximationLevelSpinBox->value());
	SettingsManager::setValue("sensorUpdateInterval", mUi->sensorUpdateSpinBox->value());
	SettingsManager::setValue("autoscalingInterval", mUi->autoScalingSpinBox->value());
	SettingsManager::setValue("textUpdateInterval", mUi->textUpdaterSpinBox->value());
	SettingsManager::setValue("nxtFlashToolRunPolicy", mUi->runningAfterUploadingComboBox->currentIndex());

	mUi->devicesConfigurer->save();

	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			AdditionalPreferences * const kitPreferences = kitPlugin->settingsWidget();
			if (kitPreferences) {
				kitPreferences->save();
			}
		}
	}

	emit saved();
}

void RobotsSettingsPage::restoreSettings()
{
	if (mKitButtons->checkedButton()) {
		// This will force reinitialization even if selection didn`t change
		mKitButtons->checkedButton()->setChecked(false);
	}

	QString const selectedKit = SettingsManager::value("SelectedRobotKit").toString();
	QAbstractButton * const selectedKitButton = mUi->constructorKitGroupBox->findChild<QAbstractButton *>(selectedKit);
	if (selectedKitButton) {
		selectedKitButton->setChecked(true);
	}

	checkSelectedRobotModelButtonFor(selectedKitButton);

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

	mUi->devicesConfigurer->refresh();

	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			AdditionalPreferences * const kitPreferences = kitPlugin->settingsWidget();
			if (kitPreferences) {
				kitPreferences->restoreSettings();
			}
		}
	}
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

void RobotsSettingsPage::onKitRadioButtonToggled(bool checked)
{
	if (!checked) {
		return;
	}

	QAbstractButton * const kitButton = static_cast<QAbstractButton *>(sender());
	QLabel * const emptyCaseLabel = new QLabel(tr("No robot models available for ") + kitButton->text(), this);
	showRadioButtonGroup(mUi->typeOfModelGroupBox, mKitRobotModels[kitButton], emptyCaseLabel);

	checkSelectedRobotModelButtonFor(kitButton);
	showAdditionalPreferences(kitButton->objectName());
}

void RobotsSettingsPage::checkSelectedRobotModelButtonFor(QAbstractButton * const kitButton)
{
	QString const kitId = kitButton->objectName();
	robotModel::RobotModelInterface const *robotModel
			= robotModel::RobotModelUtils::selectedRobotModelFor(mKitPluginManager.kitsById(kitId));
	if (!robotModel) {
		return;
	}

	QAbstractButton * const robotModelButton = mUi->typeOfModelGroupBox
			->findChild<QAbstractButton *>(kitId + robotModel->name());
	if (robotModelButton) {
		robotModelButton->setChecked(true);
	}
}

void RobotsSettingsPage::onRobotModelRadioButtonToggled(bool checked)
{
	if (!checked) {
		return;
	}

	QString const selectedKit = mKitButtons->checkedButton()->objectName();
	QAbstractButton * const robotModelButton = static_cast<QAbstractButton *>(sender());
	robotModel::RobotModelInterface * const selectedRobotModel = mButtonsToRobotModelsMapping[robotModelButton];
	mUi->devicesConfigurer->selectRobotModel(*selectedRobotModel);
	for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(selectedKit)) {
		AdditionalPreferences * const selectedKitPreferences = kitPlugin->settingsWidget();
		if (selectedKitPreferences) {
			selectedKitPreferences->onRobotModelChanged(selectedRobotModel);
		}
	}
}

void RobotsSettingsPage::showAdditionalPreferences(QString const &kitId)
{
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			AdditionalPreferences * const kitPreferences = kitPlugin->settingsWidget();
			if (kitPreferences) {
				kitPreferences->hide();
			}
		}
	}

	for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
		AdditionalPreferences * const selectedKitPreferences = kitPlugin->settingsWidget();
		if (selectedKitPreferences) {
			selectedKitPreferences->show();
		}
	}
}

void RobotsSettingsPage::showRadioButtonGroup(QWidget * const container, QButtonGroup * const radioButtons
		, QWidget * const emptyCaseWidget)
{
	if (emptyCaseWidget) {
		emptyCaseWidget->setVisible(false);
	}

	if (container->layout()) {
		for (int i = 0; i < container->layout()->count(); ++i) {
			QLayoutItem * const item = container->layout()->itemAt(i);
			if (item && item->widget()) {
				item->widget()->hide();
			}
		}

		delete container->layout();
	}

	QVBoxLayout *layout = new QVBoxLayout(container);
	container->setLayout(layout);

	if (radioButtons->buttons().isEmpty()) {
		container->setVisible(emptyCaseWidget != nullptr);
		if (emptyCaseWidget) {
			container->layout()->addWidget(emptyCaseWidget);
			emptyCaseWidget->setVisible(true);
		}

		return;
	}

	for (QAbstractButton * const button : radioButtons->buttons()) {
		container->layout()->addWidget(button);
		button->setVisible(true);
		container->setVisible(true);
	}

	if (radioButtons->buttons().count() == 1) {
		container->setVisible(false);
		return;
	}

}

void RobotsSettingsPage::saveSelectedRobotModel()
{
	QAbstractButton * const selectedKitButton = mKitButtons->checkedButton();
	if (!selectedKitButton) {
		/// @todo inconsistent scenario
		return;
	}

	SettingsManager::setValue("SelectedRobotKit", selectedKitButton->objectName());
	QAbstractButton * const selectedRobotModelButton = mKitRobotModels[selectedKitButton]->checkedButton();
	// If null passed there default model will be used so it`s ok
	mRobotModelManager.setModel(mButtonsToRobotModelsMapping[selectedRobotModelButton]);
}
