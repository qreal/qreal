#include "refactoringWindow.h"
#include "ui_refactoringWindow.h"
#include <QtCore/QDir>

using namespace qReal;

RefactoringWindow::RefactoringWindow(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::refactoringForm)
{
	mUi->setupUi(this);
	mUi->applyButton->setEnabled(false);
	mUi->discardButton->setEnabled(false);
	mUi->findNextButton->setEnabled(false);
	connect(mUi->refactoringList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openPicture(QListWidgetItem*)));
	connect(mUi->findButton, SIGNAL(clicked()), this, SLOT(findButtonActivate()));
}

void RefactoringWindow::openPicture(QListWidgetItem *item)
{
	QImage image;
	QString const fileName = item->data(Qt::UserRole).toString();
	image.load(fileName);
	if (image.isNull())
		return;
	mUi->label->setScaledContents(true);
	mUi->label->setPixmap(QPixmap(fileName));
}

RefactoringWindow::~RefactoringWindow()
{
	delete mUi;
}

void RefactoringWindow::updateRefactorings(const QString &dirPath)
{
	QDir currentDir(dirPath);
	QStringList filters;
	filters << "*.png";
	QStringList const pngFiles = currentDir.entryList(filters);
	QListWidget *refactoringList = mUi->refactoringList;
	refactoringList->clear();
	foreach (QString png, pngFiles) {
		png.chop(4);
		QListWidgetItem *item = new QListWidgetItem(png);
		item->setData(Qt::UserRole, dirPath + "/" + png + ".png");
		refactoringList->addItem(item);
	}
}

void qReal::RefactoringWindow::findButtonActivate()
{
	QList<QListWidgetItem*> selectedItems = mUi->refactoringList->selectedItems();
	if (selectedItems.size() != 1)
		return;
	emit findButtonClicked(selectedItems.at(0)->text());
}

void qReal::RefactoringWindow::activateRestButtons()
{
	mUi->applyButton->setEnabled(true);
	mUi->discardButton->setEnabled(true);
	mUi->findNextButton->setEnabled(true);
	mUi->findButton->setEnabled(false);
}
