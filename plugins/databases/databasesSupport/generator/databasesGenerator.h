#pragma once

#include <QtCore/QTranslator>
#include <qrgui/mainwindow/errorReporter.h>
#include <qrkernel/ids.h>

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
	/// Generate code from block diagram and build it in executable file
	void generateSQL();

	//bool wereErrors();

private:
	//qReal::VisualDebugger *mVisualDebugger;
	qReal::ErrorReporterInterface *mErrorReporter;
	//QList<qReal::ActionInfo> mActionInfos;

	//qReal::BlockParser *mParser;

	//QTranslator mAppTranslator;

	void setCodeFileName(QString const &name);
	void setWorkDir(QString const &path);


	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	//VisualDebugger::ErrorType mError;
	Id mCurrentDiagram;
	Id mCurrentId;

	//QMap<int, Id> mIdByLineCorrelation;
	//bool mHasCodeGenerationError;
	//bool mHasNotEndWithFinalNode;
	QString mCodeFileName;
	QString mWorkDir;
	QFile codeFile;

	//int tabsCount;
};
}
}
