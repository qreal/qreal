#include <QtWidgets/QFileDialog>
#include <QProcess>

#include "versioningPage.h"
#include "ui_versioningPage.h"
#include "../../../qrkernel/settingsManager.h"

using namespace git::ui;

PreferencesVersioningPage::PreferencesVersioningPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesVersioningPage)
{
//	mIcon = QIcon(":/icons/preferences/versioning.png");
	mUi->setupUi(this);

	mAuthenticationSettings = new qReal::versioning::ui::AuthenticationSettingsWidget("git");
	// The last widget in layout must remain spacer

	mUi->verticalLayout->insertWidget(mUi->verticalLayout->count() - 1, mAuthenticationSettings);

	mUi->checkBoxCompactMode->setChecked(qReal::SettingsManager::value("versioningCompactMode", true).toBool());
	qReal::SettingsManager::setValue("versioningCompactMode", mUi->checkBoxCompactMode->isChecked());
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
	qReal::SettingsManager::setValue("pathToGitClient", mUi->pathToGitClientLineEdit->text());
	qReal::SettingsManager::setValue("gitClientTimeout", mUi->gitTimeoutSpinBox->value());
	if (mUi->checkBoxCompactMode->isChecked() != qReal::SettingsManager::value("versioningCompactMode", true).toBool()){
		emit compactMode(mUi->checkBoxCompactMode->isChecked());
	}
	qReal::SettingsManager::setValue("versioningCompactMode", mUi->checkBoxCompactMode->isChecked());
	mAuthenticationSettings->save();
	emit checkClientExisting();
}

void PreferencesVersioningPage::restoreSettings()
{
	mAuthenticationSettings->reinit();
	mUi->pathToGitClientLineEdit->setText(qReal::SettingsManager::value("pathToGitClient", "").toString());
	mUi->gitTimeoutSpinBox->setValue(qReal::SettingsManager::value("gitClientTimeout", 30000).toInt());
	mUi->checkBoxCompactMode->setChecked(qReal::SettingsManager::value("versioningCompactMode", true).toBool());
}

void PreferencesVersioningPage::on_browseGitClientButton_clicked()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "git*");
	if (!path.isNull()) {
		mUi->pathToGitClientLineEdit->setText(path);
	}
}


QComboBox *PreferencesVersioningPage::createComboBox(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}
