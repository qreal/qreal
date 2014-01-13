#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtWidgets/QWidget>

#include <interpreterBase/blocks/block.h>

namespace interpreterBase {

/// Plugin interface for a kit support plugin.
class KitPluginInterface
{
public:
	virtual ~KitPluginInterface() {}

	/// Unique identifier of constructor kit.
	virtual QString kitId() const = 0;

	/// String that will be displayed to users as the name of the kit.
	virtual QString friendlyKitName() const = 0;

	// Transfers ownership.
	/// Widget with specific settings for a plugin.
	// TODO: It actually is PreferencesPage, because it needs to save and load settings.
	virtual QWidget *settingsWidget() const = 0;

	// Transfers ownership.
	/// List of tool widgets specific to this plugin which will be added as dock windows to an interface.
//	virtual QList<QWidget *> toolWidgets() const = 0;

	/// A list of additional blocks whose implementations are provided by this plugin or shall override defaults.
//	virtual qReal::IdList specificBlocks() const = 0;

	/// A list of common blocks which shall be disabled by this plugin.
	virtual qReal::IdList unsupportedBlocks() const = 0;
};

}

Q_DECLARE_INTERFACE(interpreterBase::KitPluginInterface, "ru.spbsu.math.QReal.KitPluginInterface/1")
