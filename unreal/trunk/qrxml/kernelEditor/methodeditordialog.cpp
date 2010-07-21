#include "methodeditordialog.h"
#include "ui_methodeditordialog.h"
#include <QDebug>

MethodEditorDialog::MethodEditorDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MethodEditorDialog)
{
	ui->setupUi(this);
	ui->nameEdit->setText("method_name");
	ui->returnTypeEdit->setText("int");

	connect(ui->buttonBox, SIGNAL(accepted()),
		this, SLOT(generateMethodString()));
	connect(ui->addParameterBtn, SIGNAL(clicked()),
		this, SLOT(createParameter()));
}

MethodEditorDialog::~MethodEditorDialog()
{
	delete ui;
}

void MethodEditorDialog::changeEvent(QEvent *e)
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

QString MethodEditorDialog::generateMethodString()
{
	QString resultStr = ui->accessModifierComboBox->currentText() + " ";
	resultStr += ui->nameEdit->text() + "(";

	int i = 0;
	foreach (ParameterEditor curPrmEd, parameterList) {
		resultStr += curPrmEd.typeEdit->text() + " " + curPrmEd.nameEdit->text();
		i++;
		if (i != parameterList.size()) {
			resultStr += ", ";
		}
	}

	resultStr += "): " + ui->returnTypeEdit->text();

	qDebug() << "AAA___________" << resultStr;
	return resultStr;
}

void MethodEditorDialog::createParameter()
{
	QHBoxLayout* hLayout = new QHBoxLayout();

	QLineEdit *newNameEdit = new QLineEdit(ui->layoutWidget);
	newNameEdit->setText("prm");
	hLayout->addWidget(newNameEdit);

	QLineEdit *newTypeEdit = new QLineEdit(ui->layoutWidget);
	newTypeEdit->setText("int");
	hLayout->addWidget(newTypeEdit);

	//hLayout->addWidget(new QPushButton(ui->layoutWidget));
	ui->verticalLayout_2->addLayout(hLayout);

	parameterList.append(ParameterEditor(newNameEdit, newTypeEdit));
}
