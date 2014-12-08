#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include "resetDialog.h"
#include "../../../qrkernel/settingsManager.h"

using namespace git::ui;

ResetDialog::ResetDialog(QWidget *parent)
	: QDialog(parent)
{
	mHashCommitLabel = new QLabel(tr("Enter hash of commit: "));
	mHashCommitComboBox = createComboBox(qReal::SettingsManager::value("hashCommit", "").toString());
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mHashCommitLabel, 0, 0);
	mainLayout->addWidget(mHashCommitComboBox, 0, 1, 1, 2);
	mainLayout->addLayout(buttonsLayout,2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("Reset options"));
	setMinimumSize(500,100);
}

QPushButton *ResetDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

QComboBox *ResetDialog::createComboBox(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}

QString ResetDialog::hashCommit() const
{
	return mHashCommitComboBox->currentText();
}


