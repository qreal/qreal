#include "../../kernel/settingsManager.h"
#include "preferencesPages/debuggerPage.h"
#include "ui_debuggerPage.h"

PreferencesDebuggerPage::PreferencesDebuggerPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesDebuggerPage)
{
	mUi->setupUi(this);

	mUi->timeoutLineEdit->setText(SettingsManager::instance()->value("debuggerTimeout", 750).toString());
	mUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = SettingsManager::instance()->value("debugColor", "red").toString();
	int curColorIndex = mUi->colorComboBox->findText(curColor);
	mUi->colorComboBox->setCurrentIndex(curColorIndex);
	mUi->colorComboBox->setCurrentIndex(SettingsManager::instance()->value("debugColor", mUi->colorComboBox->currentIndex()).toInt());
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
	SettingsManager::instance()->setValue("debuggerTimeout", mUi->timeoutLineEdit->text());
	SettingsManager::instance()->setValue("debugColor", mUi->colorComboBox->currentIndex());
}
