#include "databasesSupportPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>

namespace qReal {
namespace databasesSupport {

DatabasesSupportPlugin::DatabasesSupportPlugin()
	: mPreferencesPage(new DatabasesPreferencesPage())
	, mGenerateCodeAction(NULL)
{
}

DatabasesSupportPlugin::~DatabasesSupportPlugin()
{
	delete mPreferencesPage;
}


void DatabasesSupportPlugin::init(PluginConfigurator const &configurator)
{
	mDatabasesGenerator = new DatabasesGenerator(configurator);
	initActions();
}

QPair<QString, PreferencesPage *> DatabasesSupportPlugin::preferencesPage()
{
	return qMakePair(tr("Databases"), static_cast<PreferencesPage*>(mPreferencesPage));
}

qReal::Customizer* DatabasesSupportPlugin::customizationInterface()
{
	return &mCustomizer;
}

QList<qReal::ActionInfo> DatabasesSupportPlugin::actions()
{
	return mActionInfos;
}

void DatabasesSupportPlugin::initActions()
{
	mCheckCorectnessAction = new QAction(tr("Check correctness"), NULL);
	connect(mCheckCorectnessAction, SIGNAL(triggered()), this, SLOT(checkCorrectness()));

	mGenerateCodeAction = new QAction(tr("Generate SQL"), NULL);
	//ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mDatabasesMenu = new QMenu(tr("Databases"));
	mDatabasesMenu->addAction(mGenerateCodeAction);
	mDatabasesMenu->addAction(mCheckCorectnessAction);
	ActionInfo databasesMenuInfo(mDatabasesMenu, "tools");

	mActionInfos << databasesMenuInfo;
}

void DatabasesSupportPlugin::checkCorrectness()
{
	mDatabasesGenerator->checkCorrectness();
}

void DatabasesSupportPlugin::generateCode()
{
	mDatabasesGenerator->generateSQL();
}
}
}
