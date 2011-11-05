#include "ubiqGeneratorPlugin.h"

Q_EXPORT_PLUGIN2(ubiqGeneratorPlugin, ubiq::generator::UbiqGeneratorPlugin)

using namespace ubiq::generator;
using namespace qReal;

UbiqGeneratorPlugin::UbiqGeneratorPlugin()
{
}

UbiqGeneratorPlugin::~UbiqGeneratorPlugin()
{
}

void UbiqGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mGenerator.init(configurator.logicalModelApi());
}

qReal::CustomizationInterface* UbiqGeneratorPlugin::customizationInterface()
{
	return &mCustomizer;
}

QList<ActionInfo> UbiqGeneratorPlugin::actions()
{
	QAction *generateAction = new QAction(QObject::tr("generate"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}
