#include "diffPlugin.h"
#include <qdebug.h>

using namespace versioning;

DiffPlugin::DiffPlugin()
{
}

QList<qReal::ActionInfo> DiffPlugin::actions()
{
//	QMenu *diffMenu = new QMenu(tr("Visual diff"));diffMenu->setObjectName("Visual diff");

//	QAction *diffAction = diffMenu->addAction(tr("Diff"));
//	connect(diffAction, SIGNAL(triggered()), this, SLOT(diffClicked()));

//	QAction *diffBetweenAction = diffMenu->addAction(tr("View diff between..."));
//	connect(diffBetweenAction, SIGNAL(triggered()), this, SLOT(diffBetweenClicked()));

//	mMenu << qReal::ActionInfo(diffMenu, "tools");
	return mMenu;
}

void DiffPlugin::diffClicked()
{
//	QWidget *widget = new QWidget();
//	QGridLayout *mLayout = new QGridLayout(widget);
//	mLayout->setMargin(0);
//	widget->setLayout(mLayout);
//	showDiff(mProjectManager->saveFilePath(), widget);
}

void DiffPlugin::diffBetweenClicked()
{
}
