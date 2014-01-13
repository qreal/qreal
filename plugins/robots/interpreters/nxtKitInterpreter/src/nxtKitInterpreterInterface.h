#pragma once

#include <interpreterBase/kitPluginInterface.h>
#include <interpreterBase/robotModel/proxyRobotModel.h>

namespace nxtKitInterpreter {

class NxtKitInterpreterInterface : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxtKitInterpreter.NxtKitInterpreterInterface")

public:
	// Override.
	virtual QString kitId() const override;

	// Override.
	virtual QString friendlyKitName() const override;

	// Override
	virtual interpreterBase::robotModel::RobotModelInterface &robotModel() override;

	// Override.
	// Transfers ownership.
	virtual QWidget *settingsWidget() const override;

	// Override.
	virtual qReal::IdList specificBlocks() const;

	// Override.
	virtual qReal::IdList unsupportedBlocks() const;

private:
	interpreterBase::robotModel::ProxyRobotModel mRobotModel;
};

}
