/* Copyright 2015-2016 Anastasia Semenova
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "tableMenuWidget.h"

#include "editorViewScene.h"

#include "ui_tableMenuWidget.h"

namespace qReal {
namespace gui {
namespace editor {

class EditorViewScene;

TableMenuWidget::TableMenuWidget(const Id &id
		, EditorViewScene *editorViewScene
		, models::Models &models
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::TableMenuWidget)
	, mId(id)
	, mEditorViewScene(editorViewScene)
	, mModels(models)
	, mDbmsName(id.editor())
{
	mUi->setupUi(this);
	this->setModal(true);

	mTableNodeElement = mEditorViewScene->getNodeById(mId);

	fillTableProperties();
	fillColumnProperties();
	fillIndexProperties();

	setPropertiesForDbms();

	connect(mUi->tableDataTable, SIGNAL(itemChanged(QTableWidgetItem*))
			, this, SLOT(updateTable(QTableWidgetItem*)));
	connect(mUi->columnDataTable, SIGNAL(itemChanged(QTableWidgetItem*))
			, this, SLOT(updateColumn(QTableWidgetItem*)));
	connect(mUi->indexDataTable, SIGNAL(itemChanged(QTableWidgetItem*))
			, this, SLOT(updateIndex(QTableWidgetItem*)));

	connect(mUi->keyGroupsText, SIGNAL(textChanged()), this, SLOT(updateKeyGroups()));
	connect(mUi->commentText, SIGNAL(textChanged()), this, SLOT(updateComment()));
	connect(mUi->sqlQueryText, SIGNAL(textChanged()), this, SLOT(updateQuery()));

	connect(mUi->addIndexButton, SIGNAL(clicked()), this, SLOT(addIndex()));
	connect(mUi->deleteIndexButton, SIGNAL(clicked()), this, SLOT(deleteIndex()));
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

void TableMenuWidget::closeEvent(QCloseEvent *event)
{
	emit(closed());
	event->accept();
}

void TableMenuWidget::updateTable(QTableWidgetItem *item)
{
	int rowNum = item->row();
	NodeElement *table = mEditorViewScene->getNodeById(mId);

	QString cellContents = item->text();
	switch(rowNum) {
	case TablePropertyRowNumber::TableName: {
		table->setProperty("tableName", cellContents);
		break;
	}
	case TablePropertyRowNumber::Temp: {
		table->setProperty("temp", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::Temporary: {
		table->setProperty("temporary", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::IfNotExists: {
		table->setProperty("if_not_exists", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::AvgRowLength: {
		table->setProperty("avg_row_length", cellContents);
		break;
	}
	case TablePropertyRowNumber::CheckSum: {
		table->setProperty("check_sum", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::MaxRows: {
		table->setProperty("max_rows", cellContents);
		break;
	}
	case TablePropertyRowNumber::MinRows: {
		table->setProperty("min_rows", cellContents);
		break;
	}
	case TablePropertyRowNumber::PackKeys: {
		table->setProperty("pack_keys", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::DelayKeyWrite: {
		table->setProperty("if_not_exists", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::TableAutoIncrement: {
		table->setProperty("auto_increment", item->checkState() == Qt::Checked);
		break;
	}
	case TablePropertyRowNumber::TableType: {
		table->setProperty("tableType", cellContents);
		break;
	}
	case TablePropertyRowNumber::WithoutRowid: {
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
	case ColumnPropertyColumnNumber::Name: {
		column->setProperty("columnName", cellContents);
		break;
	}
	case ColumnPropertyColumnNumber::DataType: {
		column->setProperty("DataType", cellContents);
		break;
	}
	case ColumnPropertyColumnNumber::IsPrimaryKey: {
		column->setProperty("isPrimaryKey", item->checkState() == Qt::Checked);
		break;
	}
	case ColumnPropertyColumnNumber::IsUnique: {
		column->setProperty("isUnique", item->checkState() == Qt::Checked);
		break;
	}
	case ColumnPropertyColumnNumber::NotNull: {
		column->setProperty("notNull", item->checkState() == Qt::Checked);
		break;
	}
	case ColumnPropertyColumnNumber::Null: {
		column->setProperty("isUnique", item->checkState() == Qt::Checked);
		break;
	}
	case ColumnPropertyColumnNumber::Default: {
		column->setProperty("default", cellContents);
		break;
	}
	case ColumnPropertyColumnNumber::WithCompression: {
		column->setProperty("with_compression", item->checkState() == Qt::Checked);
		break;
	}
	case ColumnPropertyColumnNumber::WithComp: {
		column->setProperty("with_comp", item->checkState() == Qt::Checked);
		break;
	}
	case ColumnPropertyColumnNumber::ColumnAutoIncrement:
		column->setProperty("auto_increment", item->checkState() == Qt::Checked);
		break;
	}
}

void TableMenuWidget::updateIndex(QTableWidgetItem *item)
{
	int rowNum = item->row();
	int columnNum = item->column();

	Id indexId = Id::loadFromString(mUi->indexDataTable->item(rowNum, ElementId)->text());
	QString cellContents = item->text();
	switch (columnNum) {
	case IndexPropertyColumnNumber::IndexName: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "indexName", cellContents);
		break;
	}
	case IndexPropertyColumnNumber::ColumnNames: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "columnNames", cellContents);
		break;
	}
	case IndexPropertyColumnNumber::IsUniqueIndex: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "isUnique", item->checkState() == Qt::Checked);
		break;
	}
	case IndexPropertyColumnNumber::Clustered: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "clustered", item->checkState() == Qt::Checked);
		break;
	}
	case IndexPropertyColumnNumber::IndexIfNotExists: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "if_not_exists", item->checkState() == Qt::Checked);
		break;
	}
	case IndexPropertyColumnNumber::Fulltext: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "fulltext", item->checkState() == Qt::Checked);
		break;
	}
	case IndexPropertyColumnNumber::Concurrently: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "concurrently", item->checkState() == Qt::Checked);
		break;
	}
	case IndexPropertyColumnNumber::Method: {
		mModels.mutableLogicalRepoApi().setProperty(indexId, "method", item->checkState() == Qt::Checked);
		break;
	}
	}
}

void TableMenuWidget::updateKeyGroups()
{
	QString keyGroupsText = mUi->keyGroupsText->toPlainText();
	mTableNodeElement->setProperty("key_groups", keyGroupsText);
}

void TableMenuWidget::updateComment()
{
	QString comment = mUi->commentText->toPlainText();
	mTableNodeElement->setProperty("comment", comment);
}

void TableMenuWidget::updateQuery()
{
	QString query = mUi->sqlQueryText->toPlainText();
	mTableNodeElement->setProperty("query", query);
}

void TableMenuWidget::hideUncommonRowsAndColumns()
{
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Local);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Global);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Temp);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Temporary);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Unlogged);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::IfNotExists);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::AvgRowLength);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::CheckSum);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::MaxRows);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::MinRows);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::PackKeys);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::DelayKeyWrite);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::TableAutoIncrement);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::TableType);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::WithoutRowid);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Inherits);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::With);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::WithOids);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::WithoutOids);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::OnCommit);
	mUi->tableDataTable->hideRow(TablePropertyRowNumber::Tablespace);

	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::ElementId);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::IsPrimaryKey);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::IsUnique);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::NotNull);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::Null);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::Default);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::WithCompression);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::WithComp);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::ColumnAutoIncrement);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::Check);
	mUi->columnDataTable->hideColumn(ColumnPropertyColumnNumber::ElementId);

	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::IndexId);
	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::IsUniqueIndex);
	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::Clustered);
	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::IndexIfNotExists);
	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::Fulltext);
	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::Concurrently);
	mUi->indexDataTable->hideColumn(IndexPropertyColumnNumber::Method);
}

void TableMenuWidget::showSqliteRowsAndColumns()
{
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Temp);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Temporary);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::IfNotExists);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::WithoutRowid);

	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsPrimaryKey);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsUnique);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::NotNull);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::ColumnAutoIncrement);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Default);

	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::IsUniqueIndex);
	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::IndexIfNotExists);
}

void TableMenuWidget::showSqlServer2008RowsAndColumns()
{
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsPrimaryKey);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsUnique);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::NotNull);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Null);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Default);

	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::IsUniqueIndex);
	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::Clustered);
}

void TableMenuWidget::showMySql5RowsAndColumns()
{
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Temporary);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::IfNotExists);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::AvgRowLength);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::CheckSum);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::MaxRows);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::MinRows);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::PackKeys);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::DelayKeyWrite);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::TableAutoIncrement);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::TableType);

	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsPrimaryKey);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsUnique);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::NotNull);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Null);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::ColumnAutoIncrement);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Default);

	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::IsUniqueIndex);
	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::Fulltext);
}

void TableMenuWidget::showMicrosoftAccessRowsAndColumns()
{
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Temporary);

	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsPrimaryKey);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsUnique);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::NotNull);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::WithCompression);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::WithComp);
}

void TableMenuWidget::showPostgreSqlRowsAndColumns()
{
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Local);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Global);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Temp);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Temporary);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Unlogged);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::IfNotExists);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Inherits);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::With);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::WithOids);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::WithoutOids);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::OnCommit);
	mUi->tableDataTable->showRow(TablePropertyRowNumber::Tablespace);

	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsPrimaryKey);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::IsUnique);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::NotNull);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Null);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Check);
	mUi->columnDataTable->showColumn(ColumnPropertyColumnNumber::Default);

	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::IsUniqueIndex);
	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::Concurrently);
	mUi->indexDataTable->showColumn(IndexPropertyColumnNumber::Method);
}

void TableMenuWidget::setPropertiesForDbms()
{
	hideUncommonRowsAndColumns();
	if (mDbmsName == "Sqlite") {
		showSqliteRowsAndColumns();
	}
	else if (mDbmsName == "SqlServer2008") {
		showSqlServer2008RowsAndColumns();
	}
	else if (mDbmsName == "MySql5") {
		showMySql5RowsAndColumns();
	}
	else if (mDbmsName == "MicrosoftAccess") {
		showMicrosoftAccessRowsAndColumns();

		mUi->addIndexButton->setEnabled(false);
		mUi->deleteIndexButton->setEnabled(false);
	}
	else if (mDbmsName == "PostgreSql") {
		showPostgreSqlRowsAndColumns();
	}
}

void TableMenuWidget::fillTableProperties()
{
	int columnNum = 0;

	QVariant tableName = mTableNodeElement->getProperty("tableName");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::TableName
			, columnNum, new QTableWidgetItem(tableName.toString()));
	QString windowTitle = tr("Menu of table with name '") + tableName.toString() + tr("'");
	this->setWindowTitle(windowTitle);

	QVariant temp = mTableNodeElement->getProperty("temp");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::Temp
			, columnNum, new QTableWidgetItem());
	if (temp.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::Temp
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::Temp
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant temporary = mTableNodeElement->getProperty("temporary");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::Temporary
			, columnNum, new QTableWidgetItem());
	if (temporary.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::Temporary
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::Temporary
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant ifNotExists = mTableNodeElement->getProperty("if_not_exists");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::IfNotExists
			, columnNum, new QTableWidgetItem());
	if (ifNotExists.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::IfNotExists
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::IfNotExists
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant avgRowLength = mTableNodeElement->getProperty("avg_row_length");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::AvgRowLength
			, columnNum, new QTableWidgetItem(avgRowLength.toString()));

	QVariant checkSum = mTableNodeElement->getProperty("check_sum");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::CheckSum
			, columnNum, new QTableWidgetItem());
	if (checkSum.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::CheckSum
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::CheckSum
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant maxRows = mTableNodeElement->getProperty("max_rows");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::MaxRows
			, columnNum, new QTableWidgetItem(maxRows.toString()));

	QVariant minRows = mTableNodeElement->getProperty("min_rows");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::MinRows
			, columnNum, new QTableWidgetItem(minRows.toString()));

	QVariant packKeys = mTableNodeElement->getProperty("pack_keys");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::PackKeys
			, columnNum, new QTableWidgetItem());
	if (packKeys.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::PackKeys
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::PackKeys
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant delayKeyWrite = mTableNodeElement->getProperty("delay_key_write");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::DelayKeyWrite
			, columnNum, new QTableWidgetItem());
	if (delayKeyWrite.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::DelayKeyWrite
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::DelayKeyWrite
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant autoIncrement = mTableNodeElement->getProperty("auto_increment");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::TableAutoIncrement
			, columnNum, new QTableWidgetItem());
	if (autoIncrement.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::TableAutoIncrement
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::TableAutoIncrement
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant tableType = mTableNodeElement->getProperty("type");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::TableType
			, columnNum, new QTableWidgetItem(tableType.toString()));

	QVariant withoutRowid = mTableNodeElement->getProperty("without_rowid");
	mUi->tableDataTable->setItem(TablePropertyRowNumber::WithoutRowid
			, columnNum, new QTableWidgetItem());
	if (withoutRowid.toBool())
		mUi->tableDataTable->item(TablePropertyRowNumber::WithoutRowid
			, columnNum)->setCheckState(Qt::Checked);
	else
		mUi->tableDataTable->item(TablePropertyRowNumber::WithoutRowid
			, columnNum)->setCheckState(Qt::Unchecked);

	QVariant keyGroupsText = mTableNodeElement->getProperty("key_groups");
	mUi->keyGroupsText->setText(keyGroupsText.toString());

	QVariant comment = mTableNodeElement->getProperty("comment");
	mUi->commentText->setText(comment.toString());

	QVariant query = mTableNodeElement->getProperty("query");
	mUi->sqlQueryText->setText(query.toString());
}

void TableMenuWidget::fillColumnProperties()
{
	QList<NodeElement *> columns = mTableNodeElement->childNodes();
	for (NodeElement *column : columns) {
		// invisible id
		QString pr = column->getProperty("parent").toString();
		int rowCount = mUi->columnDataTable->rowCount();
		mUi->columnDataTable->insertRow(rowCount);
		mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::ElementId
				, new QTableWidgetItem(column->id().toString()));

		QVariant columnName = column->getProperty("columnName");
		mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::Name
				, new QTableWidgetItem(columnName.toString()));

		QVariant columnType = column->getProperty("DataType");
			mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::DataType
				, new QTableWidgetItem(columnType.toString()));

		QVariant isPrimaryKey = column->getProperty("isPrimaryKey");
		mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::IsPrimaryKey
				, new QTableWidgetItem(isPrimaryKey.toString()));
		if (isPrimaryKey.toBool())
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::IsPrimaryKey)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::IsPrimaryKey)->setCheckState(Qt::Unchecked);

		QVariant isUnique = column->getProperty("isUnique");
		mUi->columnDataTable->setItem(rowCount
				, ColumnPropertyColumnNumber::IsUnique, new QTableWidgetItem(isUnique.toString()));
		if (isUnique.toBool())
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::IsUnique)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::IsUnique)->setCheckState(Qt::Unchecked);

		QVariant notNull = column->getProperty("notNull");
		mUi->columnDataTable->setItem(rowCount
				, ColumnPropertyColumnNumber::NotNull, new QTableWidgetItem(notNull.toString()));
		if (notNull.toBool())
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::NotNull)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::NotNull)->setCheckState(Qt::Unchecked);

		QVariant null = column->getProperty("null");
		mUi->columnDataTable->setItem(rowCount
				, ColumnPropertyColumnNumber::Null, new QTableWidgetItem(null.toString()));
		if (null.toBool())
				mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::Null)->setCheckState(Qt::Checked);
		else
				mUi->columnDataTable->item(rowCount
						, ColumnPropertyColumnNumber::Null)->setCheckState(Qt::Unchecked);

		QVariant defaultProperty = column->getProperty("default");
		mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::Default
				, new QTableWidgetItem(defaultProperty.toString()));

		QVariant withCompression = column->getProperty("with_compression");
		mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::WithCompression
				, new QTableWidgetItem(withCompression.toString()));
		if (withCompression.toBool())
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::WithCompression)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::WithCompression)->setCheckState(Qt::Unchecked);

		QVariant withComp = column->getProperty("with_comp");
		mUi->columnDataTable->setItem(rowCount, ColumnPropertyColumnNumber::WithComp
				, new QTableWidgetItem(withComp.toString()));
		if (withComp.toBool())
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::WithComp)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::WithComp)->setCheckState(Qt::Unchecked);

		QVariant autoIncrement = column->getProperty("auto_increment");
		mUi->columnDataTable->setItem(rowCount
				, ColumnPropertyColumnNumber::ColumnAutoIncrement, new QTableWidgetItem());
		if (autoIncrement.toBool())
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::ColumnAutoIncrement)->setCheckState(Qt::Checked);
		else
			mUi->columnDataTable->item(rowCount
					, ColumnPropertyColumnNumber::ColumnAutoIncrement)->setCheckState(Qt::Unchecked);
	}
}

void TableMenuWidget::fillIndexProperties()
{
	IdList ids = mModels.logicalModelAssistApi().children(Id::rootId());
	for (Id const &id : ids) {
		QString tableName = mModels.mutableLogicalRepoApi().property(id, "tableName").toString();
		if (id.element() == "Index" && id.editor() == mDbmsName && tableName == mId.toString()) {
			int rowCount = mUi->indexDataTable->rowCount();
			mUi->indexDataTable->insertRow(rowCount);
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::IndexId
					, new QTableWidgetItem(id.toString()));

			QString indexName = mModels.mutableLogicalRepoApi().property(id, "indexName").toString();
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::IndexName
					, new QTableWidgetItem(indexName));


			QString columnNames = mModels.mutableLogicalRepoApi().property(id, "columnNames").toString();
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::ColumnNames
					, new QTableWidgetItem(columnNames));

			QVariant isUnique = mModels.mutableLogicalRepoApi().property(id, "isUnique").toString();
			mUi->indexDataTable->setItem(rowCount
					, IndexPropertyColumnNumber::IsUniqueIndex, new QTableWidgetItem());
			if (isUnique.toBool()) {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::IsUniqueIndex)->setCheckState(Qt::Checked);
			} else {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::IsUniqueIndex)->setCheckState(Qt::Unchecked);
			}

			QVariant clustered = mModels.mutableLogicalRepoApi().property(id, "clustered");
			mUi->indexDataTable->setItem(rowCount
					, IndexPropertyColumnNumber::Clustered, new QTableWidgetItem());
			if (clustered.toBool()) {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Clustered)->setCheckState(Qt::Checked);
			} else {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Clustered)->setCheckState(Qt::Unchecked);
			}

			QVariant ifNotExists = mModels.mutableLogicalRepoApi().property(id, "if_not_exists");
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::IndexIfNotExists
					, new QTableWidgetItem());
			if (ifNotExists.toBool()) {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::IndexIfNotExists)->setCheckState(Qt::Checked);
			} else {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::IndexIfNotExists)->setCheckState(Qt::Unchecked);
			}

			QVariant fulltext = mModels.mutableLogicalRepoApi().property(id, "fulltext");
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::Fulltext
					, new QTableWidgetItem());
			if (fulltext.toBool()) {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Fulltext)->setCheckState(Qt::Checked);
			} else {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Fulltext)->setCheckState(Qt::Unchecked);
			}

			QVariant concurrently = mModels.mutableLogicalRepoApi().property(id, "concurrently");
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::Concurrently
					, new QTableWidgetItem());
			if (concurrently.toBool()) {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Concurrently)->setCheckState(Qt::Checked);
			} else {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Concurrently)->setCheckState(Qt::Unchecked);
			}

			QVariant method = mModels.mutableLogicalRepoApi().property(id, "method");
			mUi->indexDataTable->setItem(rowCount, IndexPropertyColumnNumber::Method
					, new QTableWidgetItem());
			if (method.toBool()) {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Method)->setCheckState(Qt::Checked);
			}
			else {
				mUi->indexDataTable->item(rowCount
						, IndexPropertyColumnNumber::Method)->setCheckState(Qt::Unchecked);
			}
		}
	}
}

void TableMenuWidget::addIndex()
{
	const Id id = Id::loadFromString(QString("qrm:/" + mDbmsName
			+ "/DatabasesPhysicalModelMetamodel/Index"));

	Id logicalId = mModels.logicalModelAssistApi().createElement(Id::rootId(), id);

	int rowCount = mUi->indexDataTable->rowCount();
	mUi->indexDataTable->insertRow(rowCount);
	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::IndexId, new QTableWidgetItem(logicalId.toString()));

	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::IsUniqueIndex, new QTableWidgetItem());
	mUi->indexDataTable->item(rowCount
			, IndexPropertyColumnNumber::IsUniqueIndex)->setCheckState(Qt::Unchecked);

	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::Clustered, new QTableWidgetItem());
	mUi->indexDataTable->item(rowCount
			, IndexPropertyColumnNumber::Clustered)->setCheckState(Qt::Unchecked);

	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::IndexIfNotExists, new QTableWidgetItem());
	mUi->indexDataTable->item(rowCount
			, IndexPropertyColumnNumber::IndexIfNotExists)->setCheckState(Qt::Unchecked);

	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::Fulltext, new QTableWidgetItem());
	mUi->indexDataTable->item(rowCount
			, IndexPropertyColumnNumber::Fulltext)->setCheckState(Qt::Unchecked);

	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::Concurrently, new QTableWidgetItem());
	mUi->indexDataTable->item(rowCount
			, IndexPropertyColumnNumber::Concurrently)->setCheckState(Qt::Unchecked);

	mUi->indexDataTable->setItem(rowCount
			, IndexPropertyColumnNumber::Method, new QTableWidgetItem());
	mUi->indexDataTable->item(rowCount
			, IndexPropertyColumnNumber::Method)->setCheckState(Qt::Unchecked);


	QString tableId = mId.toString();
	QVariant tableIdVar(tableId);
	mModels.mutableLogicalRepoApi().setProperty(logicalId, "tableName", tableIdVar);
}

void TableMenuWidget::deleteIndex()
{
	QList<QTableWidgetItem *> selectedItems = mUi->indexDataTable->selectedItems();
	for (QTableWidgetItem *item : selectedItems) {
		int row = item->row();
		Id indexId = Id::loadFromString(mUi->indexDataTable->item(row
				, IndexPropertyColumnNumber::IndexId)->text());
		mModels.logicalModelAssistApi().mutableLogicalRepoApi().removeElement(indexId);
		mUi->indexDataTable->removeRow(row);
	}
}

}
}
}
