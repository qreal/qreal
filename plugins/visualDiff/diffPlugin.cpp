#include "diffPlugin.h"
#include <qdebug.h>

using namespace versioning;

DiffPlugin::DiffPlugin()
{
	qDebug() << "diffpl is ready";
}

QList<qReal::ActionInfo> DiffPlugin::actions()
{
	QMenu *diffMenu = new QMenu(tr("Visual diff"));

	QAction *diffAction = diffMenu->addAction(tr("Diff"));
	connect(diffAction, SIGNAL(triggered()), this, SLOT(diffClicked()));

	QAction *diffBetweenAction = diffMenu->addAction(tr("View diff between..."));
	connect(diffBetweenAction, SIGNAL(triggered()), this, SLOT(diffBetweenClicked()));

	QList<qReal::ActionInfo> result;
	result << qReal::ActionInfo(diffMenu, "tools");
	return result;
}

void DiffPlugin::diffClicked()
{
	showDiff(mProjectManager->saveFilePath());
}

void DiffPlugin::diffBetweenClicked()
{
}
