#include "customizer.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

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
	return "2.5.1";
}

QString Customizer::aboutText() const
{
	return "<b>" + windowTitle() + "<b><br><br><a href=\"http://robots.qreal.ru/\">http://robots.qreal.ru/</a>";
}

QString Customizer::examplesDirectory() const
{
	return QApplication::applicationDirPath() + "/examples";
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

void Customizer::placeSensorsConfig(QWidget *sensorsWidget)
{
	QDockWidget *sensorsDock = produceDockWidget(QObject::tr("Configure sensors"), sensorsWidget);
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, sensorsDock);
}

void Customizer::placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch)
{
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, watchWindow);
	watchWindow->setFloating(false);
	QDockWidget *graphWatchDock = produceDockWidget(QObject::tr("Sensors state"), graphicsWatch);
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, graphWatchDock);

	mDockInterface->tabifyDockWidget(watchWindow, graphWatchDock);
}

QDockWidget *Customizer::produceDockWidget(QString const &title, QWidget *content) const
{
	QDockWidget *dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
}

QString Customizer::userPaletteTitle() const
{
	return QObject::tr("Subprograms");
}

QString Customizer::userPaletteDescription() const
{
	return QObject::tr("The list of all declared subprograms in the project");
}
