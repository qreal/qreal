#pragma once

#include <QtCore/QString>
#include <QtCore/QPair>

#include <QtWidgets/QProgressBar>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

class QRUTILS_EXPORT PluginLoader: public QObject
{
	Q_OBJECT

public:
	PluginLoader(gui::MainWindowInterpretersInterface &interpretersInterface);

	void loadNewEditor(const QString &directoryName
			, const QString &pluginsName
			, const QString &pluginId
			, const QString &commandFirst
			, const QString &commandSecond
			, const QString &extension
			, const QString &prefix
			, const QString &buildConfiguration);

private:
	QString fullPluginName(const QString &pluginName
			, const QString &buildConfiguration
			, const QString &extension
			, const QString &prefix) const;

	QProgressBar *progressBar() const;

	void deleteGeneratedFiles(const QString &directoryName
			, const QString &pluginName);

	gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
};

}
