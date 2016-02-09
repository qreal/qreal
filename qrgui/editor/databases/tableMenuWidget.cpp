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

	mTableNodeElement = mEditorViewScene->getNodeById(mId);

	fillTableProperties();
	fillColumnProperties();

	setPropertiesForDbms();

	connect(mUi->tableDataTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateTable(QTableWidgetItem*)));
	connect(mUi->columnDataTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateColumn(QTableWidgetItem*)));
	connect(mUi->commentText, SIGNAL(textChanged()), this, SLOT(updateComment()));
	connect(mUi->sqlQueryText, SIGNAL(textChanged()), this, SLOT(updateQuery()));
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

void TableMenuWidget::updateTable(QTableWidgetItem *item)
{
	int rowNum = item->row();
	NodeElement *table = mEditorViewScene->getNodeById(mId);

	QString cellContents = item->text();
	switch(rowNum) {
	case TableName: {
		table->setProperty("Name", cellContents);
		break;
	}
	case Temp: {
		table->setProperty("temp", item->checkState() == Qt::Checked);
		break;
	}
	case Temporary: {
		table->setProperty("temporary", item->checkState() == Qt::Checked);
		break;
	}
	case IfNotExists: {
		table->setProperty("if_not_exists", item->checkState() == Qt::Checked);
		break;
	}
	case AvgRowLength: {
		table->setProperty("avg_row_length", cellContents);
		break;
	}
	case CheckSum: {
		table->setProperty("check_sum", item->checkState() == Qt::Checked);
		break;
	}
	case MaxRows: {
		table->setProperty("max_rows", cellContents);
		break;
	}
	case MinRows: {
		table->setProperty("min_rows", cellContents);
		break;
	}
	case PackKeys: {
		table->setProperty("pack_keys", item->checkState() == Qt::Checked);
		break;
	}
	case DelayKeyWrite: {
		table->setProperty("if_not_exists", item->checkState() == Qt::Checked);
		break;
	}
	case TableAutoIncrement: {
		table->setProperty("auto_increment", item->checkState() == Qt::Checked);
		break;
	}
	case TableType: {
		table->setProperty("tableType", cellContents);
		break;
	}
	case WithoutRowid: {
		table->setProperty("without_rowid", item->checkState() == Qt::Checked);
		break;
	}
	}
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
	case ColumnAutoIncrement:
		column->setProperty("auto_increment", item->checkState() == Qt::Checked);
		break;
	}
}

void TableMenuWidget::updateComment()
{
	QString comment = mUi->commentText->toPlainText();
	NodeElement *table = mEditorViewScene->getNodeById(mId);
	table->setProperty("comment", comment);
}

void TableMenuWidget::updateQuery()
{
	QString query = mUi->sqlQueryText->toPlainText();
	NodeElement *table = mEditorViewScene->getNodeById(mId);
	table->setProperty("query", query);
}

void TableMenuWidget::setPropertiesForDbms()
{
	mUi->columnDataTable->hideColumn(ElementId);
	if (mDbmsName == "Sqlite") {
		mUi->columnDataTable->hideColumn(Null);
		mUi->columnDataTable->hideColumn(WithCompression);
		mUi->columnDataTable->hideColumn(WithComp);

		mUi->tableDataTable->hideRow(AvgRowLength);
		mUi->tableDataTable->hideRow(CheckSum);
		mUi->tableDataTable->hideRow(MaxRows);
		mUi->tableDataTable->hideRow(MinRows);
		mUi->tableDataTable->hideRow(PackKeys);
		mUi->tableDataTable->hideRow(DelayKeyWrite);
		mUi->tableDataTable->hideRow(TableAutoIncrement);
		mUi->tableDataTable->hideRow(TableType);
	}
	else if (mDbmsName == "SqlServer2008") {
		mUi->columnDataTable->hideColumn(WithCompression);
		mUi->columnDataTable->hideColumn(WithComp);
		mUi->columnDataTable->hideColumn(ColumnAutoIncrement);

		mUi->tableDataTable->hideRow(Temp);
		mUi->tableDataTable->hideRow(Temporary);
		mUi->tableDataTable->hideRow(IfNotExists);
		mUi->tableDataTable->hideRow(AvgRowLength);
		mUi->tableDataTable->hideRow(CheckSum);
		mUi->tableDataTable->hideRow(MaxRows);
		mUi->tableDataTable->hideRow(MinRows);
		mUi->tableDataTable->hideRow(PackKeys);
		mUi->tableDataTable->hideRow(DelayKeyWrite);
		mUi->tableDataTable->hideRow(TableAutoIncrement);
		mUi->tableDataTable->hideRow(TableType);
		mUi->tableDataTable->hideRow(WithoutRowid);
	}
	else if (mDbmsName == "MySql5") {

		mUi->columnDataTable->hideColumn(WithCompression);
		mUi->columnDataTable->hideColumn(WithComp);

		mUi->tableDataTable->hideRow(Temp);
		mUi->tableDataTable->hideRow(WithoutRowid);
	}
	else if (mDbmsName == "MicrosoftAccess") {
		mUi->columnDataTable->hideColumn(Null);
		mUi->columnDataTable->hideColumn(Default);
		mUi->columnDataTable->hideColumn(ColumnAutoIncrement);

		mUi->tableDataTable->hideRow(Temp);
		mUi->tableDataTable->hideRow(IfNotExists);
		mUi->tableDataTable->hideRow(AvgRowLength);
		mUi->tableDataTable->hideRow(CheckSum);
		mUi->tableDataTable->hideRow(MaxRows);
		mUi->tableDataTable->hideRow(MinRows);
		mUi->tableDataTable->hideRow(PackKeys);
		mUi->tableDataTable->hideRow(DelayKeyWrite);
		mUi->tableDataTable->hideRow(TableAutoIncrement);
		mUi->tableDataTable->hideRow(TableType);
		mUi->tableDataTable->hideRow(WithoutRowid);
	}
	else if (mDbmsName == "PostgreSql") {

	}
}

void TableMenuWidget::fillTableProperties()
{
	int columnNum = 0;

	QVariant tableName = mTableNodeElement->getProperty("Name");
	mUi->tableDataTable->setItem(TableName, columnNum, new QTableWidgetItem(tableName.toString()));

	QVariant temp = mTableNodeElement->getProperty("temp");
	mUi->tableDataTable->setItem(Temp, columnNum, new QTableWidgetItem());
	if (temp.toBool())
		mUi->tableDataTable->item(Temp, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(Temp, columnNum)->setCheckState(Qt::Unchecked);
	mUi->tableDataTable->item(Temp, columnNum)->setFlags(Qt::NoItemFlags | Qt::ItemIsUserCheckable);

	QVariant temporary = mTableNodeElement->getProperty("temporary");
	mUi->tableDataTable->setItem(Temporary, columnNum, new QTableWidgetItem());
	if (temporary.toBool())
		mUi->tableDataTable->item(Temporary, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(Temporary, columnNum)->setCheckState(Qt::Unchecked);

	QVariant ifNotExists = mTableNodeElement->getProperty("if_not_exists");
	mUi->tableDataTable->setItem(IfNotExists, columnNum, new QTableWidgetItem());
	if (ifNotExists.toBool())
		mUi->tableDataTable->item(IfNotExists, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(IfNotExists, columnNum)->setCheckState(Qt::Unchecked);

	QVariant avgRowLength = mTableNodeElement->getProperty("avg_row_length");
	mUi->tableDataTable->setItem(AvgRowLength, columnNum, new QTableWidgetItem(avgRowLength.toString()));

	QVariant checkSum = mTableNodeElement->getProperty("check_sum");
	mUi->tableDataTable->setItem(CheckSum, columnNum, new QTableWidgetItem());
	if (checkSum.toBool())
		mUi->tableDataTable->item(CheckSum, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(CheckSum, columnNum)->setCheckState(Qt::Unchecked);

	QVariant comment = mTableNodeElement->getProperty("comment");
	mUi->commentText->setText(comment.toString());

	QVariant maxRows = mTableNodeElement->getProperty("max_rows");
	mUi->tableDataTable->setItem(MaxRows, columnNum, new QTableWidgetItem(maxRows.toString()));

	QVariant minRows = mTableNodeElement->getProperty("min_rows");
	mUi->tableDataTable->setItem(MinRows, columnNum, new QTableWidgetItem(minRows.toString()));

	QVariant packKeys = mTableNodeElement->getProperty("pack_keys");
	mUi->tableDataTable->setItem(PackKeys, columnNum, new QTableWidgetItem());
	if (packKeys.toBool())
		mUi->tableDataTable->item(PackKeys, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(PackKeys, columnNum)->setCheckState(Qt::Unchecked);

	QVariant delayKeyWrite = mTableNodeElement->getProperty("delay_key_write");
	mUi->tableDataTable->setItem(DelayKeyWrite, columnNum, new QTableWidgetItem());
	if (delayKeyWrite.toBool())
		mUi->tableDataTable->item(DelayKeyWrite, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(DelayKeyWrite, columnNum)->setCheckState(Qt::Unchecked);

	QVariant autoIncrement = mTableNodeElement->getProperty("auto_increment");
	mUi->tableDataTable->setItem(TableAutoIncrement, columnNum, new QTableWidgetItem());
	if (autoIncrement.toBool())
		mUi->tableDataTable->item(TableAutoIncrement, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TableAutoIncrement, columnNum)->setCheckState(Qt::Unchecked);

	QVariant tableType = mTableNodeElement->getProperty("type");
	mUi->tableDataTable->setItem(TableType, columnNum, new QTableWidgetItem(tableType.toString()));

	QVariant withoutRowid = mTableNodeElement->getProperty("without_rowid");
	mUi->tableDataTable->setItem(WithoutRowid, columnNum, new QTableWidgetItem());
	if (withoutRowid.toBool())
		mUi->tableDataTable->item(WithoutRowid, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(WithoutRowid, columnNum)->setCheckState(Qt::Unchecked);

	QVariant query = mTableNodeElement->getProperty("query");
	mUi->sqlQueryText->setText(query.toString());
}

void TableMenuWidget::fillColumnProperties()
{
	QList<NodeElement *> columns = mTableNodeElement->childNodes();
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
		mUi->columnDataTable->setItem(rowCount, ColumnAutoIncrement, new QTableWidgetItem());
		if (autoIncrement.toBool())
			mUi->columnDataTable->item(rowCount, ColumnAutoIncrement)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount, ColumnAutoIncrement)->setCheckState(Qt::Unchecked);
	}
}

}
