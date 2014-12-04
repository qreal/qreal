#pragma once

#include <interpreterBase/kitPluginInterface.h>
#include "ev3RobotModel.h"
#include "ev3AdditionalPreferences.h"

#include "blocks/ev3BlocksFactory.h"
#include "robotModel/real/realRobotModel.h"


namespace ev3KitInterpreter {

class Ev3KitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "Ev3KitInterpreter.Ev3KitInterpreterPlugin")

public:
	Ev3KitInterpreterPlugin();
	~Ev3KitInterpreterPlugin() override;

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			interpreterBase::robotModel::RobotModelInterface const *model) override;

	// Transfers ownership.
	QList<interpreterBase::AdditionalPreferences *> settingsWidgets() override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QIcon iconForFastSelector(interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

private:
	robotModel::real::RealRobotModel mRealRobotModel;

	Ev3AdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	blocks::Ev3BlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership
	bool mOwnsBlocksFactory = true;

};

}
