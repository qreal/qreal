#pragma once

#include <QtGui/QColor>
#include <QtCore/QFileInfo>

#include <qrkernel/ids.h>
#include <qrutils/invocationUtils/longOperation.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class ErrorReporter;

class MainWindowInterpretersInterface
{
public:
	virtual ~MainWindowInterpretersInterface() {}
	virtual void selectItem(Id const &graphicalId) = 0;
	virtual void selectItemOrDiagram(Id const &graphicalId) = 0;
	virtual void highlight(Id const &graphicalId, bool exclusive = true, QColor const &color = Qt::red) = 0;
	virtual void dehighlight(Id const &graphicalId) = 0;
	virtual void dehighlight() = 0;
	virtual ErrorReporterInterface *errorReporter() = 0;

	/// Gets graphical Id of diagram currently opened in editor.
	/// @returns Id of opened diagram, empty Id if there is none.
	/// @todo Make it const if possible.
	virtual Id activeDiagram() const = 0;

	virtual void openSettingsDialog(QString const &tab) = 0;

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

	/// Saves active diagram to .png with chosen file name
	/// @param fileName Name for saving
	virtual void saveDiagramAsAPictureToFile(QString const &fileName) = 0;

	/// automatically arrange elements on active diagram
	/// @param algorithm Way of arrangement
	/// @param absolutePathToDotFiles Path to directory DotFiles
	virtual void arrangeElementsByDotRunner(QString const &algorithm, QString const &absolutePathToDotFiles) = 0;

	/// returns selected elements on current tab
	virtual IdList selectedElementsOnActiveDiagram() = 0;

	/// Shows element`s tab if it is not active and selects element if @see setSelected is true
	virtual void activateItemOrDiagram(Id const &id, bool setSelected = true) = 0;

	virtual void updateActiveDiagram() = 0;

	virtual void deleteElementFromDiagram(Id const &id) = 0;

	/// Must be called before some long operation start.
	/// Shows progress bar on operation start
	/// @param operation Operation that going to be invoced
	virtual void reportOperation(invocation::LongOperation *operation) = 0;

	virtual QWidget *currentTab() = 0;
	virtual void openTab(QWidget *tab, QString const &title) = 0;
	virtual void closeTab(QWidget *tab) = 0;

	/// Signals that engine must prepare for modifying blocks set.
	/// After each beginPaletteModification() call there must be endPaletteModification() call.
	virtual void beginPaletteModification() = 0;

	/// Shows or hides given element on a palette.
	/// @param metatype - id of an element type to be shown/hidden.
	/// @param visible - true, if element shall be visible, false if hidden.
	virtual void setElementInPaletteVisible(Id const &metatype, bool visible) = 0;

	/// Shows or hides all elements in palette.
	/// @param visible - true, if all elements shall be visible, false if hidden.
	virtual void setVisibleForAllElementsInPalette(bool visible) = 0;

	/// Disables/enables given element on a palette.
	/// @param metatype - id of an element type to be disabled/enabled.
	/// @param enabled - true, if element shall be enabled, false if disabled (greyed out).
	virtual void setElementInPaletteEnabled(Id const &metatype, bool enabled) = 0;

	/// Enables or disables all elements in palette.
	/// @param enabled - true, if all elements shall be enabled, false if all elements shall be disabled.
	virtual void setEnabledForAllElementsInPalette(bool enabled) = 0;

	/// Commits palette modification in the system: shows or hides elements in palette, linker menus,
	/// gestures tab and enables or disables elements on diagram.
	virtual void endPaletteModification() = 0;
};

}
}
