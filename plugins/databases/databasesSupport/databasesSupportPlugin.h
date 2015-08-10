#pragma once

#include "../../../qrgui/plugins/toolPluginInterface/pluginConfigurator.h"
#include "../../../qrgui/plugins/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/mainwindow/errorReporter.h"

#include <QtCore/QTranslator>
#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrkernel/ids.h>
#include "../qrgui/preferencesDialog/preferencesPages/behaviourPage.h"

#include "databasesPreferencesPage.h"
#include "databasesCustomizer.h"
#include "generator/databasesGenerator.h"

namespace qReal {
namespace databasesSupport {

/// Main class for QReal:databases
/// Provides cooperation between main window
/// and functionality
class DatabasesSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.databasesSupport.DatabasesSupportPlugin")
public:
	DatabasesSupportPlugin();
	~DatabasesSupportPlugin();
	virtual void init(qReal::PluginConfigurator const &configurator);

	virtual QPair<QString, gui::PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	virtual QList<qReal::ActionInfo> actions();

public slots:
	/// Checks simple rules of correctnes (redirection to generator)
	void checkCorrectness();
	/// Generates physical model from logical model (redirection to generator)
	void generateCode();
	/// Checks SQL-code from physical model(redirection to generator)
	void generatePhysicalModel();

private:
	DatabasesPreferencesPage *mPreferencesPage;
	DatabasesCustomizer mCustomizer;
	DatabasesGenerator *mDatabasesGenerator;
	void initActions();

	QAction *mGenerateCodeAction;
	QAction *mCheckCorectnessAction;
	QAction *mGeneratePhysicalModelAction;
	QMenu *mDatabasesMenu;
	QList<qReal::ActionInfo> mActionInfos;

};

}
}
