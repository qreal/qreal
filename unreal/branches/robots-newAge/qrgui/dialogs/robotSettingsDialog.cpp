#include "robotSettingsDialog.h"
#include "ui_robotSettingsDialog.h"

#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialenumerator.h"

using namespace qReal::gui;

RobotSettingsDialog::RobotSettingsDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::RobotSettingsDialog)
{
	mUi->setupUi(this);

	connect(mUi->okButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

	if (ports.isEmpty()) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->setText(tr("No COM ports found"));
	} else {
		qDebug() << "List of ports:";
		foreach (QextPortInfo info, ports) {
			qDebug() << "port name:" << info.portName;
			qDebug() << "friendly name:" << info.friendName;
			qDebug() << "physical name:" << info.physName;
			qDebug() << "enumerator name:" << info.enumName;
			qDebug() << "===================================";
			mUi->comPortComboBox->addItem(info.friendName);
		}
		QSettings settings("SPbSU", "QReal");
		QString const defaultPortName = settings.value("bluetoothPortName", "").toString();
		int const defaultIndex = mUi->comPortComboBox->findText(defaultPortName);
		if (defaultIndex != -1)
			mUi->comPortComboBox->setCurrentIndex(defaultIndex);
	}
}

RobotSettingsDialog::~RobotSettingsDialog()
{
	delete mUi;
}

void RobotSettingsDialog::ok()
{
	setResult(QDialog::Accepted);
	QSettings settings("SPbSU", "QReal");
	settings.setValue("bluetoothPortName", selectedPortName());
	close();
}

void RobotSettingsDialog::cancel()
{
	setResult(QDialog::Rejected);
	close();
}

QString RobotSettingsDialog::selectedPortName() const
{
	return mUi->comPortComboBox->currentText();
}
