#pragma once

#include <QtCore/QObject>

#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace testInvocation
{

/// This plugin shows how to use invocation framework
class TestInvocationPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	virtual QList<qReal::ActionInfo> actions();
	virtual void init(qReal::PluginConfigurator const &configurator);

	void doOperation(invocation::Progress *progress, int sleepInterval, int count);
	void doOperation(int sleepInterval, int count);

private slots:
	void asyncWithoutProgress();
	void asyncWithProgress();
	void syncWithoutProgress();
	void syncWithProgress();

	void onOpertionFinished(invocation::InvocationState result);
private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindow;

};

}
