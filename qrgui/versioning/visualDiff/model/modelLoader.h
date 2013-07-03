#pragma once

#include <QtCore/QObject>

#include "../../../toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../../qrutils/versioningUtils/briefVersioningInterface.h"
#include "diffModel.h"

namespace versioning
{
namespace details
{

class ModelLoader : public QObject
{
	Q_OBJECT

public:
	ModelLoader(qReal::BriefVersioningInterface *vcs
			, qReal::ErrorReporterInterface *errorReporter
			, qReal::EditorManagerInterface *editorManager
			, qrRepo::WorkingCopyManagementInterface *workingCopyManager);

	void startModelLoading(QString const &targetProject = QString());
	void startModelLoading(int repoRevision, QString const &targetProject = QString());
	void startModelLoading(int oldRepoRevision, int newRepoRevision, QString const &targetProject = QString());

signals:
	void modelLoaded(DiffModel *model);
	void internalModelLoaded(qReal::models::Models *model);

private slots:
	void onDownloadingComplete(bool success, QString const &targetProject);
	void onOldModelLoaded(qReal::models::Models *model);
	void onNewModelLoaded(qReal::models::Models *model);

private:
	QString tempProject() const;

	qReal::models::Models *loadFromDisk(QString const &targetProject);

	void finishModelLoading();

	void reportError(QString const &message);
	void reportWarning(QString const &message);

	qReal::BriefVersioningInterface *mVcs;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::EditorManagerInterface *mEditorManager;
	qrRepo::WorkingCopyManagementInterface *mWorkingCopyManager;
	QString mTempProject;

	qReal::models::Models *mOldModel;
	qReal::models::Models *mNewModel;
	QString mRepoUrl;
	int mNewRevision;
};

}
}
