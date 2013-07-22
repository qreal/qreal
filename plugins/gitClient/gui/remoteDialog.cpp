#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include "remoteDialog.h"
#include "../../../qrkernel/settingsManager.h"

using namespace versioning::ui;

RemoteDialog::RemoteDialog(QWidget *parent)
	: QDialog(parent)
{
	mUrlLabel = new QLabel(tr("Name: "));
	mPathLabel = new QLabel(tr("Adress: "));
	mPathComboBox = createComboBox(qReal::SettingsManager::value("remoteName", "").toString());
	mUrlComboBox = createComboBox(qReal::SettingsManager::value("remoreAdress", "").toString());
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mUrlLabel, 0, 0);
	mainLayout->addWidget(mUrlComboBox, 0, 1, 1, 2);
	mainLayout->addWidget(mPathLabel, 1, 0);
	mainLayout->addWidget(mPathComboBox, 1, 1);
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


QString RemoteDialog::target() const
{
	return mPathComboBox->currentText();
}

QString RemoteDialog::url() const
{
	return mUrlComboBox->currentText();
}

