#pragma once

#include <QtCore/QString>
#include <QtCore/QScopedPointer>

#include <interpreterBase/robotModel/robotModelInterface.h>

#include "commonTwoDModel/engine/twoDModelControlInterface.h"
#include "commonTwoDModel/engine/twoDModelEngineInterface.h"
#include "commonTwoDModel/engine/configurer.h"

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

class D2RobotModel;

namespace engine {

class COMMON_TWO_D_MODEL_EXPORT TwoDModelEngineFacade : public TwoDModelControlInterface
{
	Q_OBJECT

public:
	/// @param configurer - allows to configure various model parameters specific to a kit. Takes ownership.
	TwoDModelEngineFacade(interpreterBase::robotModel::RobotModelInterface &robotModel
			, Configurer const * const configurer);

	~TwoDModelEngineFacade();

	void init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
			, qReal::SystemEventsInterface const &systemEvents
			, interpreterBase::InterpreterControlInterface &interpreterControl) override;

	qReal::ActionInfo &showTwoDModelWidgetActionInfo() override;
	interpreterBase::DevicesConfigurationProvider &devicesConfigurationProvider() override;

	TwoDModelEngineInterface &engine();

public slots:
	void onStartInterpretation() override;
	void onStopInterpretation() override;

private:
	QString const mRobotModelName;
	qReal::ActionInfo mTwoDModelActionInfo;  // Has ownership over contained QAction object.
	QScopedPointer<D2RobotModel> mTwoDModel;
};

}
}
