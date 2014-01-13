#pragma once

#include <interpreterBase/kitPluginInterface.h>

namespace nxtKitInterpreter {

class NxtKitInterpreterInterface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NxtKitInterpreterInterface")

public:
	// Override.
	virtual QString kitId() const override;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const override;

	// Override.
	virtual qReal::IdList specificBlocks() const;

	// Override.
	virtual qReal::IdList unsupportedBlocks() const;
};

}
