#include "databasesPhysicalModelPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>

namespace qReal {
namespace databasesPhysicalModel {

DatabasesPhysicalModelPlugin::DatabasesPhysicalModelPlugin()
	//: mPreferencesPage(new DatabasesPreferencesPage())
	//, mGenerateCodeAction(NULL)
	//, mCheckCorectnessAction(NULL)
{
}

DatabasesPhysicalModelPlugin::~DatabasesPhysicalModelPlugin()
{
//	delete mPreferencesPage;
}


void DatabasesPhysicalModelPlugin::init(PluginConfigurator const &configurator)
{
//	mDatabasesGenerator = new DatabasesGenerator(configurator);
	initActions();
}

/*QPair<QString, PreferencesPage *> DatabasesPhysicalModelPlugin::preferencesPage()
{
	//return qMakePair(tr("Databases"), static_cast<PreferencesPage*>(mPreferencesPage));
}

qReal::Customizer* DatabasesPhysicalModelPlugin::customizationInterface()
{
	//return &mCustomizer;
}*/

QList<qReal::ActionInfo> DatabasesPhysicalModelPlugin::actions()
{
	return mActionInfos;
}

void DatabasesPhysicalModelPlugin::initActions()
{
	mGeneratePhysicalModelAction = new QAction(tr("Generate physical model"), NULL);
	//ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGeneratePhysicalModelAction, SIGNAL(triggered()), this, SLOT(generatePhysicalModel()));

	mDatabasesMenu = new QMenu(tr("Physical model.."));
	mDatabasesMenu->addAction(mGeneratePhysicalModelAction);
	ActionInfo databasesMenuInfo(mDatabasesMenu, "tools");

	mActionInfos << databasesMenuInfo;
}

void DatabasesPhysicalModelPlugin::generatePhysicalModel()
{
//	mDatabasesGenerator->checkCorrectness();
}

}
}
