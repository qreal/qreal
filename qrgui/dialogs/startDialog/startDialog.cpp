#include <QtGui/QTabWidget>
#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "../mainwindow/mainWindow.h"

qReal::StartDialog::StartDialog(MainWindow *mainWindow, QWidget *parent)
	: QDialog(parent)
	, mMainWindow(mainWindow)
	, mTabWidget(new QTabWidget(this))
{
	setFixedSize(512, 384);
	mTabWidget->addTab(new SuggestToCreateDiagramWidget(mMainWindow, this), tr("New project with diagram"));
}
