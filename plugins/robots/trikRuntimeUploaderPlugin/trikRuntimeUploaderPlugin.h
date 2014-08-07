#pragma once

#include <QtCore/QTranslator>
#include <QtCore/QScopedPointer>

#include <qrgui/toolPluginInterface/toolPluginInterface.h>

namespace trikRuntimeUploaderPlugin {

class TrikRuntimeUploaderPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "trikRuntimeUploader.TrikRuntimeUploaderPlugin")

public:
	TrikRuntimeUploaderPlugin();
	~TrikRuntimeUploaderPlugin() override;

	virtual QList<qReal::ActionInfo> actions() override;

	virtual void init(qReal::PluginConfigurator const &configurator);

private slots:
	void uploadRuntime();

private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Does not have ownership

	QScopedPointer<QAction> mAction;
	QTranslator mAppTranslator;
};

}
