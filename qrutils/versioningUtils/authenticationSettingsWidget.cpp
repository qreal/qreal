#include "authenticationSettingsWidget.h"
#include "../../qrkernel/settingsManager.h"

#include <QGridLayout>
#include <QLabel>

using namespace versioning::ui;

AuthenticationSettingsWidget::AuthenticationSettingsWidget(QWidget *parent) :
	QWidget(parent)
{
	QFrame *frame = new QFrame;
	frame->setFrameShape(QFrame::StyledPanel);
	frame->setFrameShadow(QFrame::Raised);
	QGridLayout *layout = new QGridLayout;
	QLabel *userLabel = new QLabel(tr("Username:"));
	QLabel *passwordLabel = new QLabel(tr("Password:"));
	mUserEditor = new QLineEdit;
	mPasswordEditor = new QLineEdit;
	mPasswordEditor->setEchoMode(QLineEdit::Password);
	layout->addWidget(userLabel, 0, 0);
	layout->addWidget(passwordLabel, 1, 0);
	layout->addWidget(mUserEditor, 0, 1);
	layout->addWidget(mPasswordEditor, 1, 1);
	frame->setLayout(layout);

	mEnabledCheckBox = new QCheckBox(tr("Enable authentication"));
	connect(mEnabledCheckBox, SIGNAL(clicked()), this, SLOT(onEnableChecked()));
	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	mainLayout->setMargin(0);
	mainLayout->addWidget(mEnabledCheckBox);
	mainLayout->addWidget(frame);
	setLayout(mainLayout);
}

void AuthenticationSettingsWidget::init()
{
	mEnabledCheckBox->setChecked(SettingsManager::value(enabledSettingsName(), false).toBool());
	mUserEditor->setText(SettingsManager::value(usernameSettingsName()).toString());
	mPasswordEditor->setText(SettingsManager::value(passwordSettingsName()).toString());

	onEnableChecked();
}

void AuthenticationSettingsWidget::save()
{
	SettingsManager::setValue(enabledSettingsName(), mEnabledCheckBox->isChecked());
	SettingsManager::setValue(usernameSettingsName(), mUserEditor->text());
	SettingsManager::setValue(passwordSettingsName(), mPasswordEditor->text());
}

void AuthenticationSettingsWidget::onEnableChecked()
{
	mUserEditor->setEnabled(mEnabledCheckBox->isChecked());
	mPasswordEditor->setEnabled(mEnabledCheckBox->isChecked());
}

void AuthenticationSettingsWidget::setSettingsPrefix(const QString &prefix)
{
	mSettingsPrefix = prefix;
}

QString AuthenticationSettingsWidget::enabledSettingsName() const
{
	return mSettingsPrefix + "AuthenticationEnabled";
}

QString AuthenticationSettingsWidget::usernameSettingsName() const
{
	return mSettingsPrefix + "Username";
}

QString AuthenticationSettingsWidget::passwordSettingsName() const
{
	return mSettingsPrefix + "Password";
}
