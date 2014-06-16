#include "robotsPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>
#include <commonTwoDModel/engine/twoDModelControlInterface.h>

#include "managers/saveConvertionManager.h"

using namespace qReal;
using namespace interpreterCore;

static int const gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
	: mMainWindowInterpretersInterface(nullptr)
{
	// WARNING: hack!
	forceLoadCommonTwoDModelLib();
	mInterpreterBaseTranslator.load(":/interpreterBase_" + QLocale().name());
	mCommonTwoDModelTranslator.load(":/commonTwoDModel_" + QLocale().name());
	mInterpreterCoreTranslator.load(":/interpreterCore_" + QLocale().name());
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

void RobotsPlugin::forceLoadCommonTwoDModelLib()
{
	// This will request some symbols from commonTwoDModel lib immediate loading.
	// This hack workarrounds Window`s lazy dependencies loading (else we cannot obtain 2D model translitions).
	QString const name = twoDModel::TwoDModelControlInterface::staticMetaObject.className();
	Q_UNUSED(name)
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

QPair<QString, PreferencesPage *> RobotsPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), mRobotsPluginFacade->robotsSettingsPage());
}
