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
	// WARNING: hack!
	forceLoadLibs();
	mInterpreterBaseTranslator.load(":/interpreterBase_" + QLocale().name());
	mCommonTwoDModelTranslator.load(":/commonTwoDModel_" + QLocale().name());
	mInterpreterCoreTranslator.load(":/interpreterCore_" + QLocale().name());
	mUtilsTranslator.load(":/utils_" + QLocale().name());
	QApplication::installTranslator(&mInterpreterBaseTranslator);
	QApplication::installTranslator(&mCommonTwoDModelTranslator);
	QApplication::installTranslator(&mInterpreterCoreTranslator);
	QApplication::installTranslator(&mUtilsTranslator);

	// This will start kit plugins loading and so on so we must load translators first.
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

QPair<QString, PreferencesPage *> RobotsPlugin::preferencesPage()
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

void RobotsPlugin::forceLoadLibs()
{
	// This will request some symbols from commonTwoDModel lib immediate loading.
	// This hack workarrounds Window`s lazy dependencies loading (else we cannot obtain 2D model translitions).
	QString name = twoDModel::TwoDModelControlInterface::staticMetaObject.className();
	name = utils::AbstractTimer::staticMetaObject.className();
}
