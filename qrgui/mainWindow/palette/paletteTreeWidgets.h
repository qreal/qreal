#pragma once

#include <QtWidgets/QSplitter>

#include "mainWindow/palette/paletteTreeWidget.h"
#include "draggableElement.h"

namespace qReal {
namespace gui {

/// Represents a widget with two palettes: current selected language palette
/// (which is visible always) and time-to-time appearing user palette
class PaletteTreeWidgets : public QSplitter
{
public:
	PaletteTreeWidgets(PaletteTree &parent, MainWindow *mainWindow
			, EditorManagerInterface &editorManagerProxy);

	PaletteTreeWidgets(PaletteTree &parent, MainWindow *mainWindow
			, EditorManagerInterface &editorManagerProxy
			, Id const &editor, Id const &diagram);

	/// Adds top item type to some editor's tree.
	/// @param data Parameters of new element
	/// @param tree Editor's tree.
	void addTopItemType(PaletteElement const &data, QTreeWidget *tree);

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

	void setElementVisible(Id const &metatype, bool visible);

	void setVisibleForAllElements(bool visible);

	void setElementEnabled(Id const &metatype, bool enabled);

	void setEnabledForAllElements(bool enabled);

private:
	void initWidgets();
	void initWidget(PaletteTreeWidget * const tree);
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
	QHash<Id, DraggableElement *> mPaletteElements; // Does not take ownership.
};

}
}
