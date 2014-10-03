#pragma once

#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace generatorBase {

/// Plugin interface for supporting generators in kit plugins.
class GeneratorKitPluginInterface
{
public:
	virtual ~GeneratorKitPluginInterface() {}

	/// Returns the extension of generated files.
	virtual QString extension() const = 0;

	/// Returns the extension description (filter for the extension).
	/// Uses for update filter field in save/open dialog after kit change.
	virtual QString extensionDescription() const = 0;

	/// Internal id of a generator, must be unique.
	virtual QString generatorName() const = 0;

	/// An identifier of constructor kit. The generator is binded to the kit plugins with the same id.
	virtual QString kitId() const = 0;

	/// @todo: This is a copy-paste from ToolPliginInterface. Think how we can avoid it.

	/// Shall be overriden to return QAction or QMenu objects along with where to put them in
	/// main window. Menus can contains more actions. These actions shall be connected
	/// to slots of a plugin, so when user clicks on an action, some code in plugin gets executed
	virtual QList<qReal::ActionInfo> actions()
	{
		return QList<qReal::ActionInfo>();
	}

	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator Object that contains all needed information for a plugin, like refs to models.
	virtual void init(qReal::PluginConfigurator const &configurator
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	{
		Q_UNUSED(configurator);
		Q_UNUSED(robotModelManager)
	}

	/// Shall be overridden to return QAction instances for their customization in
	/// hot key manager.
	virtual QList<qReal::HotKeyActionInfo> hotKeyActions()
	{
		return QList<qReal::HotKeyActionInfo>();
	}
};

}

Q_DECLARE_INTERFACE(generatorBase::GeneratorKitPluginInterface, "ru.spbsu.math.QReal.GeneratorKitPluginInterface/1")
