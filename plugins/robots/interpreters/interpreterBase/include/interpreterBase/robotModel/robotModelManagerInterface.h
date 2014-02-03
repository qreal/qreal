#pragma once

#include <QtCore/QObject>

#include "interpreterBase/robotModel/robotModelInterface.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

/// Provides reference to currently selected robot model and notification when it changed. All references are guaranteed
/// to be valid through all lifetime of a plugin, but particular model or devices can be in uninitialized state.
class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelManagerInterface : public QObject
{
	Q_OBJECT

public:
	/// Returns current selected robot model
	virtual interpreterBase::robotModel::RobotModelInterface &model() const = 0;

signals:
	/// Emitted every time when user selected other robot model
	void robotModelChanged();
};

}
}
