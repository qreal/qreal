#pragma once

#include <QtCore/QObject>

#include "../../qrgui/versioning/diffPluginBase.h"
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
			, qReal::MainWindow *mainWindow, qReal::EditorManager *manager);

public slots:
	void showDiff(QString const &targetProject = QString());
	void showDiff(int repoRevision, QString const &targetProject = QString());
	void showDiff(int oldRepoRevision , int newRepoRevision
			, QString const &targetProject = QString());

private slots:
	void onModelLoaded(DiffModel *model);

private:
	qReal::BriefVersioningInterface *mVcs;
	qReal::MainWindow *mMainWindow;
	qReal::EditorManager *mEditorManager;
	qrRepo::WorkingCopyManagementInterface *mWorkingCopyManager;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::ProjectManagementInterface *mProjectManager;
	details::ModelLoader *mLoader;
};

}
