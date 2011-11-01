#include "exterminatusPlugin.h"

Q_EXPORT_PLUGIN2(exterminatus, exterminatus::ExterminatusPlugin)

using namespace exterminatus;

ExterminatusPlugin::ExterminatusPlugin()
{
}

ExterminatusPlugin::~ExterminatusPlugin()
{
}

void ExterminatusPlugin::initPlugin(qrRepo::RepoControlInterface *repo)
{
	mExterminatusTool.init(repo);
}

QList<qReal::CustomToolInterface *> ExterminatusPlugin::toolInterfaces()
{
	QList<qReal::CustomToolInterface *> result;
	result << &mExterminatusTool;
	return result;
}
