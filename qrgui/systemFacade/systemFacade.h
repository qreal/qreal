#pragma once

#include <qrgui/plugins/pluginManager/proxyEditorManager.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/models/models.h>

namespace qReal {

/// Initializes and provides all main backend parts of the visual modeling system.
/// This class is just a convenient way to use system infrastrucure from different frontends.
class SystemFacade
{
public:
	SystemFacade();

	/// Returns a reference to an entity that obtains and manages the plugins with visual languages descriptions.
	EditorManagerInterface &editorManager();

	/// Returns a reference to models facade object.
	models::Models &models();

	/// Returns a reference to an object that can be used for
	/// subscription to events inside the visual modeling system engine.
	SystemEvents &events();

private:
	ProxyEditorManager mEditorManager;
	models::Models mModels;
	SystemEvents mEvents;
};

}
