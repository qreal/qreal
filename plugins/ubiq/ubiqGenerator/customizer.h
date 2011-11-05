#pragma once

#include "../../../qrgui/toolPluginInterface/customizationInterface.h"

namespace ubiq {
namespace generator {

/// Customizes QReal interface to show Ubiq-specific details.
class Customizer : public qReal::CustomizationInterface
{
public:
	/// Logical model explorer is enabled for this plugin.
	virtual bool showLogicalModelExplorer() const
	{
		return true;
	}

	/// Window title shall reflect Technology.
	virtual QString windowTitle() const
	{
		return QObject::tr("QReal:Ubiq");
	}

	/// No specific icon provided.
	virtual QIcon applicationIcon() const
	{
		return QIcon();
	}
};

}
}
