#include <QtGui/QApplication>

#include "sbtGeneratorPlugin.h"

using namespace sbt;

Q_EXPORT_PLUGIN2(sbtGeneratorPlugin, SbtGeneratorPlugin)

using namespace qReal;

SbtGeneratorPlugin::SbtGeneratorPlugin()
{
	mAppTranslator.load(":/sbtGeneratorPlugin_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

SbtGeneratorPlugin::~SbtGeneratorPlugin()
{
}

void SbtGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mGenerator.init(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
}

QList<ActionInfo> SbtGeneratorPlugin::actions()
{
	QAction * const generateAction = new QAction(QObject::tr("generate for sbt"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), &mGenerator, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}
