/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "networkSettingsWidget.h"
#include "ui_networkSettingsWidget.h"

#include <qrkernel/settingsManager.h>


using namespace qReal::versioning::ui;

NetworkSettingsWidget::NetworkSettingsWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::NetworkSettingsWidget)
{
	mUi->setupUi(this);
	reinit();
}

NetworkSettingsWidget::NetworkSettingsWidget(const QString &settingsPrefix, QWidget *parent)
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
	mUi->enabledCheckBox->setChecked(qReal::SettingsManager::value(enabledSettingsName(), false).toBool());
	mUi->addressEditor->setText(qReal::SettingsManager::value(addressSettingsName()).toString());
	mUi->portEditor->setValue(qReal::SettingsManager::value(portSettingsName(), 0).toInt());
	mUi->userEditor->setText(qReal::SettingsManager::value(usernameSettingsName()).toString());
	mUi->passwordEditor->setText(qReal::SettingsManager::value(passwordSettingsName()).toString());
	mUi->timeoutEditor->setValue(qReal::SettingsManager::value(timeoutSettingsName(), 0).toInt());

	onEnableChecked();
}

void NetworkSettingsWidget::save()
{
	qReal::SettingsManager::setValue(enabledSettingsName(), mUi->enabledCheckBox->isChecked());
	qReal::SettingsManager::setValue(addressSettingsName(), mUi->addressEditor->text());
	qReal::SettingsManager::setValue(portSettingsName(), mUi->portEditor->value());
	qReal::SettingsManager::setValue(usernameSettingsName(), mUi->userEditor->text());
	qReal::SettingsManager::setValue(passwordSettingsName(), mUi->passwordEditor->text());
	qReal::SettingsManager::setValue(timeoutSettingsName(), mUi->timeoutEditor->value());
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
