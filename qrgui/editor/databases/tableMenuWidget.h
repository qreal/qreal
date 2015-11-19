#pragma once
#include <QWidget>

#include "qrgui/editor/private/exploserView.h"

namespace Ui {
class TableMenuWidget;
}

namespace qReal {

class EditorViewScene;

class TableMenuWidget : public QWidget
{
	Q_OBJECT

public:
	TableMenuWidget(const Id &id, EditorViewScene *editorViewScene, QWidget *parent = 0);

	~TableMenuWidget();
public slots:
	void open();
	void close();

private:
	void fillGeneralTab();

	Ui::TableMenuWidget *mUi;
	const Id mId;
	EditorViewScene *mEditorViewScene;
};
}

