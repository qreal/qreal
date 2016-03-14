/* Copyright 2014-2016 Anastasia Semenova
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
#include <QWidget>
#include <QTableWidgetItem>
#include <QDialog>

#include "qrgui/editor/private/exploserView.h"

namespace Ui {
class TableMenuWidget;
}

namespace qReal {
namespace gui {
namespace editor {

class EditorViewScene;
class NodeElement;

class TableMenuWidget : public QDialog
{
	Q_OBJECT

public:
	TableMenuWidget(const Id &id, EditorViewScene *editorViewScene, QWidget *parent = 0);

	~TableMenuWidget();
public slots:
	void open();
	void close();
	void updateTable(QTableWidgetItem *item);
	void updateColumn(QTableWidgetItem *item);
	void updateComment();
	void updateQuery();
	// Fill columnNames depending on the dbms (different set of properties)
	void setPropertiesForDbms();

private:
	enum TablePropertyRowNumber {
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
	enum ColumnPropertyColumnNumber {
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

	QString mDbmsName;

	void fillTableProperties();
	void fillColumnProperties();

	Ui::TableMenuWidget *mUi;
	const Id mId;
	NodeElement *mTableNodeElement;
	EditorViewScene *mEditorViewScene;
};
}
}
}

