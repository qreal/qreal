#include "databasesSupportPlugin.h"

namespace qReal {
namespace databasesSupport {

DatabasesSupportPlugin::DatabasesSupportPlugin()
	: mPreferencesPage(new DatabasesPreferencesPage())
{
}

DatabasesSupportPlugin::~DatabasesSupportPlugin()
{
	delete mPreferencesPage;
}


void DatabasesSupportPlugin::init(PluginConfigurator const &configurator)
{
	//mVisualDebugSupport = new VisualDebugSupport(configurator);
	initActions();
}

QPair<QString, PreferencesPage *> DatabasesSupportPlugin::preferencesPage()
{
	return qMakePair(tr("Databases"), static_cast<PreferencesPage*>(mPreferencesPage));
}

qReal::Customizer* DatabasesSupportPlugin::customizationInterface()
{
	return &mCustomizer;
}

void DatabasesSupportPlugin::initActions()
{

}
}
}
