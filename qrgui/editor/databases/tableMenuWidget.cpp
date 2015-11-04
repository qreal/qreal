#include "tableMenuWidget.h"
#include "ui_tableMenuWidget.h"

namespace qReal {

class EditorViewScene;

TableMenuWidget::TableMenuWidget(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::TableMenuWidget)
{
	mUi->setupUi(this);
	this->setVisible(false);

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
}
