#pragma once

namespace qReal {
class LogicalModelAssistInterface;
}

namespace qrRepo {
class RepoControlInterface;
}

namespace interpreterCore {

class SaveAsTaskManager
{
public:
	explicit SaveAsTaskManager(qReal::LogicalModelAssistInterface &logicalModel
			, qrRepo::RepoControlInterface &repoControlApi);

	~SaveAsTaskManager();

	void save();

private:
	qReal::LogicalModelAssistInterface &mLogicalModel;
	qrRepo::RepoControlInterface &mRepoControlApi;
};

}
