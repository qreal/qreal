#include "versioningPage.h"
#include "ui_versioningPage.h"
#include "../../../qrkernel/settingsManager.h"

#include <QFileDialog>

using namespace versioning::ui;

PreferencesVersioningPage::PreferencesVersioningPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesVersioningPage)
{
	mIcon = QIcon(":/icons/preferences/versioning.png");
	mUi->setupUi(this);

	mAuthenticationSettings = new AuthenticationSettingsWidget;
	mAuthenticationSettings->setSettingsPrefix("svn");
	mAuthenticationSettings->init();
	// The last widget in layout must remain spacer
	mUi->verticalLayout->insertWidget(mUi->verticalLayout->count() - 1, mAuthenticationSettings);

// TODO: Add proxy settings.
//	mNetworkSettings = new NetworkSettingsWidget;
//	mNetworkSettings->setSettingsPrefix("svn");
//	mNetworkSettings->init();
//	mUi->verticalLayout->insertWidget(mUi->verticalLayout->count() - 1, mNetworkSettings);

	mUi->pathToSvnClientLineEdit->setText(SettingsManager::value("pathToSvnClient", "").toString());
	mUi->svnTimeoutSpinBox->setValue(SettingsManager::value("svnClientTimeout", 30000).toInt());
}

PreferencesVersioningPage::~PreferencesVersioningPage()
{
	delete mUi;
}

void PreferencesVersioningPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesVersioningPage::save()
{
	SettingsManager::setValue("pathToSvnClient", mUi->pathToSvnClientLineEdit->text());
	SettingsManager::setValue("svnClientTimeout", mUi->svnTimeoutSpinBox->value());
	mAuthenticationSettings->save();
//	mNetworkSettings->save();
}

void PreferencesVersioningPage::on_browseSvnClientButton_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "svn*");
	if (path != NULL) {
		mUi->pathToSvnClientLineEdit->setText(path);
	}
}
