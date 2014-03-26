#pragma once

#include <QtCore/QObject>

#include "robotModelInterface.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

/// Provides reference to currently selected robot model and notification when it changed. All references are guaranteed
/// to be valid through all lifetime of a plugin, but particular model or devices can be in uninitialized state.
class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelManagerInterface : public QObject
{
	Q_OBJECT

public:
	/// Destructor.
	virtual ~RobotModelManagerInterface() {}

	/// Returns current selected robot model
	virtual RobotModelInterface &model() const = 0;

signals:
	/// Emitted every time when user selected other robot model
	void robotModelChanged(RobotModelInterface &model);

	/// Signal from underlying model, emitted when QReal physically connects to robot. If there is no need for
	/// connection, it is emitted immediately. Signal correctly reconnects to a model when it is changed, so client
	/// code shall connect to this signal instead of model.
	/// @param success - true, if connected successfully.
	void connected(bool success);

	/// Signal from underlying model, emitted when robot is disconnected. Signal correctly reconnects to a model
	/// when it is changed, so client code shall connect to this signal instead of model.
	void disconnected();

	/// Signal from underlying model, emitted when model is finished configuring devices. Signal correctly reconnects
	/// to a model when it is changed, so client code shall connect to this signal instead of model.
	void allDevicesConfigured();
};

}
}
