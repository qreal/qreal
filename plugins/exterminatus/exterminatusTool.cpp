#include "exterminatusTool.h"

using namespace exterminatus;

ExterminatusTool::ExterminatusTool()
	: mRepo(NULL)
	, mAction(tr("exterminate"), NULL)
{
	connect(&mAction, SIGNAL(triggered()), this, SLOT(exterminate()));
}

ExterminatusTool::~ExterminatusTool()
{
}

QList<qReal::CustomToolInterface::ActionInfo> ExterminatusTool::actions()
{
	ActionInfo info(&mAction, "file", "tools");
	QList<ActionInfo> result;
	result << info;
	return result;
}

void ExterminatusTool::init(qrRepo::RepoControlInterface *repo)
{
	mRepo = repo;
}

void ExterminatusTool::exterminate()
{
	mRepo->exterminate();
}
