#include <QtGui/QStandardItemModel>

#include "mainwindow/mainWindow.h"
#include "restoreElementDialog.h"
#include "ui_restoreElementDialog.h"

using namespace qReal;

RestoreElementDialog::RestoreElementDialog(QWidget *parent, MainWindow &mainWindow
		, EditorManagerInterface const &interpreterEditorManager, IdList const &elementsWithTheSameNameList)
	: QDialog(parent)
	, mUi(new Ui::RestoreElementDialog)
	, mMainWindow(mainWindow)
	, mInterpreterEditorManager(interpreterEditorManager)
	, mElementsWithTheSameNameList(elementsWithTheSameNameList)
{
	mUi->setupUi(this);
	fillSameNameElementsTV();
	connect(mUi->restoreButton, &QPushButton::clicked, this, &RestoreElementDialog::restoreButtonClicked);
	connect(mUi->createNewButton, &QPushButton::clicked, this, &RestoreElementDialog::createButtonClicked);
}

RestoreElementDialog::~RestoreElementDialog()
{
	delete mUi;
}

void RestoreElementDialog::fillSameNameElementsTV()
{
	QStandardItemModel *standardModel = new QStandardItemModel();

	QStandardItem *item = standardModel->invisibleRootItem();
	for (const auto &element: mElementsWithTheSameNameList) {
		QString state = tr("Existed");
		if (mInterpreterEditorManager.getIsHidden(element) == "true") {
			state = tr("Deleted");
		}
		QList<QStandardItem *> elementRow = prepareRow(mInterpreterEditorManager.friendlyName(element)
				+ " (" + state + ")", "", "");
		for (QStandardItem *item: elementRow) {
			item->setEditable(false);
		}
		item->appendRow(elementRow);

		QStringList propertiesInformationList = mInterpreterEditorManager.getPropertiesInformation(element);
		for (int i = 0; i <= propertiesInformationList.count() - 3; i += 3) {
			QList<QStandardItem *> preparedRow = prepareRow(propertiesInformationList[i]
					, propertiesInformationList[i + 1], propertiesInformationList[i + 2]);
			for (QStandardItem *item: preparedRow) {
				item->setSelectable(false);
				item->setEditable(false);
			}
			elementRow.first()->appendRow(preparedRow);
		}
	}

	standardModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Value"));
	mUi->sameNameElementsTV->setModel(standardModel);
	mUi->sameNameElementsTV->expandAll();
}

QList<QStandardItem *> RestoreElementDialog::prepareRow(QString const &first, QString const &second
		, QString const &third)
{
	 QList<QStandardItem *> rowItems;
	 rowItems << new QStandardItem(first);
	 rowItems << new QStandardItem(second);
	 rowItems << new QStandardItem(third);
	 return rowItems;
}

void RestoreElementDialog::restoreButtonClicked()
{
	if (mUi->sameNameElementsTV->selectionModel()->selectedIndexes().isEmpty()) {
		return;
	}

	int selectedRow = mUi->sameNameElementsTV->selectionModel()->selectedIndexes().first().row();
	Id const node = mElementsWithTheSameNameList[selectedRow];
	if (mInterpreterEditorManager.getIsHidden(node) == "true") {
		mInterpreterEditorManager.resetIsHidden(node);
		mMainWindow.loadPlugins();
	}
	emit restoreChosen(1);
	done(QDialog::Accepted);
}

void RestoreElementDialog::createButtonClicked()
{
	emit createNewChosen();
	done(QDialog::Accepted);
}
