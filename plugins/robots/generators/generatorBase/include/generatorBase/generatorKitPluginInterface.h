#pragma once

#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/textEditor/languageInfo.h>
#include <qrtext/languageToolboxInterface.h>
#include <interpreterBase/robotModel/robotModelManagerInterface.h>

namespace generatorBase {

/// Plugin interface for supporting generators in kit plugins.
class GeneratorKitPluginInterface
{
public:
	virtual ~GeneratorKitPluginInterface() {}

	/// Returns an information about the language code on which will be generated;
	/// this information will be used by text editors when user will be edit the generated code.
	/// Transfers ownership.
//	virtual qReal::text::LanguageInfo *targetLanguage() const = 0;

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
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage)
	{
		Q_UNUSED(configurator);
		Q_UNUSED(robotModelManager)
		Q_UNUSED(textLanguage)
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
