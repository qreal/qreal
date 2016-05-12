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

#pragma once

#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QDialog>

#include "qrgui/editor/private/exploserView.h"
#include "qrgui/models/models.h"

namespace Ui {
class TableMenuWidget;
}

namespace qReal {
namespace gui {
namespace editor {

class EditorViewScene;
class NodeElement;

// TODO: Divide this class into classes for each dbms (will make the code less messy)
// Note: mUi is better to have a common style
class TableMenuWidget : public QDialog
{
	Q_OBJECT

public:
	TableMenuWidget(const Id &id
		, EditorViewScene *editorViewScene
		, const models::Models &models
		, QWidget *parent = 0);

	~TableMenuWidget();
public slots:
	void open();
	void close();

	/// Updates properties of the element "Table" according to the changes in the widget
	void updateTable(QTableWidgetItem *item);
	/// Updates properties of the elements "Column" according to the changes in the widget
	void updateColumn(QTableWidgetItem *item);
	/// Updates properties of the elements "Index" according to the changes in the widget
	void updateIndex(QTableWidgetItem *item);
	/// Updates property key_groups of the "Table" according to the changes in the widget
	void updateKeyGroups();
	/// Updates property comment of the "Table" according to the changes in the widget
	void updateComment();
	/// Updates property sql_query of the "Table" according to the changes in the widget
	void updateQuery();

	/// Hides all rows and column that can be different for the dbms
	void hideUncommonRowsAndColumns();
	/// Shows rows and columns that exist in Sqlite
	void showSqliteRowsAndColumns();
	/// Shows rows and columns that exist in SqlServer2008
	void showSqlServer2008RowsAndColumns();
	/// Shows rows and columns that exist in MySql5
	void showMySql5RowsAndColumns();
	/// Shows rows and columns that exist in MicrosoftAccess
	void showMicrosoftAccessRowsAndColumns();
	/// Shows rows and columns that exist in PostgreSql
	void showPostgreSqlRowsAndColumns();

	/// Applies previous show-functions depending on the dbms
	void setPropertiesForDbms();

	/// Adds a new index to the table and in the repository
	void addIndex();
	/// Deletes the index from the table and from the repository
	void deleteIndex();

private slots:
	void closeEvent(QCloseEvent *event);

signals:
	void closed();

private:
	/// List of the table properties which are represented as rows in the widget
	enum TablePropertyRowNumber
	{
		TableName = 0
		, Local
		, Global
		, Temp
		, Temporary
		, Unlogged
		, IfNotExists
		, AvgRowLength
		, CheckSum
		, MaxRows
		, MinRows
		, PackKeys
		, DelayKeyWrite
		, TableAutoIncrement
		, TableType
		, WithoutRowid
		, Inherits
		, With
		, WithOids
		, WithoutOids
		, OnCommit
		, Tablespace
	};

	/// List of the column properties which are represented as columns in the widget
	/// Columns are represented as rows
	enum ColumnPropertyColumnNumber
	{
		ElementId = 0
		 , Name
		 , DataType
		 , IsPrimaryKey
		 , IsUnique
		 , NotNull
		 , Null
		 , Default
		 , WithCompression
		 , WithComp
		 , ColumnAutoIncrement
		 , Check
	};

	/// List of the index properties which are represented as columns in the widget
	/// Indexes are represented as rows
	enum IndexPropertyColumnNumber
	{
		IndexId = 0
		, IndexName
		, ColumnNames
		, IsUniqueIndex
		, Clustered
		, IndexIfNotExists
		, Fulltext
		, Concurrently
		, Method
	};

	QString mDbmsName;

	void fillTableProperties();
	void fillColumnProperties();
	void fillIndexProperties();

	// Have ownership.
	Ui::TableMenuWidget *mUi;
	const Id mId;

	// Doesn't have ownership.
	NodeElement *mTableNodeElement;

	const models::Models &mModels;

	// Doesn't have ownership.
	EditorViewScene *mEditorViewScene;
};
}
}
}

