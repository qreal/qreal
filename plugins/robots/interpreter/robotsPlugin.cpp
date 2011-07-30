#include "robotsPlugin.h"

#include <QtCore/QTranslator>
#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(robotsPlugin, qReal::interpreters::robots::RobotsPlugin)

using namespace qReal::interpreters::robots;

RobotsPlugin::RobotsPlugin()
{
	QTranslator *appTranslator = new QTranslator();
	appTranslator->load(":/robotsInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(appTranslator);
}

RobotsPlugin::~RobotsPlugin()
{
}

void RobotsPlugin::initPlugin(PluginConfigurator const &configurator)
{
	mPluginGui.init(configurator);
}

QList<qReal::CustomToolInterface *> RobotsPlugin::toolInterfaces()
{
	return QList<qReal::CustomToolInterface *>() << &mPluginGui;
}

QList<qReal::InterpreterInterface *> RobotsPlugin::interpreterInterfaces()
{
	return QList<qReal::InterpreterInterface *>();
}

qReal::CustomizationInterface* RobotsPlugin::customizationInterface()
{
	return &mCustomizer;
}
