#pragma once

#include <QtCore/QObject>

#include "../versioning/diffPluginBase.h"
#include "model/modelLoader.h"

namespace versioning
{

class DiffPluginWrapper : public QObject, public qReal::DiffPluginInterface
{
	Q_OBJECT

public:
	DiffPluginWrapper(qReal::DiffPluginBase *plugin
			, qrRepo::WorkingCopyManagementInterface *workingCopyManager
			, qReal::BriefVersioningInterface *vcs
			, qReal::MainWindow *mainWindow
			, qReal::EditorManagerInterface *manager);

public slots:
	void showDiff(QString const &targetProject, QWidget *parentWidget);
	void showDiff(QString repoRevision, QString const &targetProject, QWidget *parentWidget);
	void showDiff(QString oldRepoRevision , QString newRepoRevision
			, QString const &targetProject, QWidget *parentWidget);

private slots:
	void onModelLoaded(DiffModel *model);

private:
	qrRepo::WorkingCopyManagementInterface *mWorkingCopyManager;
	qReal::BriefVersioningInterface *mVcs;
	qReal::MainWindow *mMainWindow;
	qReal::EditorManagerInterface *mEditorManager;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::ProjectManagementInterface *mProjectManager;
	details::ModelLoader *mLoader;
	QWidget *mParentWidget;
};

}
