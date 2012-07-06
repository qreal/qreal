#pragma once

#include "../../qrkernel/ids.h"
#include "../toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class ErrorReporter;

class MainWindowInterpretersInterface
{
public:
	virtual ~MainWindowInterpretersInterface() {}
	virtual void selectItem(Id const &graphicalId) = 0;
	virtual void selectItemOrDiagram(Id const &graphicalId) = 0;
	virtual void highlight(Id const &graphicalId, bool exclusive = true) = 0;
	virtual void dehighlight(Id const &graphicalId) = 0;
	virtual void dehighlight() = 0;
	virtual ErrorReporterInterface *errorReporter() = 0;
	virtual Id activeDiagram() = 0;
	virtual void openSettingsDialog(QString const &tab) = 0;

	/// Save currently opened model
	virtual void saveAll() = 0;

	/// Opens new tab with text editor and shows a text in it
	/// @param title A title of the tab
	/// @param text A text that should be shown in an editor
	virtual void showInTextEditor(QString const &title, QString const &text) = 0;

	/// Rereads model information from repository and reinitializes models
	/// and all related views. Needs to be called after major changes in repo.
	virtual void reinitModels() = 0;

	/// Returns reference to MainWindow to use as a parent in different plugin windows
	virtual QWidget *windowWidget() = 0;

	/// Tries to unload editor plugin with given name.
	/// @param pluginName Name of an editor plugin to unload
	/// @returns True, if plugin unloaded successfully or no plugin with such name
	/// was loaded, false if plugin is not unloaded.
	virtual bool unloadPlugin(QString const &pluginName) = 0;

	/// Tries to load editor plugin with given name.
	/// @param fileName Name of an editor plugin file to load (.dll or .so name without extension)
	/// @param pluginName Name of an editor plugin itself (as it will return in id())
	/// @returns True, if plugin loaded successfully, false if plugin is not loaded.
	virtual bool loadPlugin(QString const &fileName, QString const &pluginName) = 0;

	/// Checks if an editor plugin is loaded
	/// @param pluginName Name of a plugin to check (as it will return in id() method)
	/// @returns True, if plugin is present and loaded
	virtual bool pluginLoaded(QString const &pluginName) = 0;

	virtual bool unloadConstraintsPlugin(QString const &pluginName, QString const &pluginId) = 0;
	virtual bool loadConstraintsPlugin(QString const &fileName) = 0;
};

}
}
