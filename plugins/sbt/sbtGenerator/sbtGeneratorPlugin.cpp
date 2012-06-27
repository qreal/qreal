#include "sbtGeneratorPlugin.h"

Q_EXPORT_PLUGIN2(sbtGeneratorPlugin, SbtGeneratorPlugin)

using namespace qReal;

SbtGeneratorPlugin::SbtGeneratorPlugin()
{
}

SbtGeneratorPlugin::~SbtGeneratorPlugin()
{
}

void SbtGeneratorPlugin::init(PluginConfigurator const &configurator)
{
    //mGenerator.init(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
}

//qReal::Customizer* SbtGeneratorPlugin::customizationInterface()
//{
//    //return &mCustomizer;
//}

QList<ActionInfo> SbtGeneratorPlugin::actions()
{
    QAction * const generateAction = new QAction(QObject::tr("generate for sbt"), NULL);
    ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
    //QObject::connect(generateAction, SIGNAL(triggered()), &mGenerator, SLOT(generate()));

    return QList<ActionInfo>() << generateActionInfo;
}
