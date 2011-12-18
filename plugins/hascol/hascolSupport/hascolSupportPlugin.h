#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

namespace hascol {
namespace support {

class HascolSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	HascolSupportPlugin();
	virtual ~HascolSupportPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	void generateHascolSourceCode();
	void parseHascolSources();

private:
	QAction mGenerateCodeAction;
	QAction mParseSourcesAction;
	QTranslator mAppTranslator;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership
};

}
}
