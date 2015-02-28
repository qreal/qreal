#pragma once

#include <QtCore/QScopedPointer>

#include <kitBase/kitPluginInterface.h>
#include <twoDModel/engine/twoDModelControlInterface.h>
#include <nxtKit/blocks/nxtBlocksFactory.h>

#include "nxtAdditionalPreferences.h"
#include "robotModel/real/realRobotModel.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace nxt {

class NxtKitInterpreterPlugin : public QObject, public kitBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxtKitInterpreter.NxtKitInterpreterPlugin")

public:
	NxtKitInterpreterPlugin();
	~NxtKitInterpreterPlugin() override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;

	QString kitId() const override;
	QString friendlyKitName() const override;

	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
	kitBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			const kitBase::robotModel::RobotModelInterface *model) override;

	QList<qReal::ActionInfo> customActions() override;  // Transfers ownership of embedded QActions
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;
	QString defaultSettingsFile() const override;

	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

private slots:
	/// Shows or hides 2d model action depending on whether current tab is robots diagram.
	void onActiveTabChanged(const qReal::Id &rootElementId);

private:
	robotModel::real::RealRobotModel mRealRobotModel;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModel;

	/// @todo Use shared pointers instead of this sh~.
	blocks::NxtBlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership
	bool mOwnsBlocksFactory = true;

	NxtAdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	kitBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
	QString mCurrentlySelectedModelName;
};

}
