#include "../../../qrkernel/settingsManager.h"
#include "debuggerPage.h"
#include "ui_debuggerPage.h"

PreferencesDebuggerPage::PreferencesDebuggerPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesDebuggerPage),
	mIcon(":/icons/preferences/bug.png")
{
	mUi->setupUi(this);

	mUi->timeoutLineEdit->setText(SettingsManager::value("debuggerTimeout", 750).toString());
	mUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = SettingsManager::value("debugColor", "red").toString();
	int curColorIndex = mUi->colorComboBox->findText(curColor);
	mUi->colorComboBox->setCurrentIndex(curColorIndex);
}

PreferencesDebuggerPage::~PreferencesDebuggerPage()
{
	delete mUi;
}

void PreferencesDebuggerPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesDebuggerPage::save()
{
	SettingsManager::setValue("debuggerTimeout", mUi->timeoutLineEdit->text());
	SettingsManager::setValue("debugColor", mUi->colorComboBox->currentText());
}

QIcon PreferencesDebuggerPage::getIcon() const
{
	return mIcon;
}
