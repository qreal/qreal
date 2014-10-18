#pragma once

#include <QtCore/QString>
#include <QtCore/QScopedPointer>


#include "commonTwoDModel/robotModel/twoDRobotModel.h"
#include "commonTwoDModel/engine/twoDModelControlInterface.h"
#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

#include "commonTwoDModel/robotModel/twoDRobotModel.h"

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class D2ModelWidget;
}

namespace engine {

/// Manages all 2D model engine combining components together and provides API for controling it.
class COMMON_TWO_D_MODEL_EXPORT TwoDModelEngineFacade : public TwoDModelControlInterface
{
	Q_OBJECT

public:
	/// @param configurer - allows to configure various model parameters specific to a kit. Takes ownership.
	TwoDModelEngineFacade(twoDModel::robotModel::TwoDRobotModel &robotModel);

	~TwoDModelEngineFacade();

	void init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
			, qReal::SystemEventsInterface const &systemEvents
			, qReal::GraphicalModelAssistInterface &graphicalModel
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface const &interpretersInterface
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

	QScopedPointer<model::Model> mModel;
	QScopedPointer<view::D2ModelWidget> mView;
	QScopedPointer<TwoDModelEngineInterface> mApi;
};

}
}
