#pragma once

namespace qReal {
class LogicalModelAssistInterface;
}

namespace qrRepo {
class RepoControlInterface;
}

namespace interpreterCore {

class ExerciseExportManager
{
public:
	explicit ExerciseExportManager(qReal::LogicalModelAssistInterface &logicalModel
			, qrRepo::RepoControlInterface &repoControlApi);

	~ExerciseExportManager();

	void save();

private:
	qReal::LogicalModelAssistInterface &mLogicalModel;
	qrRepo::RepoControlInterface &mRepoControlApi;
};

}
