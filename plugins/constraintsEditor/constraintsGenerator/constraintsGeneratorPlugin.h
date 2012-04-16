#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "generator.h"

namespace constraints {
namespace generator {

class ConstraintsGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	ConstraintsGeneratorPlugin();
	virtual ~ConstraintsGeneratorPlugin();

	/// Initializes generator with ref to logical model.
	virtual void init(qReal::PluginConfigurator const &configurator);

	/// Returns list containing "generate" action, which runs a generator.
	virtual QList<qReal::ActionInfo> actions();

private:
	Generator mGenerator;
};

}
}
