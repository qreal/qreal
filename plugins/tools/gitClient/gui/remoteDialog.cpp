#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include "remoteDialog.h"
#include "../../../qrkernel/settingsManager.h"

using namespace git::ui;

RemoteDialog::RemoteDialog(QWidget *parent)
	: QDialog(parent)
{
	mRemoteNameLabel = new QLabel(tr("Name: "));
	mRemoteUrlLabel = new QLabel(tr("Adress: "));
	mRemoteNameComboBox = createComboBox(qReal::SettingsManager::value("gitRemoteName", "").toString());
	mRemoteUrlComboBox = createComboBox(qReal::SettingsManager::value("gitRemoteUrl", "").toString());
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mRemoteNameLabel, 0, 0);
	mainLayout->addWidget(mRemoteNameComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(mRemoteUrlLabel, 1, 0);
	mainLayout->addWidget(mRemoteUrlComboBox, 1, 1);
	mainLayout->addLayout(buttonsLayout,2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("New remote"));
	setFixedSize(500,100);
}

QPushButton *RemoteDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

QComboBox *RemoteDialog::createComboBox(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}


QString RemoteDialog::remoteName() const
{
	return mRemoteNameComboBox->currentText();
}

QString RemoteDialog::remoteUrl() const
{
	return mRemoteUrlComboBox->currentText();
}

