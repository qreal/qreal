#pragma once

#include <QtCore/QDir>
#include <QtCore/QList>

#include "../toolPluginInterface/toolPluginInterface.h"
#include "../toolPluginInterface/customToolInterface.h"

namespace qReal {

class ToolPluginManager
{
public:
	explicit ToolPluginManager(QObject *parent = NULL);
	virtual ~ToolPluginManager();

	void init(qrRepo::RepoControlInterface *repo);
	QList<CustomToolInterface::ActionInfo> actions() const;

private:
	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;
};

}
