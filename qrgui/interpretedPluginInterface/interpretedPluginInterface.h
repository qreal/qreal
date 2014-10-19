#pragma once

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtCore/QList>
#include <QtWidgets/QAction>

#include <qrrepo/repoControlInterface.h>
#include <qrrepo/logicalRepoApi.h>

#include "dialogs/preferencesPages/preferencesPage.h"

#include "toolPluginInterface/customizer.h"
#include "toolPluginInterface/pluginConfigurator.h"
#include "toolPluginInterface/actionInfo.h"
#include "toolPluginInterface/hotKeyActionInfo.h"
#include "toolPluginInterface/projectConverter.h"

#include "toolPluginInterface/pluginInterface.h"

namespace qReal {

/// Base class for interpreted plugins.
class InterpretedPluginInterface : public PluginInterface
{
public:
	/// Shall be overriden in concrete plugin to initialize itself.
	/// @param configurator - Object that contains all needed information for a plugin, like refs to models.
	/// @param metamodelRepoApi - Object that contains information about metamodel
	virtual void init(
			PluginConfigurator const &configurator
			, qrRepo::LogicalRepoApi &metamodelRepoApi
			) = 0;
};

}

Q_DECLARE_INTERFACE(qReal::InterpretedPluginInterface, "ru.tepkom.QReal.InterpretedPluginInterface/0.2")
