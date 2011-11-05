#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "customizer.h"
#include "generator.h"

namespace ubiq {
namespace generator {

/** Entry point for generator plugin, through this class QReal GUI loads and
	runs a plugin. It provides a list of actions that user can do with a plugin
	(basically there is only "generate" action), and GUI customization object.
*/
class UbiqGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	UbiqGeneratorPlugin();
	virtual ~UbiqGeneratorPlugin();

	/// Initializes generator with ref to logical model.
	virtual void init(qReal::PluginConfigurator const &configurator);

	/// Returns list containing "generate" action, which runs a generator.
	virtual QList<qReal::ActionInfo> actions();

	/// Provides customization object that changes main window title and so on.
	virtual qReal::Customizer* customizationInterface();

private:
	/// Customization object for configuring QReal main window.
	Customizer mCustomizer;

	/// Generator main object, directs generation.
	Generator mGenerator;
};

}
}

