#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

#include "filterObject.h"

class QToolBar;
class QAction;

namespace uxInfo {

/// Logs user actions and collects statistics for future analisys.
class UXInfoPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "uxInfo.UXInfoPlugin")

public:
	UXInfoPlugin();

	void init(qReal::PluginConfigurator const &configurator) override;
	QStringList defaultSettingsFiles() override;

private:
	void initSettingsUi(qReal::gui::PreferencesPage &behaviourPage);

	void startUsabilityTest();
	void finishUsabilityTest();

	void processEvent(QObject *obj, QEvent *e);

	QToolBar *mUsabilityTestingToolbar;  // Has ownership
	QAction *mStartTest;  // Has ownership via Qt parentship system
	QAction *mFinishTest;  // Has ownership via Qt parentship system
	bool mUsabilityTestingMode;
	FilterObject mFilterObject;
	int mStartTimestamp;
};

}
