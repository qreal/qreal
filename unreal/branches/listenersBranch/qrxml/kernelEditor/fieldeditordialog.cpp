#include "fieldeditordialog.h"
#include "ui_fieldeditordialog.h"

FieldEditorDialog::FieldEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldEditorDialog)
{
    ui->setupUi(this);
    ui->nameEdit->setText("field_name");
    ui->typeEdit->setText("int");

    connect(ui->buttonBox, SIGNAL(accepted()),
	    this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),
	    this, SLOT(reject()));
}

FieldEditorDialog::~FieldEditorDialog()
{
    delete ui;
}

QString FieldEditorDialog::generateFieldString()
{
	return ui->typeEdit->text() + " " + ui->nameEdit->text();
}

void FieldEditorDialog::changeEvent(QEvent *e)
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

QString FieldEditorDialog::getFieldName()
{
	return ui->nameEdit->text();
}

QString FieldEditorDialog::getFieldType()
{
	return ui->typeEdit->text();
}
