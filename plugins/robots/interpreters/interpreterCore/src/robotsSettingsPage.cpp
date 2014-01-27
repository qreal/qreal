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
{
	mIcon = QIcon(":/icons/preferences/robot.png");
	mUi->setupUi(this);


//	mUi->typeOfModelGroupBox->setVisible(false);

	QList<QString> const kitNames = mKitPluginManager.kitIds();
	mKitPluginManager.selectKit(kitNames[0]);
	initializeKitRadioButtons();

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

void RobotsSettingsPage::onKitRadioButtonToggled(bool checked)
{
	if (!checked) {
		return;
	}

	QAbstractButton * const kitButton = static_cast<QAbstractButton *>(sender());
	showRadioButtonGroup(mUi->typeOfModelGroupBox, mKitRobotModels[kitButton]);
}

void RobotsSettingsPage::onRobotModelRadioButtonToggled(bool checked)
{
	if (!checked) {
		return;
	}
}

void RobotsSettingsPage::clearLayout(QLayout *layout)
{
	QLayoutItem *item;
	while ((item = layout->takeAt(0))) {
		if (item->layout()) {
			clearLayout(item->layout());
		}

		layout->removeItem(item);
	}
}

void RobotsSettingsPage::initializeKitRadioButtons()
{
	QLabel * const emptyCaseLabel = new QLabel(tr("No constructor kit plugins loaded"), this);
	QButtonGroup * const kitRadioButtons = new QButtonGroup(this);
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		interpreterBase::KitPluginInterface const &kit = mKitPluginManager.kitById(kitId);
		QRadioButton * const kitRadioButton = new QRadioButton(kit.friendlyKitName());
		kitRadioButtons->addButton(kitRadioButton);
		connect(kitRadioButton, SIGNAL(toggled(bool)), this, SLOT(onKitRadioButtonToggled(bool)));
		mKitRobotModels[kitRadioButton] = initializeRobotModelsButtons(kitId, kitRadioButton);
	}

	showRadioButtonGroup(mUi->constructorKitGroupBox, kitRadioButtons, emptyCaseLabel);
}

QButtonGroup *RobotsSettingsPage::initializeRobotModelsButtons(QString const &kitId, QRadioButton * const kitButton)
{
	QButtonGroup * const result = new QButtonGroup(kitButton);
	for (auto &robotModel : mKitPluginManager.kitById(kitId).robotModels()) {
		QString const name = robotModel->name();
		QRadioButton * const button = new QRadioButton(name);
		mButtonsToRobotModelsMapping[button] = robotModel;
		connect(button, &QRadioButton::toggled, this, &RobotsSettingsPage::onRobotModelRadioButtonToggled);
		result->addButton(button);
	}

	return result;
}

void RobotsSettingsPage::showRadioButtonGroup(QWidget * const container, QButtonGroup * const radioButtons
		, QWidget * const emptyCaseWidget)
{
	if (emptyCaseWidget) {
		emptyCaseWidget->setVisible(false);
	}

	clearLayout(container->layout());
	if (radioButtons->buttons().isEmpty()) {
		container->setVisible(emptyCaseWidget != nullptr);
		if (emptyCaseWidget) {
			container->layout()->addWidget(emptyCaseWidget);
			emptyCaseWidget->setVisible(true);
		}

		return;
	}

	if (radioButtons->buttons().count() == 1) {
		container->setVisible(false);
		//radioButtons->buttons()[0]->setChecked(true);
		return;
	}

	for (QAbstractButton * const button : radioButtons->buttons()) {
		container->layout()->addWidget(button);
		container->setVisible(true);
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
