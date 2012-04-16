#include "constraintsGeneratorPlugin.h"

Q_EXPORT_PLUGIN2(ConstraintsGeneratorPlugin, constraints::generator::ConstraintsGeneratorPlugin)

using namespace constraints::generator;
using namespace qReal;

ConstraintsGeneratorPlugin::ConstraintsGeneratorPlugin()
{
}

ConstraintsGeneratorPlugin::~ConstraintsGeneratorPlugin()
{
}

void ConstraintsGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mGenerator.init(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
}

QList<ActionInfo> ConstraintsGeneratorPlugin::actions()
{
	QAction * const generateAction = new QAction(QObject::tr("Generate constraints"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), &mGenerator, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}
