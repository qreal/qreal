#pragma once

#include "kitPluginManager.h"
#include "interpreter/interpreter.h"
#include "robotModelManager.h"

namespace interpreterCore {

/// Responsible for initialization, interconnecting and keeping in sync various plugin subsystems.
class RobotsPluginFacade
{
public:
	RobotsPluginFacade();

private:
	/// Main class for robot interpreter. Contains implementation of generic diagram interpreter.
	interpreter::InterpreterInterface *mInterpreter;  // Has ownership

	KitPluginManager mKitPluginManager;

	RobotModelManager mRobotModelManager;

//	details::SensorsConfigurationManager mSensorsConfigurationManager;

};

}
