#pragma once

#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"

#include "exterminatusTool.h"

namespace exterminatus {

class ExterminatusPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	ExterminatusPlugin();
	virtual ~ExterminatusPlugin();

	void initPlugin(qrRepo::RepoControlInterface *repo);
	QList<qReal::CustomToolInterface *> toolInterfaces();
private:

	ExterminatusTool mExterminatusTool;
};

}
