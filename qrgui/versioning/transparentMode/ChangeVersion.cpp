#include "ChangeVersion.h"
#include "ui_ChangeVersion.h"
#include <QString>

using namespace qReal;

ChangeVersion::ChangeVersion(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ChangeVersion)
{
	ui->setupUi(this);
	connect(ui->listWidgetForLog, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(obtainHash(QListWidgetItem*)));
}

ChangeVersion::~ChangeVersion()
{
	delete ui;
}

void ChangeVersion::obtainHash(QListWidgetItem *item)
{
	QString hash = item->data(Qt::UserRole).toString();
	emit hashObtained(hash);
	emit swapTab();
}

void ChangeVersion::updateLog(QList<QPair<QString , QString> > listLog) // hash & mainPart
{
	ui->listWidgetForLog->clear();
	int number = 0;
	while (number < listLog.size()){
		QListWidgetItem *item = new QListWidgetItem;
		item->setData(Qt::UserRole,listLog.at(number).first);
		item->setText(listLog.at(number).second);
		ui->listWidgetForLog->addItem(item);
		number++;
	}
}
