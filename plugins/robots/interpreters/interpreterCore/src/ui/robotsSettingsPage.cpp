/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "robotsSettingsPage.h"
#include "ui_robotsSettingsPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsWatcher/sensorsGraph.h>
#include <kitBase/additionalPreferences.h>
#include <kitBase/robotModel/robotModelUtils.h>

#include "interpreterCore/managers/robotModelManager.h"

using namespace interpreterCore::ui;
using namespace kitBase;
using namespace qReal;

RobotsSettingsPage::RobotsSettingsPage(KitPluginManager &kitPluginManager
		, RobotModelManager &robotModelManager
		, LogicalModelAssistInterface &logicalModel
		, QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesRobotSettingsPage)
	, mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
	, mLogicalModel(logicalModel)
{
	setWindowIcon(QIcon(":/icons/preferences/robot.svg"));
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
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			QList<AdditionalPreferences *> const kitPreferences = kitPlugin->settingsWidgets();
			for (AdditionalPreferences * const kitPreference: kitPreferences) {
				if (kitPreference) {
					mUi->settingsExtensionFrame->layout()->addWidget(kitPreference);
				}
			}
		}
	}
}

void RobotsSettingsPage::initializeKitRadioButtons()
{
	QLabel * const emptyCaseLabel = new QLabel(tr("No constructor kit plugins loaded"), this);
	mKitButtons = new QButtonGroup(this);
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			const QString &kitFriendlyName = kitPlugin->friendlyKitName();
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

QButtonGroup *RobotsSettingsPage::initializeRobotModelsButtons(const QString &kitId, QRadioButton * const kitButton)
{
	QButtonGroup * const result = new QButtonGroup(kitButton);
	QList<robotModel::RobotModelInterface *> robotModels;
	for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
		robotModels += kitPlugin->robotModels();
	}

	robotModel::RobotModelUtils::sortRobotModels(robotModels);
	for (auto robotModel : robotModels) {
		QRadioButton * const button = new QRadioButton(robotModel->friendlyName(), this);
		button->setObjectName(kitId + robotModel->name());
		button->hide();
		mButtonsToRobotModelsMapping[button] = robotModel;
		connect(button, &QRadioButton::toggled, this, &RobotsSettingsPage::onRobotModelRadioButtonToggled);
		result->addButton(button);
	}

	return result;
}

void RobotsSettingsPage::save()
{
	saveSelectedRobotModel();
	SettingsManager::setValue("sensorUpdateInterval", mUi->sensorUpdateSpinBox->value());
	SettingsManager::setValue("autoscalingInterval", mUi->autoScalingSpinBox->value());
	SettingsManager::setValue("textUpdateInterval", mUi->textUpdaterSpinBox->value());
	SettingsManager::setValue("nxtFlashToolRunPolicy", mUi->runningAfterUploadingComboBox->currentIndex());

	mUi->devicesConfigurer->save();

	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			QList<AdditionalPreferences *> const kitPreferences = kitPlugin->settingsWidgets();
			for (AdditionalPreferences * const kitPreference: kitPreferences) {
				if (kitPreference) {
					kitPreference->save();
				}
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

	const QString selectedKit = SettingsManager::value("SelectedRobotKit").toString();
	QAbstractButton * const selectedKitButton = mUi->constructorKitGroupBox->findChild<QAbstractButton *>(selectedKit);
	if (selectedKitButton) {
		selectedKitButton->setChecked(true);
		checkSelectedRobotModelButtonFor(selectedKitButton);
	}

	const int sensorsUpdateDefault = utils::sensorsGraph::SensorsGraph::readSensorDefaultInterval;
	const int autoscalingDefault = utils::sensorsGraph::SensorsGraph::autoscalingDefault;
	const int textUpdateDefault = utils::sensorsGraph::SensorsGraph::textUpdateDefault;
	mUi->sensorUpdateSpinBox->setValue(SettingsManager::value("sensorUpdateInterval", sensorsUpdateDefault).toInt());
	mUi->autoScalingSpinBox->setValue(SettingsManager::value("autoscalingInterval", autoscalingDefault).toInt());
	mUi->textUpdaterSpinBox->setValue(SettingsManager::value("textUpdateInterval", textUpdateDefault).toInt());

	mUi->runningAfterUploadingComboBox->setCurrentIndex(SettingsManager::value("nxtFlashToolRunPolicy").toInt());

	mUi->devicesConfigurer->refresh();

	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			QList<AdditionalPreferences *> const kitPreferences = kitPlugin->settingsWidgets();
			for (AdditionalPreferences * const kitPreference: kitPreferences) {
				if (kitPreference) {
					kitPreference->restoreSettings();
				}
			}
		}
	}
}

void RobotsSettingsPage::onProjectOpened()
{
	mUi->devicesConfigurer->setEnabled(
			!mLogicalModel.logicalRepoApi().metaInformation("twoDModelSensorsReadOnly").toBool());
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
	const QString kitId = kitButton->objectName();
	const robotModel::RobotModelInterface *robotModel
			= robotModel::RobotModelUtils::selectedRobotModelFor(mKitPluginManager.kitsById(kitId));
	if (!robotModel) {
		return;
	}

	QAbstractButton * const robotModelButton = mUi->typeOfModelGroupBox
			->findChild<QAbstractButton *>(kitId + robotModel->name());
	if (robotModelButton) {
		robotModelButton->setChecked(true);
		changeRobotModel(robotModelButton);
	}
}

void RobotsSettingsPage::onRobotModelRadioButtonToggled(bool checked)
{
	if (!checked) {
		return;
	}

	QAbstractButton * const robotModelButton = static_cast<QAbstractButton *>(sender());
	changeRobotModel(robotModelButton);
}

void RobotsSettingsPage::showAdditionalPreferences(const QString &kitId)
{
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			QList<AdditionalPreferences *> const kitPreferences = kitPlugin->settingsWidgets();
			for (AdditionalPreferences * const kitPreference: kitPreferences) {
				if (kitPreference) {
					kitPreference->hide();
				}
			}
		}
	}

	for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
		QList <AdditionalPreferences *> const selectedKitPreferences = kitPlugin->settingsWidgets();
		for (AdditionalPreferences * const selectedKitPreference: selectedKitPreferences) {
			if (selectedKitPreference) {
				selectedKitPreference->show();
			}
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

void RobotsSettingsPage::changeRobotModel(QAbstractButton * const robotModelButton)
{
	const QString selectedKit = mKitButtons->checkedButton()->objectName();
	robotModel::RobotModelInterface * const selectedRobotModel = mButtonsToRobotModelsMapping[robotModelButton];
	mUi->devicesConfigurer->selectRobotModel(*selectedRobotModel);
	for (KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(selectedKit)) {
		QList<AdditionalPreferences *> const selectedKitPreferences = kitPlugin->settingsWidgets();
		for (AdditionalPreferences * const selectedKitPreference: selectedKitPreferences) {
			if (selectedKitPreference) {
				selectedKitPreference->onRobotModelChanged(selectedRobotModel);
			}
		}
	}
}
