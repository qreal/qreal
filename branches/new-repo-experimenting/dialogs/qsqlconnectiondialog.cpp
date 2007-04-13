#include "qsqlconnectiondialog.h"
#include "ui_qsqlconnectiondialog.h"

#include <QtSql/QSqlDatabase>
#include <QtCore/QSettings>

QSqlConnectionDialog::QSqlConnectionDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QStringList drivers = QSqlDatabase::drivers();

	// remove compat names
	drivers.removeAll("QMYSQL3");
	drivers.removeAll("QOCI8");
	drivers.removeAll("QODBC3");
	drivers.removeAll("QPSQL7");
	drivers.removeAll("QTDS7");

	drivers.sort();

	ui.comboDriver->addItems(drivers);

	readSettings();
}

QSqlConnectionDialog::~QSqlConnectionDialog()
{
}

void QSqlConnectionDialog::readSettings()
{
	QSettings settings("Tepkom", "Real Repository");

	QString driver = settings.value("driver").toString();
	ui.comboDriver->setCurrentIndex(ui.comboDriver->findText(driver));

	ui.editDatabase->setText(settings.value("database").toString());
	ui.editUsername->setText(settings.value("username").toString());
	ui.editPassword->setText(settings.value("password").toString());
	ui.editHostname->setText(settings.value("hostname").toString());
	ui.portSpinBox->setValue(settings.value("port",-1).toInt());
}

void QSqlConnectionDialog::writeSettings()
{
	QSettings settings("Tepkom", "Real Repository");
	settings.setValue("driver", ui.comboDriver->currentText());
	settings.setValue("database", ui.editDatabase->text());
	settings.setValue("username",ui.editUsername->text());
	settings.setValue("password", ui.editPassword->text());
	settings.setValue("hostname",ui.editHostname->text());
	settings.setValue("port",ui.portSpinBox->value());
}


QString QSqlConnectionDialog::driverName() const
{
	return ui.comboDriver->currentText();
}

QString QSqlConnectionDialog::databaseName() const
{
	return ui.editDatabase->text();
}

QString QSqlConnectionDialog::userName() const
{
	return ui.editUsername->text();
}

QString QSqlConnectionDialog::password() const
{
	return ui.editPassword->text();
}

QString QSqlConnectionDialog::hostName() const
{
	return ui.editHostname->text();
}

int QSqlConnectionDialog::port() const
{
	return ui.portSpinBox->value();
}

void QSqlConnectionDialog::on_okButton_clicked()
{
	if (ui.comboDriver->currentText().isEmpty()) {
		QMessageBox::information(this, tr("No database driver selected"),
				tr("Please select a database driver"));
		ui.comboDriver->setFocus();
	} else {
		writeSettings();
		accept();
	}
}
