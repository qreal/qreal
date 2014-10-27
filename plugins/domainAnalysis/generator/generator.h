#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrrepo/logicalRepoApi.h>

namespace domainAnalysis {

/// Main plugin class for domainAnalysis generator code. Provides language
/// metamodel generator by domain model
class Generator: public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "domainAnalysis.Generator")

public:
	Generator();
	virtual ~Generator();
	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	void onGenerateMetamodel();

private:

	/// Action that starts generation the metamodel by domain model
	QAction mGenerateMetamodelAction;

	/// Interface of MainWindow
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership
};
}
