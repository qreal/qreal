#pragma once

#include "plugins/toolPluginInterface/pluginInterface.h"

namespace qReal {

/// Base class for interpreted plugins.
class InterpretedPluginInterface : public PluginInterface
{
public:
	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator - Object that contains all needed information for a plugin, like refs to models.
	/// @param metamodelRepoApi - Object that contains information about metamodel
	virtual void init(
			const PluginConfigurator &configurator
			, qrRepo::LogicalRepoApi &metamodelRepoApi
			) = 0;
};

}

Q_DECLARE_INTERFACE(qReal::InterpretedPluginInterface, "ru.tepkom.QReal.InterpretedPluginInterface/0.2")
