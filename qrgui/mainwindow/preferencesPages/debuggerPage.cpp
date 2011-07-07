#include "preferencesPages/debuggerPage.h"
#include "ui_debuggerPage.h"

#include <QSettings>

PreferencesDebuggerPage::PreferencesDebuggerPage(QWidget *parent) :
	PreferencesPage(parent),
	ui(new Ui::preferencesDebuggerPage)
{
	ui->setupUi(this);

	QSettings settings("SPbSU", "QReal");
	ui->timeoutLineEdit->setText(settings.value("debuggerTimeout", 750).toString());
	ui->colorComboBox->addItems(QColor::colorNames());
	QString curColor = settings.value("debugColor", "red").toString();
	int curColorIndex = ui->colorComboBox->findText(curColor);
	ui->colorComboBox->setCurrentIndex(curColorIndex);
	settings.value("debugColor", ui->colorComboBox->currentText());
}

PreferencesDebuggerPage::~PreferencesDebuggerPage()
{
	delete ui;
}

void PreferencesDebuggerPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesDebuggerPage::save()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("debuggerTimeout", ui->timeoutLineEdit->text());
	settings.setValue("debugColor", ui->colorComboBox->currentText());
}
