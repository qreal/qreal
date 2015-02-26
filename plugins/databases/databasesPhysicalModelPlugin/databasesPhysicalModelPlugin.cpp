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
	mPluginConfigurator = &configurator;
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
	//QPointF scenePos = QPointF(371.0, 278.0);
	Id id = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/DatabasesPhysicalNode"));
	//qReal::Id id = qReal::Id::createElementId("DatabasesPhysicalModelMetamodel", "DatabasesPhysicalModelMetamodel", "DatabasesPhysicalNode");
	//QString name = "DatabasesPhysicalNode";
	mPluginConfigurator->logicalModelApi().createElement(Id::rootId(), id);
	//Id created = mPluginConfigurator->logicalModelApi().createElement(Id::rootId(), id);
	//mPluginConfigurator->graphicalModelApi().createElement(Id::rootId(), created, true, QString("smth"), QPointF(23.0, 23.0));
	//mPluginConfigurator->logicalModelApi().createElement()
	//mPluginConfigurator->graphicalModelApi().createElement(Id::rootId(), "DatabasesPhysicalNode");
	//mPluginConfigurator->logicalModelApi().createElement(Id::rootId(), "DatabasesPhysicalNode");
}

}
}
