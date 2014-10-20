#pragma once

#include <QtCore/QScopedPointer>

#include <interpreterBase/kitPluginInterface.h>
#include <commonTwoDModel/engine/twoDModelControlInterface.h>

#include "trikAdditionalPreferences.h"
#include "blocks/trikBlocksFactory.h"
#include "robotModel/real/realRobotModelV6.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace trikKitInterpreter {

class TrikKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxtKitInterpreter.TrikKitInterpreterPlugin")

public:
	TrikKitInterpreterPlugin();
	~TrikKitInterpreterPlugin() override;

	void init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
			, qReal::SystemEventsInterface const &systemEvents
			, qReal::GraphicalModelAssistInterface &graphicalModel
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface const &interpretersInterface
			, interpreterBase::InterpreterControlInterface &interpreterControl) override;

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			interpreterBase::robotModel::RobotModelInterface const *model) override;

	interpreterBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

	QWidget *quickPreferencesFor(interpreterBase::robotModel::RobotModelInterface const &model) override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QString defaultSettingsFile() const override;

	QIcon iconForFastSelector(interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

	interpreterBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

private slots:
	/// Shows or hides 2d model action depending on whether current tab is robots diagram.
	void onActiveTabChanged(qReal::Id const &rootElementId);

private:
	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModelV6;
	robotModel::real::RealRobotModelV6 mRealRobotModelV6;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModelV6;

	/// @todo Use shared pointers instead of this sh~.
	blocks::TrikBlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership
	bool mOwnsBlocksFactory = true;

	TrikAdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
	bool mOwnsAdditionalPreferences = true;

	QWidget *mIpAdressQuickConfigurer = nullptr;  // Transfers ownership
	bool mOwnsIpAdressQuickConfigurer = true;

	interpreterBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
	QString mCurrentlySelectedModelName;
};

}
