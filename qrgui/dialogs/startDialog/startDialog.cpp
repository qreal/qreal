#include <QtGui/QTabWidget>
#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"

StartDialog::StartDialog(QWidget *parent)
	: QDialog(parent)
	, mTabWidget(new QTabWidget(this))
{
	SuggestToCreateDiagramWidget *suggestToCreateDiagramWidget = new SuggestToCreateDiagramWidget;
	mTabWidget->addTab(suggestToCreateDiagramWidget, tr("New project with diagram"));
}
