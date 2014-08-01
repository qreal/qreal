#pragma once

#include <../qrgui/toolPluginInterface/toolPluginInterface.h>

#include <QtCore/QTranslator>
#include <QtCore/QScopedPointer>

namespace trikRuntimeUploaderPlugin {

class TrikRuntimeUploaderPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "trikRuntimeUploader.TrikRuntimeUploaderPlugin")

public:
	TrikRuntimeUploaderPlugin();
	virtual ~TrikRuntimeUploaderPlugin();

	virtual QList<qReal::ActionInfo> actions();

	virtual void init(qReal::PluginConfigurator const &configurator);

private slots:
	void uploadRuntime();

private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Does not have ownership

	QScopedPointer<QAction> mAction;
	QTranslator mAppTranslator;
};

}
