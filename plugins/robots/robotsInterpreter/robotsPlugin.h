#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "interpreter.h"
#include "robotSettingsPage.h"
#include "customizer.h"

namespace qReal {
namespace interpreters {
namespace robots {

class RobotsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RobotsPlugin();
	virtual ~RobotsPlugin();

	virtual void init(PluginConfigurator const &configurator);
	virtual QList<ActionInfo> actions();
	virtual QPair<QString, PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();
	virtual void closeNeededWidget();

private slots:
	void showRobotSettings();
	void show2dModel();

private:
	Customizer mCustomizer;
	Interpreter mInterpreter;

	/// Page with plugin settings. Created here, but then ownership is passed to
	/// a caller of preferencesPage().
	PreferencesRobotSettingsPage *mRobotSettingsPage;  // Does not have ownership

	gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;
	QAction *m2dModelAction;
	QAction *mRunAction;
	QAction *mStopAction;
	QTranslator *mAppTranslator;
};

}
}
}
