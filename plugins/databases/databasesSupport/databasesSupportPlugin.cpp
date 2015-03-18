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
	, mCheckCorectnessAction(NULL)
{
}

DatabasesSupportPlugin::~DatabasesSupportPlugin()
{
	delete mPreferencesPage;
}


void DatabasesSupportPlugin::init(PluginConfigurator const &configurator)
{
	mDatabasesGenerator = new SqlServerGenerator(configurator);
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

	mGeneratePhysicalModelAction = new QAction(tr("Generate physical model"), NULL);
	connect(mGeneratePhysicalModelAction, SIGNAL(triggered()), this, SLOT(generatePhysicalModel()));

	mDatabasesMenu = new QMenu(tr("Databases"));
	mDatabasesMenu->addAction(mGenerateCodeAction);
	mDatabasesMenu->addAction(mCheckCorectnessAction);
	mDatabasesMenu->addAction(mGeneratePhysicalModelAction);
	ActionInfo databasesMenuInfo(mDatabasesMenu, "tools");

	/*mDatabasesPhysicalModelMenu = new QMenu(tr("Physical model.."));
	mDatabasesPhysicalModelMenu->addAction(mGeneratePhysicalModelAction);
	ActionInfo databasesPhysicalModelMenuInfo(mDatabasesPhysicalModelMenu, "tools");*/

	mActionInfos << databasesMenuInfo;

	//mActionInfos << databasesPhysicalModelMenuInfo;
}

void DatabasesSupportPlugin::checkCorrectness()
{
	mDatabasesGenerator->checkCorrectness();
}

void DatabasesSupportPlugin::generateCode()
{
	mDatabasesGenerator->generateSQLCode();
}

void DatabasesSupportPlugin::generatePhysicalModel()
{
	mDatabasesGenerator->generatePhysicalModel();
	/*Id id = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/DatabasesPhysicalNode"));
	Id created = mModelApi->logicalModelApi.createElement(Id::rootId(), id);
	mModelApi->graphicalModelApi.createElement(Id::rootId(), created, true, QString("Databases Physical Model"), QPointF(23.0, 23.0));

	IdList entityNodes = findNodes("Entity");
	foreach (Id const &entityId, entityNodes) {
		Id tableId = Id::loadFromString(QString("qrm:/DatabasesPhysicalModelMetamodel/DatabasesPhysicalModelMetamodel/Table"));
		Id createdTableId = mModelApi->logicalModelApi.createElement(Id::rootId(), tableId);
		QPointF tableCoordinates = mModelApi->graphicalModelApi.position(mModelApi->graphicalModelApi.graphicalIdsByLogicalId(entityId).first());
		Id createdGraphicalId = mModelApi->graphicalModelApi.graphicalIdsByLogicalId(created).first();
		mModelApi->graphicalModelApi.createElement(createdGraphicalId, createdTableId, true, QString("Table"), tableCoordinates);
		QString tableName = getProperty(entityId, "Name").toString();
	}*/
}

}
}
