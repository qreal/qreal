#include "suggestToCreateDiagramDialog.h"

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(QWidget *parent)
	: QDialog(parent)
	, mLabel(tr("There is no existing diagram,\n choose diagram you want work with:"))
	, mDiagramsListWidget(new QListWidget)
	, mCancelButton(new QPushButton)
	, mOkButton(new QPushButton)
	, mVLayout(new QVBoxLayout)
	, mHLayout(new QHBoxLayout)
{
	this->setLayout(mVLayout);
	this->setMinimumSize(320, 240);
	this->setMaximumSize(320, 240);
	this->setWindowTitle(tr("Choose new diagram"));
	mDiagramsListWidget.setParent(this);
	mCancelButton.setText(tr("Cancel"));
	mOkButton.setText(tr("Done"));

	QObject::connect(&mDiagramsListWidget,SIGNAL(currentRowChanged(int)),this,SIGNAL(diagramsListCurrentRowChanged(int)));
	QObject::connect(&mDiagramsListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SIGNAL(diagramsListItemDoubleClicked(QListWidgetItem*)));
	QObject::connect(&mDiagramsListWidget,SIGNAL(destroyed()),this,SIGNAL(diagramsListDestroyed()));
	QObject::connect(&mCancelButton,SIGNAL(clicked()),this,SIGNAL(cancelButtonClicked()));
	QObject::connect(&mOkButton,SIGNAL(clicked()),this,SIGNAL(okButtonClicked()));

	mVLayout->addWidget(&mLabel);
	mVLayout->addWidget(&mDiagramsListWidget);
	mHLayout->addWidget(&mOkButton);
	mHLayout->addWidget(&mCancelButton);
	mVLayout->addLayout(mHLayout);
}

void SuggestToCreateDiagramDialog::addItemToDiagramsList(QString diagramName) {
	mDiagramsListWidget.addItem(diagramName);
}

void SuggestToCreateDiagramDialog::diagramsListSetCurrentRow(int num) {
	mDiagramsListWidget.setCurrentRow(num, QItemSelectionModel::Select);
}
