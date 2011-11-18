#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "databaseSchemeGenerator/databaseSchemeGenerator.h"


namespace qReal {
namespace ains {

class AinsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	AinsPlugin();
	virtual ~AinsPlugin();

	virtual void init(PluginConfigurator const &configurator);
	virtual QList<ActionInfo> actions();
	virtual QPair<QString, PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();

public slots:
	void generateDatabaseScheme();

private:
	QAction *mGenerateDatabaseSchemeAction;
	DatabaseEditorSchemeGenerator *mDatabaseEditorGenerator;

};

}
}
