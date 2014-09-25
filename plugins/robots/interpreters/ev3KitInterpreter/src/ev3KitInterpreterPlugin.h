#pragma once

#include <interpreterBase/kitPluginInterface.h>
#include "ev3RobotModel.h"

namespace ev3KitInterpreter {

class NullKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nullKitInterpreter.NullKitInterpreterPlugin")

public:
	NullKitInterpreterPlugin();

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			interpreterBase::robotModel::RobotModelInterface const *model) override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QIcon iconForFastSelector(interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

private:
	Ev3RobotModel mRobotModel;
};

}
