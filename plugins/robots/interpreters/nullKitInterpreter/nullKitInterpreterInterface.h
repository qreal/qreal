#pragma once

#include <QtCore/QObject>

#include <interpreterBase/kitPluginInterface.h>

namespace nullKitInterpreter {

class NullKitInterpreterInterface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NullKitInterpreterInterface")

public:
	// Override.
	virtual QString kitId() const;

	// Override.
	virtual QString friendlyKitName() const override;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const;

	// Override.
	// Transfers ownership.
//	virtual QList<QWidget *> toolWidgets() const;

	// Override.
	virtual qReal::IdList specificBlocks() const;

	// Override.
	virtual qReal::IdList unsupportedBlocks() const;
};

}
