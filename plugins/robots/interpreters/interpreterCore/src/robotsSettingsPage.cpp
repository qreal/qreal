#include "robotsSettingsPage.h"
#include "ui_robotsSettingsPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;
using namespace qReal;

RobotsSettingsPage::RobotsSettingsPage(KitPluginManager &kitPluginManager, QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesRobotSettingsPage)
		, mKitPluginManager(kitPluginManager)
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
		foreach (QString const &kitName, mKitPluginManager.kitIds()) {
			QRadioButton * const kitRadioButton = new QRadioButton();
			kitRadioButton->setText(kitName);
			if (mUi->constructorKitGroupBox->layout()->count() == 1) {
				kitRadioButton->setChecked(true);
			}

			static_cast<QVBoxLayout *>(mUi->constructorKitGroupBox->layout())->insertWidget(
					mUi->constructorKitGroupBox->layout()->count() - 1, kitRadioButton);
		}
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
	emit saved();
}

void RobotsSettingsPage::restoreSettings()
{
//	enums::robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
//	initRobotModelType(typeOfRobotModel);
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

QString RobotsSettingsPage::selectedKit() const
{
	if (!mUi->constructorKitGroupBox->isVisible()) {
		return mKitPluginManager.kitIds()[0];
	}

	// TODO: Constructor kit selection based on radio buttons.
	return mKitPluginManager.kitIds()[0];
}
