#include "tableMenuWidget.h"
#include "ui_tableMenuWidget.h"
#include "editorViewScene.h"

namespace qReal {

class EditorViewScene;

TableMenuWidget::TableMenuWidget(const Id &id, EditorViewScene *editorViewScene, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::TableMenuWidget)
	, mId(id)
	, mEditorViewScene(editorViewScene)
{
	mUi->setupUi(this);
	this->setVisible(false);
	fillGeneralTab();
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
	}
	int u = 9;
}

}
