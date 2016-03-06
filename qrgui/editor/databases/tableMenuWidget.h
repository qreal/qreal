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

