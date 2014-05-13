#include "robotsPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace interpreterCore;

static int const gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
	: mMainWindowInterpretersInterface(nullptr)
{
	mInterpreterBaseTranslator.load(":/interpreterBase_" + QLocale::system().name());
	mCommonTwoDModelTranslator.load(":/commonTwoDModel_" + QLocale::system().name());
	mInterpreterCoreTranslator.load(":/interpreterCore_" + QLocale::system().name());
	QApplication::installTranslator(&mInterpreterBaseTranslator);
	QApplication::installTranslator(&mCommonTwoDModelTranslator);
	QApplication::installTranslator(&mInterpreterCoreTranslator);

	// This will start kit plugins loading and so on so we must to load translators first.
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

QList<ActionInfo> RobotsPlugin::actions()
{
	return mRobotsPluginFacade->actionsManager().actions();
}

QList<HotKeyActionInfo> RobotsPlugin::hotKeyActions()
{
	return mRobotsPluginFacade->actionsManager().hotKeyActionInfos();
}

QPair<QString, PreferencesPage *> RobotsPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), mRobotsPluginFacade->robotsSettingsPage());
}
