#pragma once

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>

namespace qReal {

class NullMainWindow : public QObject, public gui::MainWindowInterpretersInterface
{
	Q_OBJECT

public:
	NullMainWindow(ErrorReporterInterface &errorReporter, SystemEvents &events);
	NullMainWindow(ErrorReporterInterface &errorReporter, SystemEvents &events
			, ProjectManagementInterface const &projectManager, GraphicalModelAssistInterface const &graphicalModel);

	void selectItem(const Id &graphicalId) override;
	void selectItemOrDiagram(const Id &graphicalId) override;
	void highlight(const Id &graphicalId, bool exclusive = true, const QColor &color = Qt::red) override;
	void dehighlight(const Id &graphicalId) override;
	void dehighlight() override;
	ErrorReporterInterface *errorReporter() override;

	/// Gets graphical Id of diagram currently opened in editor.
	/// @returns Id of opened diagram, empty Id if there is none.
	/// @todo Make it const if possible.
	Id activeDiagram() const override;

	void openSettingsDialog(const QString &tab) override;

	/// Rereads model information from repository and reinitializes models
	/// and all related views. Needs to be called after major changes in repo.
	void reinitModels() override;

	/// Returns reference to MainWindow to use as a parent in different plugin windows
	QWidget *windowWidget() override;

	/// Tries to unload editor plugin with given name.
	/// @param pluginName Name of an editor plugin to unload
	/// @returns True, if plugin unloaded successfully or no plugin with such name
	/// was loaded, false if plugin is not unloaded.
	bool unloadPlugin(const QString &pluginName) override;

	/// Tries to load editor plugin with given name.
	/// @param fileName Name of an editor plugin file to load (.dll or .so name without extension)
	/// @param pluginName Name of an editor plugin itself (as it will return in id())
	/// @returns True, if plugin loaded successfully, false if plugin is not loaded.
	bool loadPlugin(const QString &fileName, const QString &pluginName) override;

	/// Checks if an editor plugin is loaded
	/// @param pluginName Name of a plugin to check (as it will return in id() method)
	/// @returns True, if plugin is present and loaded
	bool pluginLoaded(const QString &pluginName) override;

	/// Saves active diagram to .png with chosen file name
	/// @param fileName Name for saving
	void saveDiagramAsAPictureToFile(const QString &fileName) override;

	/// automatically arrange elements on active diagram
	/// @param algorithm Way of arrangement
	/// @param absolutePathToDotFiles Path to directory DotFiles
	void arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles) override;

	/// returns selected elements on current tab
	IdList selectedElementsOnActiveDiagram() override;

	/// Shows element`s tab if it is not active and selects element if @see setSelected is true
	void activateItemOrDiagram(const Id &id, bool setSelected = true) override;

	void updateActiveDiagram() override;

	void deleteElementFromDiagram(const Id &id) override;

	/// Must be called before some long operation start.
	/// Shows progress bar on operation start
	/// @param operation Operation that going to be invoced
	void reportOperation(invocation::LongOperation *operation) override;

	QWidget *currentTab() override;
	void openTab(QWidget *tab, const QString &title) override;
	void closeTab(QWidget *tab) override;
	/// Sets text on the header of the tab containing the given widget or does nothing if such tab was not found.
	void setTabText(QWidget *tab, const QString &text) override;

	/// Signals that engine must prepare for modifying blocks set.
	/// After each beginPaletteModification() call there must be endPaletteModification() call.
	void beginPaletteModification() override;

	/// Shows or hides given element on a palette.
	/// @param metatype - id of an element type to be shown/hidden.
	/// @param visible - true, if element shall be visible, false if hidden.
	void setElementInPaletteVisible(const Id &metatype, bool visible) override;

	/// Shows or hides all elements in palette.
	/// @param visible - true, if all elements shall be visible, false if hidden.
	void setVisibleForAllElementsInPalette(bool visible) override;

	/// Disables/enables given element on a palette.
	/// @param metatype - id of an element type to be disabled/enabled.
	/// @param enabled - true, if element shall be enabled, false if disabled (greyed out).
	void setElementInPaletteEnabled(const Id &metatype, bool enabled) override;

	/// Enables or disables all elements in palette.
	/// @param enabled - true, if all elements shall be enabled, false if all elements shall be disabled.
	void setEnabledForAllElementsInPalette(bool enabled) override;

	/// Commits palette modification in the system: shows or hides elements in palette, linker menus,
	/// gestures tab and enables or disables elements on diagram.
	void endPaletteModification() override;

	/// Returns a set of registered preferences pages. Key represents page ID, value - the page itself.
	/// This method may be useful for storing plugins settings on existing pages.
	QMap<QString, gui::PreferencesPage *> preferencesPages() const override;

private:
	void openFirstDiagram();
	void openTabWithEditor(const Id &id);

	ErrorReporterInterface &mErrorReporter;
	SystemEvents &mEvents;
	GraphicalModelAssistInterface const *mGraphicalModel;
	Id mActiveId;
};

}
