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
{
}

DatabasesPhysicalModelPlugin::~DatabasesPhysicalModelPlugin()
{
//	delete mPreferencesPage;
}


void DatabasesPhysicalModelPlugin::init(PluginConfigurator const &configurator)
{
	mModelApi = new ModelApiHelper(configurator);
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
	connect(mGeneratePhysicalModelAction, SIGNAL(triggered()), this, SLOT(generatePhysicalModel()));

	mDatabasesMenu = new QMenu(tr("Physical model.."));
	mDatabasesMenu->addAction(mGeneratePhysicalModelAction);
	ActionInfo databasesMenuInfo(mDatabasesMenu, "tools");

	mActionInfos << databasesMenuInfo;
}

void DatabasesPhysicalModelPlugin::generatePhysicalModel()
{
	Id id = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/DatabasesPhysicalNode"));
	Id created = mModelApi->logicalModelApi.createElement(Id::rootId(), id);
	mModelApi->graphicalModelApi.createElement(Id::rootId(), created, true, QString("Databases Physical Model"), QPointF(23.0, 23.0));
}

}
}
