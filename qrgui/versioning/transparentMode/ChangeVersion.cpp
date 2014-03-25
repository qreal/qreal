#include "ChangeVersion.h"
#include "ui_ChangeVersion.h"
#include <QtCore/QString>

using namespace qReal;

ChangeVersion::ChangeVersion(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::ChangeVersion)
{
	mUi->setupUi(this);
}

ChangeVersion::~ChangeVersion()
{
	delete mUi;
}

void ChangeVersion::obtainHash(QListWidgetItem *item)
{
	QString hash = item->data(Qt::UserRole).toString();
	mUi->listWidgetForLog->clear();
	mUi->listWidgetForLog->deleteLater();
	emit hashObtained(hash);
}

void ChangeVersion::updateLog(QList<QPair<QString , QString> > listLog) // hash & mainPart
{
	if (listLog.size() != 0){
		disconnect(mUi->listWidgetForLog, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(obtainHash(QListWidgetItem*)));
		connect(mUi->listWidgetForLog, SIGNAL(itemDoubleClicked(QListWidgetItem*))
				, this, SLOT(obtainHash(QListWidgetItem*)),Qt::QueuedConnection);
		mUi->listWidgetForLog->clear();
		int number = 0;
		while (number < listLog.size()){
			QListWidgetItem *item = new QListWidgetItem;
			item->setData(Qt::UserRole,listLog.at(number).first);
			item->setText(listLog.at(number).second);
			mUi->listWidgetForLog->addItem(item);
			number++;
		}
	} else {
		mUi->listWidgetForLog->clear();
		disconnect(mUi->listWidgetForLog, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(obtainHash(QListWidgetItem*)));
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(tr("There are not version of the project or project was not versioned. \n Or project isn't loaded"));
		mUi->listWidgetForLog->addItem(item);
	}
}

