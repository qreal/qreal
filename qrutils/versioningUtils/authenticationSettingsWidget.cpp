#include "authenticationSettingsWidget.h"
#include "ui_authenticationSettingsWidget.h"


#include <qrkernel/settingsManager.h>

using namespace qReal::versioning::ui;

AuthenticationSettingsWidget::AuthenticationSettingsWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::AuthenticationSettingsWidget)
{
	mUi->setupUi(this);
	reinit();

	connect(mUi->enabledCheckBox, SIGNAL(clicked()), this, SLOT(onEnableChecked()));
}

AuthenticationSettingsWidget::AuthenticationSettingsWidget(QString const &settingsPrefix, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::AuthenticationSettingsWidget)
	, mSettingsPrefix(settingsPrefix)
{
	mUi->setupUi(this);
	reinit();

	connect(mUi->enabledCheckBox, SIGNAL(clicked()), this, SLOT(onEnableChecked()));
}

AuthenticationSettingsWidget::~AuthenticationSettingsWidget()
{
	delete mUi;
}

void AuthenticationSettingsWidget::retranslate()
{
	mUi->retranslateUi(this);
}

void AuthenticationSettingsWidget::reinit()
{
	mUi->enabledCheckBox->setChecked(qReal::SettingsManager::value(enabledSettingsName(), false).toBool());
	mUi->userEditor->setText(qReal::SettingsManager::value(usernameSettingsName()).toString());
	mUi->passwordEditor->setText(qReal::SettingsManager::value(passwordSettingsName()).toString());
	mUi->emailEditor->setText(qReal::SettingsManager::value(emailSettingsName()).toString());

	onEnableChecked();
}

void AuthenticationSettingsWidget::save()
{
	qReal::SettingsManager::setValue(enabledSettingsName(), mUi->enabledCheckBox->isChecked());
	qReal::SettingsManager::setValue(usernameSettingsName(), mUi->userEditor->text());
	qReal::SettingsManager::setValue(passwordSettingsName(), mUi->passwordEditor->text());
	qReal::SettingsManager::setValue(emailSettingsName(), mUi->emailEditor->text());
}

void AuthenticationSettingsWidget::onEnableChecked()
{
	mUi->userEditor->setEnabled(mUi->enabledCheckBox->isChecked());
	mUi->passwordEditor->setEnabled(mUi->enabledCheckBox->isChecked());
	mUi->emailEditor->setEnabled(mUi->enabledCheckBox->isChecked());
}

void AuthenticationSettingsWidget::setSettingsPrefix(const QString &prefix)
{
	mSettingsPrefix = prefix;
}

QString AuthenticationSettingsWidget::enabledSettingsName(const QString &prefix)
{
	return prefix + "AuthenticationEnabled";
}

QString AuthenticationSettingsWidget::usernameSettingsName(const QString &prefix)
{
	return prefix + "Username";
}

QString AuthenticationSettingsWidget::passwordSettingsName(const QString &prefix)
{
	return prefix + "Password";
}

QString AuthenticationSettingsWidget::emailSettingsName(const QString &prefix)
{
	return prefix + "Email";
}

QString AuthenticationSettingsWidget::enabledSettingsName() const
{
	return enabledSettingsName(mSettingsPrefix);
}

QString AuthenticationSettingsWidget::usernameSettingsName() const
{
	return usernameSettingsName(mSettingsPrefix);
}

QString AuthenticationSettingsWidget::passwordSettingsName() const
{
	return passwordSettingsName(mSettingsPrefix);
}

QString AuthenticationSettingsWidget::emailSettingsName() const
{
	return emailSettingsName(mSettingsPrefix);
}
