#include "referenceList.h"
#include "ui_referenceList.h"

ReferenceList::ReferenceList(qReal::MainWindow *mainWindow, QPersistentModelIndex const &index
		, QString const &refType, QString const &currentValue, int role, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::ReferenceList)
	, mIndex(index)
	, mRole(role)
	, mWindow(mainWindow)
{
	mUi->setupUi(this);

	loadList(refType);
	highlightCurrentValue(currentValue);

	initConnections();
}

ReferenceList::~ReferenceList()
{
	delete mUi;
}

void ReferenceList::loadList(QString const &refType)
{
	const qrRepo::LogicalRepoApi *repoApi = &(mWindow->models()->logicalRepoApi());
	// there will be no need in this when models are synchronized
	qReal::models::GraphicalModelAssistApi &assistApi = mWindow->models()->graphicalModelAssistApi();

	foreach (qReal::Id element, repoApi->elementsByType(refType)) {
		if (repoApi->isLogicalElement(element) && !assistApi.graphicalIdsByLogicalId(element).isEmpty()) {
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

void ReferenceList::highlightCurrentValue(QString const &currentValue)
{
	for (int i = 0; i < mUi->listWidget->count(); i++) {
		QListWidgetItem* currItem = mUi->listWidget->item(i);
		if (currItem->data(Qt::UserRole) == currentValue) {
			currItem->setSelected(true);
		}
	}
}

void ReferenceList::initConnections()
{
	connect(mUi->listWidget, SIGNAL(itemClicked(QListWidgetItem*))
			, this, SLOT(activateElement(QListWidgetItem*)));
	connect(this, SIGNAL(accepted()), this, SLOT(valueChanged()));
	connect(this, SIGNAL(finished(int)), this, SLOT(restoreSelected()));
}

void ReferenceList::activateElement(QListWidgetItem *item)
{
	qReal::Id id = qReal::Id::loadFromString(item->data(Qt::UserRole).toString());
	mWindow->activateItemOrDiagram(id, false, true);
}

void ReferenceList::valueChanged()
{
	QString newValue = getNewValue();
	emit referenceSet(newValue, mIndex, mRole);
}

QString ReferenceList::getNewValue()
{
	QString newValue;
	if (!mUi->listWidget->selectedItems().isEmpty()) {
		newValue = mUi->listWidget->selectedItems()[0]->data(Qt::UserRole).toString();
	}
	return newValue;
}

void ReferenceList::restoreSelected()
{
	qReal::Id indexId = mWindow->models()->logicalModelAssistApi().idByIndex(mIndex);
	mWindow->activateItemOrDiagram(indexId, false, true);
}
