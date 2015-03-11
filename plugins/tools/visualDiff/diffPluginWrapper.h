#pragma once

#include <QtCore/QObject>

#include <qrgui/versioning/diffPluginInterface.h>
#include <qrutils/versioningUtils/briefVersioningInterface.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "model/modelLoader.h"


namespace versioning
{

class DiffPluginWrapper : public QObject, public qReal::DiffPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.versioning.diff")

public:
	DiffPluginWrapper();

	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();
	virtual QList<qReal::ActionInfo> actions();
	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QPair<QString, qReal::gui::PreferencesPage *> preferencesPage();

	void configure(
		qReal::ProjectManagementInterface *projectManager
		, qReal::ErrorReporterInterface *errorReporter
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		, qReal::BriefVersioningInterface *vcs
		, QWidget *parent, qReal::EditorManagerInterface *manager
	);

public slots:
	void showDiff(QString const &targetProject, QWidget *parentWidget, bool compactMode = false);
	void showDiff(QString repoRevision, QString const &targetProject, QWidget *parentWidget, bool compactMode = false);
	void showDiff(
		QString oldRepoRevision
		, QString newRepoRevision
		, QString const &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	);

private slots:
	void onModelLoaded(DiffModel *model);

private:
	qrRepo::WorkingCopyManagementInterface *mWorkingCopyManager;
	qReal::BriefVersioningInterface *mVcs;
	QWidget *mMainWindow;
	qReal::EditorManagerInterface *mEditorManager;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::ProjectManagementInterface *mProjectManager;
	details::ModelLoader *mLoader;
	QWidget *mParentWidget;
	bool mCompactMode;
};

}
