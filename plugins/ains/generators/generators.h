#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace ains {

class AinsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	AinsPlugin();
	virtual ~AinsPlugin();

	virtual void init(PluginConfigurator const &configurator);
	virtual QList<ActionInfo> actions();
	virtual QPair<QString, PreferencesPage *> preferencesPage();
	virtual CustomizationInterface* customizationInterface();
	virtual void updateSettings();
};

}
