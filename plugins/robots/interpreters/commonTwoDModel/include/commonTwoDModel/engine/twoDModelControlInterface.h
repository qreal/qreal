#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/mainWindow/mainWindowInterpretersInterface.h>
#include <interpreterBase/devicesConfigurationProvider.h>
#include <interpreterBase/eventsForKitPluginInterface.h>
#include <interpreterBase/interpreterControlInterface.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

/// An interface for controlling 2D model window.
class COMMON_TWO_D_MODEL_EXPORT TwoDModelControlInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TwoDModelControlInterface() {}

	/// Returns an action information for showing 2D model window.
	virtual qReal::ActionInfo &showTwoDModelWidgetActionInfo() = 0;

	/// Returns a reference to the devices configurator.
	/// Can be used by outside enviroment to connect it to other ones.
	virtual interpreterBase::DevicesConfigurationProvider &devicesConfigurationProvider() = 0;

	/// Performs 2D model`s initialization with the given system components.
	/// @todo: Separate twoD model engine from the enviroment (get rid of parameters)
	virtual void init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
			, qReal::SystemEvents const &systemEvents
			, qReal::GraphicalModelAssistInterface &graphicalModel
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface const &interpretersInterface
			, interpreterBase::InterpreterControlInterface &interpreterControl) = 0;

public slots:
	/// Starts interpretation process in 2D model.
	virtual void onStartInterpretation() = 0;

	/// Stops interpretation process in 2D model if started.
	virtual void onStopInterpretation() = 0;

signals:
	/// Emitted each time when user requests interpretation start from 2D model window.
	void runButtonPressed();

	/// Emitted each time when user requests interpretation stop from 2D model window.
	void stopButtonPressed();	
};

}
