#include "referenceList.h"
#include "ui_referenceList.h"

ReferenceList::ReferenceList(qReal::MainWindow *mainWindow, QPersistentModelIndex const &index
		, QString const &refType, QString const &currentValue, int role, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::ReferenceList)
	, mIndex(index)
	, mValue(currentValue)
	, mRole(role)
	, mWindow(mainWindow)
{
	mUi->setupUi(this);
	loadList(refType);
	highlightCurrentValue();

	connect(mUi->listWidget, SIGNAL(itemClicked(QListWidgetItem*))
			, this, SLOT(changeValue(QListWidgetItem*)));
	connect(this, SIGNAL(accepted()), this, SLOT(valueChanged()));
}

ReferenceList::~ReferenceList()
{
	delete mUi;
}

void ReferenceList::loadList(QString const &refType)
{
	const qrRepo::LogicalRepoApi *repoApi = &(mWindow->models()->logicalRepoApi());

	foreach (qReal::Id element, repoApi->elementsByType(refType)) {
		if (repoApi->isLogicalElement(element)) {
			addItem(element);
		}
	}
}

void ReferenceList::addItem(qReal::Id const &element)
{
	QString name = mWindow->models()->logicalRepoApi().name(element);

	QListWidgetItem *item = new QListWidgetItem(name, mUi->listWidget);
	item->setData(Qt::UserRole, element.toString());
}

void ReferenceList::highlightCurrentValue()
{
	for (int i = 0; i < mUi->listWidget->count(); i++) {
		QListWidgetItem* currItem = mUi->listWidget->item(i);
		if (currItem->data(Qt::UserRole) == mValue) {
			currItem->setSelected(true);
		}
	}
}

void ReferenceList::changeValue(QListWidgetItem *item)
{
	mValue = item->data(Qt::UserRole).toString();
}

void ReferenceList::valueChanged()
{
	emit referenceSet(mValue, mIndex, mRole);
}
