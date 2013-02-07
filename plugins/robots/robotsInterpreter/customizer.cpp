#include "customizer.h"
#include "../../../qrkernel/settingsManager.h"

using namespace qReal::interpreters::robots;

QString Customizer::windowTitle() const
{
	return QObject::tr("QReal:Robots");
}

QIcon Customizer::applicationIcon() const
{
	return QIcon(":/icons/kroki.png");
}

void Customizer::customizeDocks(gui::MainWindowDockInterface *dockInterface)
{
	mDockInterface = dockInterface;
	dockInterface->logicalModelDock()->hide();
	dockInterface->tabifyDockWidget(dockInterface->graphicalModelDock(), dockInterface->propertyEditorDock());
}

void Customizer::placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget)
{
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, watchWindow);
	watchWindow->setFloating(false);
	QDockWidget *sensorsDock = produceDockWidget(QObject::tr("Configure sensors"), sensorsWidget);
	mDockInterface->addDockWidget(Qt::LeftDockWidgetArea, sensorsDock);
}

QDockWidget *Customizer::produceDockWidget(QString const &title, QWidget *content) const
{
	QDockWidget *dock = new QDockWidget(title);
	dock->setWidget(content);
	return dock;
}

void Customizer::customizeScene(SceneCustomizationInterface *sceneCustomizer)
{
	mSceneCustomizer = sceneCustomizer;
	resetTitlesVisibility();
}

void Customizer::rereadSettings()
{
	resetTitlesVisibility();
}

void Customizer::resetTitlesVisibility()
{
	bool const titlesVisible = qReal::SettingsManager::value("showTitlesForRobots").toBool();
	mSceneCustomizer->setTitlesVisible(titlesVisible);
}
