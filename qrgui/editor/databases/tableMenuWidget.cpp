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
	, mDbmsName(id.editor())
{
	mUi->setupUi(this);
	this->setModal(true);
	fillGeneralTab();

	setColumnPropertiesForDbms();

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

	Id columnId = Id::loadFromString(mUi->columnDataTable->item(rowNum, ElementId)->text());
	NodeElement *column = mEditorViewScene->getNodeById(columnId);

	QString cellContents = item->text();
	switch (columnNum) {
	case Name: {
		column->setProperty("Name", cellContents);
		break;
	}
	case DataType: {
		column->setProperty("DataType", cellContents);
		break;
	}
	case IsPrimaryKey: {
		column->setProperty("isPrimaryKey", item->checkState() == Qt::Checked);
		break;
	}
	case IsUnique: {
		column->setProperty("isUnique", item->checkState() == Qt::Checked);
		break;
	}
	case NotNull: {
		column->setProperty("notNull", item->checkState() == Qt::Checked);
		break;
	}
	case Null: {
		column->setProperty("isUnique", item->checkState() == Qt::Checked);
		break;
	}
	case Default: {
		column->setProperty("default", cellContents);
		break;
	}
	case WithCompression: {
		column->setProperty("with_compression", item->checkState() == Qt::Checked);
		break;
	}
	case WithComp: {
		column->setProperty("with_comp", item->checkState() == Qt::Checked);
		break;
	}
	case AutoIncrement:
		column->setProperty("auto_increment", item->checkState() == Qt::Checked);
		break;
	}
}

void TableMenuWidget::setColumnPropertiesForDbms()
{
	mUi->columnDataTable->hideColumn(ElementId);
	if (mDbmsName == "Sqlite") {
		mUi->columnDataTable->hideColumn(Null);
		mUi->columnDataTable->hideColumn(WithCompression);
		mUi->columnDataTable->hideColumn(WithComp);
	}
	else if (mDbmsName == "SqlServer2008") {
		mUi->columnDataTable->hideColumn(WithCompression);
		mUi->columnDataTable->hideColumn(WithComp);
		mUi->columnDataTable->hideColumn(AutoIncrement);
	}
	else if (mDbmsName == "MySql5") {

		mUi->columnDataTable->hideColumn(WithCompression);
		mUi->columnDataTable->hideColumn(WithComp);
	}
	else if (mDbmsName == "MicrosoftAccess") {
		mUi->columnDataTable->hideColumn(Null);
		mUi->columnDataTable->hideColumn(Default);
		mUi->columnDataTable->hideColumn(AutoIncrement);
	}
}

void TableMenuWidget::fillGeneralTab()
{
	NodeElement *table = mEditorViewScene->getNodeById(mId);
	QVariant tableName = table->getProperty("Name");


	QList<NodeElement *> columns = table->childNodes();
	for (NodeElement *column : columns) {
		// invisible id
		int rowCount = mUi->columnDataTable->rowCount();
		mUi->columnDataTable->insertRow(rowCount);
		mUi->columnDataTable->setItem(rowCount, ElementId, new QTableWidgetItem(column->id().toString()));

		QVariant columnName = column->getProperty("Name");
		mUi->columnDataTable->setItem(rowCount, Name, new QTableWidgetItem(columnName.toString()));

		QVariant columnType = column->getProperty("DataType");
		mUi->columnDataTable->setItem(rowCount, DataType, new QTableWidgetItem(columnType.toString()));

		QVariant isPrimaryKey = column->getProperty("isPrimaryKey");
		mUi->columnDataTable->setItem(rowCount, IsPrimaryKey, new QTableWidgetItem(isPrimaryKey.toString()));
		if (isPrimaryKey.toBool())
			mUi->columnDataTable->item(rowCount, IsPrimaryKey)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, IsPrimaryKey)->setCheckState(Qt::Unchecked);

		QVariant isUnique = column->getProperty("isUnique");
		mUi->columnDataTable->setItem(rowCount, IsUnique, new QTableWidgetItem(isUnique.toString()));
		if (isUnique.toBool())
			mUi->columnDataTable->item(rowCount, IsUnique)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, IsUnique)->setCheckState(Qt::Unchecked);

		QVariant notNull = column->getProperty("notNull");
		mUi->columnDataTable->setItem(rowCount, NotNull, new QTableWidgetItem(notNull.toString()));
		if (notNull.toBool())
			mUi->columnDataTable->item(rowCount, NotNull)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, NotNull)->setCheckState(Qt::Unchecked);

		QVariant null = column->getProperty("null");
		mUi->columnDataTable->setItem(rowCount, Null, new QTableWidgetItem(null.toString()));
		if (null.toBool())
			mUi->columnDataTable->item(rowCount, Null)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, Null)->setCheckState(Qt::Unchecked);

		QVariant defaultProperty = column->getProperty("default");
		mUi->columnDataTable->setItem(rowCount, Default, new QTableWidgetItem(defaultProperty.toString()));

		QVariant withCompression = column->getProperty("with_compression");
		mUi->columnDataTable->setItem(rowCount, WithCompression, new QTableWidgetItem(withCompression.toString()));
		if (withCompression.toBool())
			mUi->columnDataTable->item(rowCount, WithCompression)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, WithCompression)->setCheckState(Qt::Unchecked);


		QVariant withComp = column->getProperty("with_comp");
		mUi->columnDataTable->setItem(rowCount, WithComp, new QTableWidgetItem(withComp.toString()));
		if (withComp.toBool())
			mUi->columnDataTable->item(rowCount, WithComp)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, WithComp)->setCheckState(Qt::Unchecked);

		QVariant autoIncrement = column->getProperty("auto_increment");
		mUi->columnDataTable->setItem(rowCount, AutoIncrement, new QTableWidgetItem());
		if (autoIncrement.toBool())
			mUi->columnDataTable->item(rowCount, AutoIncrement)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, AutoIncrement)->setCheckState(Qt::Unchecked);
	}
}

}
