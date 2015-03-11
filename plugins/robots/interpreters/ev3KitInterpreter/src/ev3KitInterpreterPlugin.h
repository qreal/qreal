#pragma once

#include <kitBase/kitPluginInterface.h>
#include <ev3Kit/blocks/ev3BlocksFactory.h>

#include "ev3AdditionalPreferences.h"
#include "robotModel/real/realRobotModel.h"


namespace ev3 {

class Ev3KitInterpreterPlugin : public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "Ev3KitInterpreter.Ev3KitInterpreterPlugin")

public:
	Ev3KitInterpreterPlugin();

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	// Transfers ownership.
	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

private:
	robotModel::real::RealRobotModel mRealRobotModel;
	Ev3AdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
	blocks::Ev3BlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership

};

}
