#include "versioningPage.h"
#include "ui_versioningPage.h"
#include "../../../qrkernel/settingsManager.h"

#include <QtGui/QFileDialog>

using namespace versioning::ui;

PreferencesVersioningPage::PreferencesVersioningPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesVersioningPage)
{
	mIcon = QIcon(":/icons/preferences/versioning.png");
	mUi->setupUi(this);

	mAuthenticationSettings = new AuthenticationSettingsWidget("svn");
	// The last widget in layout must remain spacer
	mUi->verticalLayout->insertWidget(mUi->verticalLayout->count() - 1, mAuthenticationSettings);

	mUi->pathToSvnClientLineEdit->setText(qReal::SettingsManager::value("pathToSvnClient", "").toString());
	mUi->svnTimeoutSpinBox->setValue(qReal::SettingsManager::value("svnClientTimeout", 30000).toInt());
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
		mAuthenticationSettings->retranslate();
		break;
	default:
		break;
	}
}

void PreferencesVersioningPage::save()
{
	qReal::SettingsManager::setValue("pathToSvnClient", mUi->pathToSvnClientLineEdit->text());
	qReal::SettingsManager::setValue("svnClientTimeout", mUi->svnTimeoutSpinBox->value());
	mAuthenticationSettings->save();
}

void PreferencesVersioningPage::on_browseSvnClientButton_clicked()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "svn*");
	if (!path.isNull()) {
		mUi->pathToSvnClientLineEdit->setText(path);
	}
}
