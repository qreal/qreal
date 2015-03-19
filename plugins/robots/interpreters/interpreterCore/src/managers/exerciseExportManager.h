#pragma once

namespace qReal {
class LogicalModelAssistInterface;
class ProjectManagementInterface;
}

namespace qrRepo {
class RepoControlInterface;
}

namespace interpreterCore {

class ExerciseExportManager
{
public:
	explicit ExerciseExportManager(qReal::LogicalModelAssistInterface &logicalModel
			, qrRepo::RepoControlInterface &repoControlApi, qReal::ProjectManagementInterface &projectManager);

	~ExerciseExportManager();

	void save();

private:
	qReal::LogicalModelAssistInterface &mLogicalModel;
	qrRepo::RepoControlInterface &mRepoControlApi;
	qReal::ProjectManagementInterface &mProjectManager;
};

}
