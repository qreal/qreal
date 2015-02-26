#pragma once

#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/mainwindow/errorReporter.h"

#include <QtCore/QTranslator>

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrkernel/ids.h>


namespace qReal {
namespace databasesPhysicalModel {

class DatabasesPhysicalModelPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.databasesPhysicalModel.DatabasesPhysicalModelPlugin")
public:
	DatabasesPhysicalModelPlugin();
	~DatabasesPhysicalModelPlugin();
	virtual void init(qReal::PluginConfigurator const &configurator);

	//virtual QPair<QString, PreferencesPage *> preferencesPage();
	//virtual qReal::Customizer* customizationInterface();
	virtual QList<qReal::ActionInfo> actions();
signals:

public slots:
	void generatePhysicalModel();

private:
	//DatabasesPreferencesPage *mPreferencesPage;
	//DatabasesCustomizer mCustomizer;
	//DatabasesGenerator *mDatabasesGenerator;
	const PluginConfigurator* mPluginConfigurator;
	void initActions();

	QAction *mGeneratePhysicalModelAction;
	QMenu *mDatabasesMenu;
	QList<qReal::ActionInfo> mActionInfos;

};

}
}
