#pragma once
#include <QWidget>
#include <QTableWidgetItem>
#include <QDialog>

#include "qrgui/editor/private/exploserView.h"

namespace Ui {
class TableMenuWidget;
}

namespace qReal {

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
	// Fill columnNames depending on the dbms (different set of properties)
	void setPropertiesForDbms();

private:
	enum TablePropertyRowNumber {
		TableName = 0
		, Temp
		, Temporary
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
		 , ColumnAutoIncrement };

	QString mDbmsName;

	void fillTableProperties();
	void fillColumnProperties();

	Ui::TableMenuWidget *mUi;
	const Id mId;
	NodeElement *mTableNodeElement;
	EditorViewScene *mEditorViewScene;
};
}

