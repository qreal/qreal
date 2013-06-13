#include "customizer.h"
#include "../../../qrkernel/settingsManager.h"

using namespace qReal::interpreters::robots;

QString Customizer::windowTitle() const
{
	return QObject::tr("QReal:Robots") + " " + productVersion();
}

QIcon Customizer::applicationIcon() const
{
	return QIcon(":/icons/kroki.png");
}

QString Customizer::productVersion() const
{
	// TODO: other storage for it?
	return "2.3.2";
}

QString Customizer::aboutText() const
{
	return "<b>" + windowTitle() + "<b><br><br><a href=\"http://qreal.ru/\">http://qreal.ru/</a>";
}

bool Customizer::showInterpeterButton() const
{
	return false;
}

void Customizer::customizeDocks(gui::MainWindowDockInterface *dockInterface)
{
	mDockInterface = dockInterface;
	dockInterface->logicalModelDock()->hide();
	dockInterface->tabifyDockWidget(dockInterface->graphicalModelDock(), dockInterface->propertyEditorDock());
	dockInterface->graphicalModelDock()->setWindowTitle(tr("Blocks"));
}

void Customizer::placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget, QWidget *nxtDisplay)
{
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, watchWindow);
	watchWindow->setFloating(false);
	QDockWidget *sensorsDock = produceDockWidget(QObject::tr("Configure sensors"), sensorsWidget);
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, sensorsDock);
	QDockWidget *nxtDisplayDock = produceDockWidget(QObject::tr("Nxt display"), nxtDisplay);
	mDockInterface->tabifyDockWidget(mDockInterface->paletteDock(), nxtDisplayDock);
}

QDockWidget *Customizer::produceDockWidget(QString const &title, QWidget *content) const
{
	QDockWidget *dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
}
