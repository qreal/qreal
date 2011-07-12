#pragma once

#include <QtCore/QObject>
#include <QtCore/QDir>

namespace qReal {

class ToolPluginManager : public QObject
{
	Q_OBJECT

public:
	explicit ToolPluginManager(QObject *parent = NULL);


private:
	QDir mPluginsDir;
};

}
