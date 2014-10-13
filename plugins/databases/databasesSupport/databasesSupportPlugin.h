#pragma once

#include <QtCore/QTranslator>
#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "databasesPreferencesPage.h"
#include "databasesCustomizer.h"

namespace qReal {
namespace databasesSupport {

class DatabasesSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.databasesSupport.DatabasesSupportPlugin")
public:
	DatabasesSupportPlugin();
	~DatabasesSupportPlugin();
	virtual void init(qReal::PluginConfigurator const &configurator);

	virtual QPair<QString, PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	//virtual QList<qReal::ActionInfo> actions();
signals:

public slots:

private:
	DatabasesPreferencesPage *mPreferencesPage;
	DatabasesCustomizer mCustomizer;
	void initActions();

};

}
}
