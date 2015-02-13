#include "robotsPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>
#include <interpreterCore/managers/saveConvertionManager.h>

using namespace qReal;
using namespace robotsPlugin;

static const int gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
	: mMainWindowInterpretersInterface(nullptr)
{
	mRobotsPluginFacade.reset(new interpreterCore::RobotsPluginFacade);
}

void RobotsPlugin::init(const PluginConfigurator &configurator)
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
	return interpreterCore::SaveConvertionManager::converters();
}

QStringList RobotsPlugin::defaultSettingsFiles()
{
	return mRobotsPluginFacade->defaultSettingsFiles();
}

twoDModel::engine::TwoDModelGuiFacade *RobotsPlugin::guiScriptFacade()
{
	return mRobotsPluginFacade.data()->guiScriptFacade();
}
