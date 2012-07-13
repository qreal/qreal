#include "networkSettingsWidget.h"
#include "../../qrkernel/settingsManager.h"

#include <QGridLayout>
#include <QLabel>

using namespace versioning::ui;

NetworkSettingsWidget::NetworkSettingsWidget(QWidget *parent) :
	QWidget(parent)
{
	QFrame *frame = new QFrame;
	frame->setFrameShape(QFrame::StyledPanel);
	frame->setFrameShadow(QFrame::Raised);
	QGridLayout *layout = new QGridLayout;
	QLabel *serverAddressLabel = new QLabel(tr("Server address:"));
	QLabel *serverPortLabel = new QLabel(tr("Server port:"));
	QLabel *userLabel = new QLabel(tr("Username:"));
	QLabel *passwordLabel = new QLabel(tr("Password:"));
	QLabel *timeoutLabel = new QLabel(tr("Timeout in sec:"));
	mServerAddressEditor = new QLineEdit;
	mServerPortEditor = new QSpinBox;
	mServerPortEditor->setMinimum(0);
	mServerPortEditor->setMaximum(65535);
	mServerPortEditor->setAccelerated(true);
	mUserEditor = new QLineEdit;
	mPasswordEditor = new QLineEdit;
	mPasswordEditor->setEchoMode(QLineEdit::Password);
	mTimeoutEditor = new QSpinBox;
	mTimeoutEditor->setMinimum(0);
	mTimeoutEditor->setMaximum(1000);
	mTimeoutEditor->setAccelerated(true);
	layout->setColumnStretch(0, 30);
	layout->setColumnStretch(1, 50);
	layout->addWidget(serverAddressLabel, 0, 0);
	layout->addWidget(serverPortLabel, 1, 0);
	layout->addWidget(userLabel, 2, 0);
	layout->addWidget(passwordLabel, 3, 0);
	layout->addWidget(timeoutLabel, 4, 0);
	layout->addWidget(mServerAddressEditor, 0, 1);
	layout->addWidget(mServerPortEditor, 1, 1);
	layout->addWidget(mUserEditor, 2, 1);
	layout->addWidget(mPasswordEditor, 3, 1);
	layout->addWidget(mTimeoutEditor, 4, 1);
	frame->setLayout(layout);

	mEnabledCheckBox = new QCheckBox(tr("Enable proxy"));
	connect(mEnabledCheckBox, SIGNAL(clicked()), this, SLOT(onEnableChecked()));
	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	mainLayout->setMargin(0);
	mainLayout->addWidget(mEnabledCheckBox);
	mainLayout->addWidget(frame);
	setLayout(mainLayout);
}

void NetworkSettingsWidget::init()
{
	mEnabledCheckBox->setChecked(SettingsManager::value(enabledSettingsName(), false).toBool());
	mServerAddressEditor->setText(SettingsManager::value(addressSettingsName()).toString());
	mServerPortEditor->setValue(SettingsManager::value(portSettingsName(), 0).toInt());
	mUserEditor->setText(SettingsManager::value(usernameSettingsName()).toString());
	mPasswordEditor->setText(SettingsManager::value(passwordSettingsName()).toString());
	mTimeoutEditor->setValue(SettingsManager::value(timeoutSettingsName(), 0).toInt());

	onEnableChecked();

}

void NetworkSettingsWidget::save()
{
	SettingsManager::setValue(enabledSettingsName(), mEnabledCheckBox->isChecked());
	SettingsManager::setValue(addressSettingsName(), mServerAddressEditor->text());
	SettingsManager::setValue(portSettingsName(), mServerPortEditor->value());
	SettingsManager::setValue(usernameSettingsName(), mUserEditor->text());
	SettingsManager::setValue(passwordSettingsName(), mPasswordEditor->text());
	SettingsManager::setValue(timeoutSettingsName(), mTimeoutEditor->value());
}

void NetworkSettingsWidget::onEnableChecked()
{
	mServerAddressEditor->setEnabled(mEnabledCheckBox->isChecked());
	mServerPortEditor->setEnabled(mEnabledCheckBox->isChecked());
	mUserEditor->setEnabled(mEnabledCheckBox->isChecked());
	mPasswordEditor->setEnabled(mEnabledCheckBox->isChecked());
	mTimeoutEditor->setEnabled(mEnabledCheckBox->isChecked());
}

void NetworkSettingsWidget::setSettingsPrefix(const QString &prefix)
{
	mSettingsPrefix = prefix;
}

QString NetworkSettingsWidget::enabledSettingsName() const
{
	return mSettingsPrefix + "ProxyEnabled";
}

QString NetworkSettingsWidget::addressSettingsName() const
{
	return mSettingsPrefix + "ProxyServer";
}

QString NetworkSettingsWidget::portSettingsName() const
{
	return mSettingsPrefix + "ProxyPort";
}

QString NetworkSettingsWidget::usernameSettingsName() const
{
	return mSettingsPrefix + "ProxyUsername";
}

QString NetworkSettingsWidget::passwordSettingsName() const
{
	return mSettingsPrefix + "ProxyPassword";
}

QString NetworkSettingsWidget::timeoutSettingsName() const
{
	return mSettingsPrefix + "ProxyTimeout";
}
