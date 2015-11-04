#pragma once
#include <QWidget>

namespace Ui {
class TableMenuWidget;
}

namespace qReal {

class EditorViewScene;

class TableMenuWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TableMenuWidget(QWidget *parent = 0);
	~TableMenuWidget();
public slots:
	void open();

private:
	Ui::TableMenuWidget *mUi;
};
}

