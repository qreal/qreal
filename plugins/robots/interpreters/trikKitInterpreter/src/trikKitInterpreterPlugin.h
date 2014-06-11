#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QTranslator>

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

	void init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
			, qReal::SystemEventsInterface const &systemEvents
			, qReal::GraphicalModelAssistInterface &graphicalModel
			, qReal::LogicalModelAssistInterface &logicalModel
			, interpreterBase::InterpreterControlInterface &interpreterControl) override;

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			interpreterBase::robotModel::RobotModelInterface const *model) override;

	interpreterBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

	QList<qReal::ActionInfo> customActions() override;

	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	QIcon iconForFastSelector(interpreterBase::robotModel::RobotModelInterface const &robotModel) const override;

	interpreterBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

private slots:
	/// Shows or hides 2d model action depending on whether current tab is robots diagram.
	void onActiveTabChanged(qReal::Id const &rootElementId);

private:
	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModelV6;
	robotModel::real::RealRobotModelV6 mRealRobotModelV6;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModelV6;
	blocks::TrikBlocksFactory *mBlocksFactory;  // Transfers ownership
	TrikAdditionalPreferences *mAdditionalPreferences;  // Transfers ownership
	interpreterBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
	QString mCurrentlySelectedModelName;
	QTranslator mAppTranslator;
};

}
