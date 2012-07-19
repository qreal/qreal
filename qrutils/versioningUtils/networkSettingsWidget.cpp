#include "networkSettingsWidget.h"
#include "ui_networkSettingsWidget.h"
#include "../../qrkernel/settingsManager.h"

using namespace versioning::ui;

NetworkSettingsWidget::NetworkSettingsWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::NetworkSettingsWidget)
{
	mUi->setupUi(this);
	reinit();
}

NetworkSettingsWidget::NetworkSettingsWidget(const QString &settingsPrefix
		, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::NetworkSettingsWidget)
	, mSettingsPrefix(settingsPrefix)
{
	mUi->setupUi(this);
	reinit();
}

NetworkSettingsWidget::~NetworkSettingsWidget()
{
	delete mUi;
}

void NetworkSettingsWidget::retranslate()
{
	mUi->retranslateUi(this);
}

void NetworkSettingsWidget::reinit()
{
	mUi->enabledCheckBox->setChecked(SettingsManager::value(enabledSettingsName(), false).toBool());
	mUi->addressEditor->setText(SettingsManager::value(addressSettingsName()).toString());
	mUi->portEditor->setValue(SettingsManager::value(portSettingsName(), 0).toInt());
	mUi->userEditor->setText(SettingsManager::value(usernameSettingsName()).toString());
	mUi->passwordEditor->setText(SettingsManager::value(passwordSettingsName()).toString());
	mUi->timeoutEditor->setValue(SettingsManager::value(timeoutSettingsName(), 0).toInt());

	onEnableChecked();
}

void NetworkSettingsWidget::save()
{
	SettingsManager::setValue(enabledSettingsName(), mUi->enabledCheckBox->isChecked());
	SettingsManager::setValue(addressSettingsName(), mUi->addressEditor->text());
	SettingsManager::setValue(portSettingsName(), mUi->portEditor->value());
	SettingsManager::setValue(usernameSettingsName(), mUi->userEditor->text());
	SettingsManager::setValue(passwordSettingsName(), mUi->passwordEditor->text());
	SettingsManager::setValue(timeoutSettingsName(), mUi->timeoutEditor->value());
}

void NetworkSettingsWidget::onEnableChecked()
{
	mUi->addressEditor->setEnabled(mUi->enabledCheckBox->isChecked());
	mUi->portEditor->setEnabled(mUi->enabledCheckBox->isChecked());
	mUi->userEditor->setEnabled(mUi->enabledCheckBox->isChecked());
	mUi->passwordEditor->setEnabled(mUi->enabledCheckBox->isChecked());
	mUi->timeoutEditor->setEnabled(mUi->enabledCheckBox->isChecked());
}

void NetworkSettingsWidget::setSettingsPrefix(const QString &prefix)
{
	mSettingsPrefix = prefix;
}

QString NetworkSettingsWidget::enabledSettingsName(const QString &prefix)
{
	return prefix + "ProxyEnabled";
}

QString NetworkSettingsWidget::addressSettingsName(const QString &prefix)
{
	return prefix + "ProxyServer";
}

QString NetworkSettingsWidget::portSettingsName(const QString &prefix)
{
	return prefix + "ProxyPort";
}

QString NetworkSettingsWidget::usernameSettingsName(const QString &prefix)
{
	return prefix + "ProxyUsername";
}

QString NetworkSettingsWidget::passwordSettingsName(const QString &prefix)
{
	return prefix + "ProxyPassword";
}

QString NetworkSettingsWidget::timeoutSettingsName(const QString &prefix)
{
	return prefix + "ProxyTimeout";
}

QString NetworkSettingsWidget::enabledSettingsName() const
{
	return enabledSettingsName(mSettingsPrefix);
}

QString NetworkSettingsWidget::addressSettingsName() const
{
	return addressSettingsName(mSettingsPrefix);
}

QString NetworkSettingsWidget::portSettingsName() const
{
	return portSettingsName(mSettingsPrefix);
}

QString NetworkSettingsWidget::usernameSettingsName() const
{
	return usernameSettingsName(mSettingsPrefix);
}

QString NetworkSettingsWidget::passwordSettingsName() const
{
	return passwordSettingsName(mSettingsPrefix);
}

QString NetworkSettingsWidget::timeoutSettingsName() const
{
	return timeoutSettingsName(mSettingsPrefix);
}
