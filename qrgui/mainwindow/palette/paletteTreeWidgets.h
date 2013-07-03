#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

#include "paletteTreeWidget.h"

namespace qReal {
namespace gui {

/// Represents a widget with two palettes: current selected language palette
/// (which is visible always) and time-to-time appearing user palette
class PaletteTreeWidgets : public QWidget
{
public:
	PaletteTreeWidgets(PaletteTree &parent, MainWindow &mainWindow
			, EditorManagerInterface &editorManagerProxy);

	PaletteTreeWidgets(PaletteTree &parent, MainWindow &mainWindow
			, EditorManagerInterface &editorManagerProxy
			, Id const &editor, Id const &diagram);

	/// Adds top item type to some editor's tree.
	/// @param id Item id.
	/// @param name Item name.
	/// @param description Item description.
	/// @param icon Item icon.
	/// @param tree Editor's tree.
	void addTopItemType(Id const &id, QString const &name, QString const &description
			, QIcon const &icon, QSize const &preferredSize, QTreeWidget *tree);

	/// Collapses all nodes of all current trees.
	void collapse();

	/// Expands all nodes of all current trees.
	void expand();

	/// Change icon's sizes in widget
	void resizeIcons();

	/// Returns maximum count of items in all rows of widget
	int maxItemsCountInARow() const;

	/// Saves expanded groups into settings
	void saveConfiguration(QString const &title) const;

private:
	void initWidget();
	void initWidget(PaletteTreeWidget * const tree, QVBoxLayout * const layout);
	void initEditorTree();
	void initUserTree();

	/// Saves expanded groups into settings
	void saveConfiguration(PaletteTreeWidget const *tree, QString const &title) const;

	EditorManagerInterface *mEditorManager; // Does not take ownership
	PaletteTree *mParentPalette; // Does not take ownership
	MainWindow *mMainWindow; // Does not take ownership
	Id mEditor;
	Id mDiagram;
	PaletteTreeWidget *mEditorTree; // Takes ownership
	PaletteTreeWidget *mUserTree; // Takes ownership
};

}
}
