#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "nxtOSEK/nxtFlashTool.h"

namespace robots {
namespace generator {

class RobotsGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RobotsGeneratorPlugin();
	virtual ~RobotsGeneratorPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	void generateRobotSourceCode();
	void flashRobot();
	void uploadProgram();

private:
	void checkNxtTools();

	QAction mGenerateCodeAction;
	QAction mFlashRobotAction;
	QAction mUploadProgramAction;
	QTranslator mAppTranslator;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership
	qrRepo::RepoControlInterface *mRepoControlApi;  // Does not have ownership
	bool mNxtToolsPresent;
	NxtFlashTool *mFlashTool;
};

}
}
