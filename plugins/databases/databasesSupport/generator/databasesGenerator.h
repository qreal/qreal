#pragma once

#include <QtCore/QTranslator>
#include <qrgui/mainwindow/errorReporter.h>
#include <qrkernel/ids.h>
#include "datatypesChecker.h"

namespace qReal {
namespace databasesSupport {

/// Databases generator for generating SQL code
class DatabasesGenerator: public QObject
{
	Q_OBJECT
public:
	DatabasesGenerator(PluginConfigurator const configurator);
	virtual ~DatabasesGenerator();

	void activeTabChanged(qReal::Id const &rootElementId);
public slots:
	void checkCorrectness();
	/// Generate code from block diagram and build it in executable file
	void generateSQL();

	//bool wereErrors();

private:
	/*enum ErrorType {
		missingBeginNode,
		noErrors
	};*/

	//qReal::VisualDebugger *mVisualDebugger;
	qReal::ErrorReporterInterface *mErrorReporter;
	//QList<qReal::ActionInfo> mActionInfos;

	//qReal::BlockParser *mParser;

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
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	//VisualDebugger::ErrorType mError;
	Id mCurrentDiagram;
	Id mCurrentId;
	IdList mPassedElements;

	//QMap<int, Id> mIdByLineCorrelation;
	//bool mHasCodeGenerationError;
	//bool mHasNotEndWithFinalNode;
	QString mCodeFileName;
	QString mWorkDir;
	QFile codeFile;

	DatatypesChecker *mDatatypesChecker;

	//int tabsCount;
};
}
}
