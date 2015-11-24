#include "tableMenuWidget.h"
#include "ui_tableMenuWidget.h"
#include "editorViewScene.h"

namespace qReal {

class EditorViewScene;

TableMenuWidget::TableMenuWidget(const Id &id, EditorViewScene *editorViewScene, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::TableMenuWidget)
	, mId(id)
	, mEditorViewScene(editorViewScene)
{
	mUi->setupUi(this);
	this->setModal(true);
	fillGeneralTab();
	mUi->columnDataTable->hideColumn(2);

	connect(mUi->columnDataTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateColumn(QTableWidgetItem*)));
}

TableMenuWidget::~TableMenuWidget()
{
	delete mUi;
}

void TableMenuWidget::open()
{
	this->setVisible(true);
	this->activateWindow();
}

void TableMenuWidget::close()
{

}

void TableMenuWidget::updateColumn(QTableWidgetItem *item)
{
	int rowNum = item->row();
	int columnNum = item->column();

	Id columnId = Id::loadFromString(mUi->columnDataTable->item(rowNum, 2)->text());
	NodeElement *column = mEditorViewScene->getNodeById(columnId);
	switch (columnNum) {
	case 0: {
		column->setProperty("Name", item->text());
		break;
	}
	case 1: {
		column->setProperty("DataType", item->text());
		break;
	}
	}
}

void TableMenuWidget::fillGeneralTab()
{
	NodeElement *table = mEditorViewScene->getNodeById(mId);
	QVariant tableName = table->getProperty("Name");


	QList<NodeElement *> columns = table->childNodes();
	for (NodeElement *column : columns) {
		QVariant columnName = column->getProperty("Name");
		QVariant columnType = column->getProperty("DataType");
		int rowCount = mUi->columnDataTable->rowCount();
		mUi->columnDataTable->insertRow(rowCount);
		mUi->columnDataTable->setItem(rowCount, 0, new QTableWidgetItem(columnName.toString()));
		mUi->columnDataTable->setItem(rowCount, 1, new QTableWidgetItem(columnType.toString()));
		mUi->columnDataTable->setItem(rowCount, 2, new QTableWidgetItem(column->id().toString()));
	}
}

}
