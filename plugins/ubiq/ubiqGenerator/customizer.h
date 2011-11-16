#pragma once

#include "../../../qrgui/toolPluginInterface/customizer.h"

namespace ubiq {
namespace generator {

/// Customizes QReal interface to show Ubiq-specific details.
class Customizer : public qReal::Customizer
{
public:
	/// Window title shall reflect Technology name.
	virtual QString windowTitle() const
	{
		return QObject::tr("QReal:Ubiq");
	}

	/// Connections are extensively used by this plugin, so they are enabled.
	virtual bool showConnectionRelatedMenus() const
	{
		return true;
	}
};

}
}
