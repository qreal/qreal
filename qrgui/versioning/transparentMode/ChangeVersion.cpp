#include "ChangeVersion.h"
#include "ui_ChangeVersion.h"
#include <QtCore/QString>

using namespace qReal;

ChangeVersion::ChangeVersion(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::ChangeVersion)
{
	mUi->setupUi(this);
	mUi->pushButton->hide();
	mUi->label->hide();
	connect(mUi->pushButton, SIGNAL(clicked()), SLOT(obtainHash()));
//	mlist = new QListWidget;
//	mUi->gridLayout->addWidget(mlist);
}

ChangeVersion::~ChangeVersion()
{
	delete mUi;
}

void ChangeVersion::obtainHash()
{
	QString hash = mUi->listWidgetForLog->currentItem()->data(Qt::UserRole).toString();
	mUi->listWidgetForLog->clear();
	mUi->listWidgetForLog->deleteLater();
	emit hashObtained(hash);
}

void ChangeVersion::showDiff(QListWidgetItem *item)
{
	if (mUi->listWidgetForLog->row(item) != 0){
		mUi->pushButton->show();
	}else{
		mUi->pushButton->hide();
	}
}

void ChangeVersion::updateLog(QList<QPair<QString , QString> > listLog) // hash & mainPart
{
	mUi->pushButton->hide();
	if (listLog.size() != 0){
		disconnect(mUi->listWidgetForLog, SIGNAL(itemClicked(QListWidgetItem*))
				, this, SLOT(showDiff(QListWidgetItem*)));
		connect(mUi->listWidgetForLog, SIGNAL(itemClicked(QListWidgetItem*))
				, this, SLOT(showDiff(QListWidgetItem*)),Qt::QueuedConnection);
		mUi->listWidgetForLog->clear();
		int number = 0;
		while (number < listLog.size()){
			QListWidgetItem *item = new QListWidgetItem;
			item->setData(Qt::UserRole,listLog.at(number).first);
			item->setText(listLog.at(number).second);
			mUi->listWidgetForLog->addItem(item);
			number++;
		}
		mUi->listWidgetForLog->item(0)->setSelected(true);
	} else {
		mUi->listWidgetForLog->clear();
		QString text = "There are not version of the project or project was not versioned. \n Or project isn't loaded";
		mUi->label->setText(text);
		mUi->label->showMaximized();
	}
}

