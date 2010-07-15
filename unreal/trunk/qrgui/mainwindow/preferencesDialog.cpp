#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesDialog::on_okButton_clicked()
{
	applyChanges();
	close();
}

void PreferencesDialog::on_applyButton_clicked()
{
	applyChanges();
}

void PreferencesDialog::applyChanges()
{

}
