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

class TableMenuWidget : public QDialog
{
	Q_OBJECT

public:
	TableMenuWidget(const Id &id, EditorViewScene *editorViewScene, QWidget *parent = 0);

	~TableMenuWidget();
public slots:
	void open();
	void close();
	void updateColumn(QTableWidgetItem *item);

private:
	void fillGeneralTab();

	Ui::TableMenuWidget *mUi;
	const Id mId;
	EditorViewScene *mEditorViewScene;
};
}

