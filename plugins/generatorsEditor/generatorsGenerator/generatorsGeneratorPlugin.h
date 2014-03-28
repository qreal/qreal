#pragma once
#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace generators {
namespace generator {

class GeneratorsGeneratorPlugin : public QObject , public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "generators.generator.GeneratorsGeneratorPlugin")

public:
	GeneratorsGeneratorPlugin();
	virtual ~GeneratorsGeneratorPlugin();

	/// Initializes generator with ref to logical model.
	virtual void init(qReal::PluginConfigurator const &configurator);

	/// Returns list containing "generate" action, which runs a generator.
	virtual QList<qReal::ActionInfo> actions();
};

}
}
