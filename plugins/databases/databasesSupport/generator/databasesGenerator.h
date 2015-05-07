#pragma once

#include <QtCore/QTranslator>
#include <qrgui/mainwindow/errorReporter.h>
#include <qrkernel/ids.h>
#include "datatypesChecker.h"
#include "databasesPreferencesPage.h"

namespace qReal {
namespace databasesSupport {

class DatabasesGenerator : public QObject
{
	Q_OBJECT
public:
	DatabasesGenerator(PluginConfigurator const configurator, DatabasesPreferencesPage const *preferencesPage);
	~DatabasesGenerator();

	void activeTabChanged(qReal::Id const &rootElementId);
	void clearPhysicalModel();
public slots:
	bool checkCorrectness();
	void generatePhysicalModel();

	void generateSQLCode();
	void changeEditor(QString const &dbmsName);

private:
	void generateWithSqlServer2008();
	void generateWithMySql5();
	void generateWithSqlite();
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::Id createElementFromString(QString const &elemName, QPointF coord = QPointF(), Id const &parentLogicalId = Id::rootId(), bool coordByParent = false);
	qReal::Id makeColumnFromAttribute(Id const &attributeId, Id const &parentId);
	qReal::Id makeTableFromEntity(Id const &entityId, Id const &parentId);
	qReal::Id makeTableFromEntitySet(IdList const &set, Id const &parentId);
	qReal::Id copyOneToManyRelationship(Id const &logicalModelId, Id const &parentId, Id const &from, Id const &to);
	qReal::Id copyManyToManyRelationship(Id const &logicalModelId, Id const &parentId, Id const &from, Id const &to);

	QList<IdList> processEntities(Id const &logicalDiagramId);
	bool processOneToManyRelationships(QList<IdList> oneToOneBoundedEntitiesSets, IdList setTables, int **match, Id const &logicalDiagramId);
	bool processManyToManyRelationships(QList<IdList> oneToOneBoundedEntitiesSets, IdList setTables, int **match, Id const &logicalDiagramId);

	//QList<qReal::ActionInfo> mActionInfos;
	//QTranslator mAppTranslator;

	void setCodeFileName(QString const &name);
	void setWorkDir(QString const &path);
	IdList findNodes(QString const &name);
	QVariant getProperty(Id const &id, QString const &propertyName);
	Id getParent(Id const &id);
	IdList getChildren(Id const &id);
	IdList getBoundedWithOneToOneRealationship(Id const &id);
	void error(QString error, bool isCritical);
	Id getPrimaryKey(Id const &entityId);

	Id getPrimaryKeyOfSet(IdList const &entitySet);
	QString getPrimaryKeyNameOfSet(IdList const &entitySet);
	int getParentList(Id const &childEntity,QList<IdList> set);
	QString getListTableName(IdList const &list);
	bool checkEntities();
	bool checkAttributes();
	bool checkRelationships();

	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface &mGraphicalModelApi;
	Id mCurrentDiagram;
	Id mCurrentId;
	IdList mPassedElements;
	QString mCodeFileName;
	QString mWorkDir;
	QFile codeFile;
	QString mDbms;
	DatabasesPreferencesPage const *mPreferencesPage;
	gui::MainWindowInterpretersInterface &mMainWindowInterface;
	//DatatypesChecker *mDatatypesChecker;
};

}
}

