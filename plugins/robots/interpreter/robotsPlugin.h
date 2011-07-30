#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"

#include "pluginGui.h"
#include "customizer.h"

namespace qReal {
namespace interpreters {
namespace robots {

class RobotsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RobotsPlugin();
	virtual ~RobotsPlugin();

	virtual void initPlugin(PluginConfigurator const &configurator);
	virtual QList<qReal::CustomToolInterface *> toolInterfaces();
	virtual QList<qReal::InterpreterInterface *> interpreterInterfaces();
	virtual CustomizationInterface* customizationInterface();

private:
	PluginGui mPluginGui;
	Customizer mCustomizer;
};

}
}
}
