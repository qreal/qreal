#pragma once

#include <QtCore/QString>
#include <QtCore/QScopedPointer>


#include "twoDModel/robotModel/twoDRobotModel.h"
#include "twoDModel/engine/twoDModelControlInterface.h"
#include "twoDModel/engine/twoDModelEngineInterface.h"

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class D2ModelWidget;
}

namespace engine {

/// Manages all 2D model engine combining components together and provides API for controling it.
class TWO_D_MODEL_EXPORT TwoDModelEngineFacade : public TwoDModelControlInterface
{
	Q_OBJECT

public:
	/// @param configurer - allows to configure various model parameters specific to a kit. Takes ownership.
	explicit TwoDModelEngineFacade(twoDModel::robotModel::TwoDRobotModel &robotModel);

	~TwoDModelEngineFacade() override;

	void init(const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
			, const qReal::SystemEvents &systemEvents
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, kitBase::InterpreterControlInterface &interpreterControl) override;

	qReal::ActionInfo &showTwoDModelWidgetActionInfo() override;
	kitBase::DevicesConfigurationProvider &devicesConfigurationProvider() override;

	TwoDModelEngineInterface &engine();

public slots:
	void onStartInterpretation() override;
	void onStopInterpretation() override;

private:
	void loadReadOnlyFlags(const qReal::LogicalModelAssistInterface &logicalModel);

	const QString mRobotModelName;
	qReal::ActionInfo mTwoDModelActionInfo;  // Has ownership over contained QAction object.

	QScopedPointer<model::Model> mModel;
	QScopedPointer<view::D2ModelWidget> mView;
	QScopedPointer<TwoDModelEngineInterface> mApi;
};

}
}
