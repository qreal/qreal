#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <kitBase/devicesConfigurationProvider.h>
#include <kitBase/eventsForKitPluginInterface.h>
#include <kitBase/interpreterControlInterface.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

/// An interface for controlling 2D model window.
class TWO_D_MODEL_EXPORT TwoDModelControlInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TwoDModelControlInterface() {}

	/// Returns an action information for showing 2D model window.
	virtual qReal::ActionInfo &showTwoDModelWidgetActionInfo() = 0;

	/// Returns a reference to the devices configurator.
	/// Can be used by outside enviroment to connect it to other ones.
	virtual kitBase::DevicesConfigurationProvider &devicesConfigurationProvider() = 0;

	/// Performs 2D model`s initialization with the given system components.
	/// @todo: Separate twoD model engine from the enviroment (get rid of parameters)
	virtual void init(const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
			, const qReal::SystemEvents &systemEvents
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, kitBase::InterpreterControlInterface &interpreterControl) = 0;

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
