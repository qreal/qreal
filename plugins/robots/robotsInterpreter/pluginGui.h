#pragma once

#include <QtCore/QObject>

#include "../../../qrgui/toolPluginInterface/customToolInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "interpreter.h"
#include "robotSettingsPage.h"

namespace qReal {
namespace interpreters {
namespace robots {

class PluginGui: public QObject, public CustomToolInterface
{
	Q_OBJECT

public:
	PluginGui();
	virtual QList<ActionInfo> actions();
	virtual void init(PluginConfigurator const &configurator);
	virtual QPair<QString, PreferencesPage *> preferencesPage();
	void updateSettings();

private slots:
	void showRobotSettings();
	void show2dModel();

private:
	Interpreter mInterpreter;
	PreferencesRobotSettingsPage mRobotSettinsPage;
	gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;
	QAction *m2dModelAction;
	QAction *mRunAction;
	QAction *mStopAction;
};

}
}
}
