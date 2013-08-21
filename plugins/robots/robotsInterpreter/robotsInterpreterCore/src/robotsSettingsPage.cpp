#include "robotsSettingsPage.h"
#include "ui_robotsSettingsPage.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>

#include <qrkernel/settingsManager.h>

using namespace robotsInterpreterCore;
using namespace qReal;

RobotsSettingsPage::RobotsSettingsPage(RobotsSettingsPageExtensionsInterface const &settingsExtensions, QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesRobotSettingsPage)
		, mSettingsExtensions(settingsExtensions)
{
	mIcon = QIcon(":/icons/preferences/robot.png");
	mUi->setupUi(this);

	QList<QString> const kitNames = mSettingsExtensions.kitNames();

	if (kitNames.isEmpty()) {
		QLabel * const label = new QLabel();
		label->setText(tr("No constructor kit plugins loaded"));
		static_cast<QVBoxLayout *>(mUi->constructorKitGroupBox->layout())->insertWidget(0, label);
		mUi->typeOfModelGroupBox->setVisible(false);
		return;
	}

	if (kitNames.count() > 1) {
		foreach (QString const &kitName, mSettingsExtensions.kitNames()) {
			QRadioButton * const kitRadioButton = new QRadioButton();
			kitRadioButton->setText(kitName);
			if (mUi->constructorKitGroupBox->layout()->count() == 1) {
				kitRadioButton->setChecked(true);
			}

			static_cast<QVBoxLayout *>(mUi->constructorKitGroupBox->layout())->insertWidget(
					mUi->constructorKitGroupBox->layout()->count() - 1, kitRadioButton);
		}
	}

	QWidget * const extensionWidget = mSettingsExtensions.kitSpecificSettingsWidget(kitNames[0]);
	static_cast<QVBoxLayout *>(mUi->settingsExtensionFrame->layout())->insertWidget(0, extensionWidget);
}

RobotsSettingsPage::~RobotsSettingsPage()
{
	delete mUi;
}

void RobotsSettingsPage::save()
{
//	SettingsManager::setValue("robotModel", selectedRobotModel());
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
