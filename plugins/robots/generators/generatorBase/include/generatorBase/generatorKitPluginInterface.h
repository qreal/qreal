#pragma once

#include <qrgui/toolPluginInterface/actionInfo.h>
#include <qrgui/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/toolPluginInterface/pluginConfigurator.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace generatorBase {

/// Plugin interface for supporting generators in kit plugins.
class GeneratorKitPluginInterface
{
public:
	virtual ~GeneratorKitPluginInterface() {}

	virtual QString extension() const = 0;
	virtual QString extDescrition() const = 0;
	virtual QString generatorName() const = 0;

	/// An identifier of constructor kit. The generator is binded to the kit plugins with the same id.
	virtual QString kitId() const = 0;

	/// @todo: This is a copy-paste from ToolPliginInterface. Think how we can avoid it.

	/// Shall be overriden to return QAction or QMenu objects along with where to put them in
	/// main window. Menus can contains more actions. These actions shall be connected
	/// to slots of a plugin, so when user clicks on an action, some code in plugin gets executed
	virtual QList<ActionInfo> actions()
	{
		return QList<ActionInfo>();
	}

	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator Object that contains all needed information for a plugin, like refs to models.
	virtual void init(PluginConfigurator const &configurator
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	{
		Q_UNUSED(configurator);
		Q_UNUSED(robotModelManager)
	}

	/// Shall be overridden to return QAction instances for their customization in
	/// hot key manager.
	virtual QList<HotKeyActionInfo> hotKeyActions()
	{
		return QList<HotKeyActionInfo>();
	}
};

}

Q_DECLARE_INTERFACE(generatorBase::GeneratorKitPluginInterface, "ru.spbsu.math.QReal.GeneratorKitPluginInterface/1")
