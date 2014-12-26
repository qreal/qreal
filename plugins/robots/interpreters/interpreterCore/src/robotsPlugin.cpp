#include "robotsPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>
#include <commonTwoDModel/engine/twoDModelControlInterface.h>
#include <utils/abstractTimer.h>

#include "managers/saveConvertionManager.h"

using namespace qReal;
using namespace interpreterCore;

static int const gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
	: mMainWindowInterpretersInterface(nullptr)
{
	mRobotsPluginFacade.reset(new RobotsPluginFacade);
}

void RobotsPlugin::init(PluginConfigurator const &configurator)
{
	mRobotsPluginFacade->init(configurator);
	SettingsManager::setValue("IndexGrid", gridWidth);
}

qReal::Customizer *RobotsPlugin::customizationInterface()
{
	return &mRobotsPluginFacade->customizer();
}

QPair<QString, gui::PreferencesPage *> RobotsPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), mRobotsPluginFacade->robotsSettingsPage());
}

QList<ActionInfo> RobotsPlugin::actions()
{
	return mRobotsPluginFacade->actionsManager().actions();
}

QList<HotKeyActionInfo> RobotsPlugin::hotKeyActions()
{
	return mRobotsPluginFacade->actionsManager().hotKeyActionInfos();
}

QList<ProjectConverter> RobotsPlugin::projectConverters()
{
	return SaveConvertionManager::converters();
}

QStringList RobotsPlugin::defaultSettingsFiles()
{
	return mRobotsPluginFacade->defaultSettingsFiles();
}
