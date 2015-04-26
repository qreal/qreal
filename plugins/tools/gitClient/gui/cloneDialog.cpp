#include "cloneDialog.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include <qrkernel/settingsManager.h>

using namespace git::ui;

CloneDialog::CloneDialog(QWidget *parent)
	: QDialog(parent)
{
	mUrlLabel = new QLabel(tr("Clone from: "));
	mUrlComboBox = createComboBox(qReal::SettingsManager::value("cloneUrl", "").toString());
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
	mainLayout->addLayout(buttonsLayout, 2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("Clone options"));
	setMinimumSize(500,100);
}

QPushButton *CloneDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

QComboBox *CloneDialog::createComboBox(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}

QString CloneDialog::url() const
{
	return mUrlComboBox->currentText();
}


