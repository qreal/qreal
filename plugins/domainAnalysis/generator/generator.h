#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "../../../qrrepo/logicalRepoApi.h"

//#include "compilerPage.h"

namespace domainAnalysis {

/// Main plugin class for metaeditor support code. Provides generation of editor
/// by metamodel specified in .xml file or in QReal save file, compilation of
/// generated editor and loading it to QReal, and parsing of existing .xml metamodel.
class generator: public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "metaEditor.MetaEditorSupportPlugin")

public:
	generator();
	virtual ~generator();
	//explicit generator(qrRepo::LogicalRepoApi &api);
	QHash<qReal::Id, QPair<QString, QString> > getMetamodelList();
	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	void onGenerateMetamodel();

private:

	/// Action that starts generation of .xml file with metamodel for qrxc
	QAction mGenerateMetamodelAction;

	/// Interface of MainWindow, used, for example, to reinit models when finished parsing
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository, mutable to allow parser build model
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership

	/// Control API of a repository, used for requests such as current save file name
	qrRepo::RepoControlInterface *mRepoControlApi;  // Does not have ownership

	qrRepo::RepoApi *mRepo;

};
}
