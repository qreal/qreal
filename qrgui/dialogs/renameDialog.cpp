#include "renameDialog.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialogButtonBox>

using namespace qReal::gui;

RenameDialog::RenameDialog(QString const &initialText, QWidget *parent)
	: QDialog(parent), mInitialName(initialText)
{
	setWindowTitle(QCoreApplication::translate("qReal::gui::RenameDialog", "Enter new name"));

	mTextBox = new QLineEdit(mInitialName);
	mTextBox->setPlaceholderText(mInitialName);

	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(mTextBox);
	layout->addWidget(buttons, 0, Qt::AlignRight);
	setLayout(layout);
}

RenameDialog::~RenameDialog()
{
}

QString RenameDialog::name() const
{
	return mTextBox->text();
}

QString RenameDialog::selectNewName(QString const &currentName, QWidget *parent)
{
	RenameDialog *dialog = new RenameDialog(currentName, parent);
	dialog->exec();
	QString const result = dialog->name();
	if (!parent) {
		delete dialog;
	}
	return result;
}

void RenameDialog::accept()
{
	if (mTextBox->text().isEmpty()) {
		mTextBox->setText(mInitialName);
	}
	QDialog::reject();
}

void RenameDialog::reject()
{
	mTextBox->setText(mInitialName);
	QDialog::reject();
}
