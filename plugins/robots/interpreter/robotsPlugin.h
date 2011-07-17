#pragma once

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace robots {

class RobotsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RobotsPlugin();
	virtual ~RobotsPlugin();

	virtual void initPlugin(qrRepo::RepoControlInterface *repo);
	virtual QList<qReal::CustomToolInterface *> toolInterfaces();
	virtual QList<InterpreterInterface *> interpreterInterfaces();

private:

};

}
