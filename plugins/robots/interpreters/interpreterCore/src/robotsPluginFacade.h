#pragma once

#include <QtCore/QObject>

#include <qrgui/toolPluginInterface/pluginConfigurator.h>

#include "kitPluginManager.h"
#include "interpreter/interpreter.h"
#include "robotModelManager.h"
#include "robotsSettingsPage.h"

namespace interpreterCore {

/// Responsible for initialization, interconnecting and keeping in sync core plugin subsystems.
/// \todo Document this.
class RobotsPluginFacade : public QObject
{
	Q_OBJECT

public:
	RobotsPluginFacade();

	~RobotsPluginFacade() override;

	/// \todo Remove connectToRobotAction, use signals instead.
	void init(qReal::PluginConfigurator const &configurer, QAction &connectToRobotAction);

	interpreter::InterpreterInterface &interpreter();

	PreferencesPage *robotsSettingsPage() const;

	KitPluginManager &kitPluginManager();

private:
	/// Main class for robot interpreter. Contains implementation of generic diagram interpreter.
	interpreter::InterpreterInterface *mInterpreter;  // Has ownership

	/// Page with plugin settings. Created here, but then ownership is passed to
	/// a caller of preferencesPage().
	RobotsSettingsPage *mRobotSettingsPage;  // Does not have ownership

	KitPluginManager mKitPluginManager;

	RobotModelManager mRobotModelManager;

//	details::SensorsConfigurationManager mSensorsConfigurationManager;

};

}
