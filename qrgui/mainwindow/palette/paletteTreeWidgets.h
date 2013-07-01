#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

#include "paletteTreeWidget.h"

namespace qReal {
namespace gui {

/// Represents a widget with two palettes: current palette
class PaletteTreeWidgets : public QWidget
{
public:
	PaletteTreeWidgets(PaletteTree &parent, MainWindow &mainWindow
			, EditorManagerInterface &editorManagerProxy);

	PaletteTreeWidgets(PaletteTree &parent, MainWindow &mainWindow
			, EditorManagerInterface &editorManagerProxy
			, Id const &editor, Id const &diagram);

	/// Adds item type to some editor's tree.
	/// @param id Item id.
	/// @param name Item name.
	/// @param description Item description.
	/// @param icon Item icon.
	/// @param tree Editor's tree.
	/// @param parent Parent of item's group.
	void addItemType(Id const &id, QString const &name, QString const &description
			, QIcon const &icon, QSize const &preferredSize
			, QTreeWidget *tree, QTreeWidgetItem *parent);

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

	/// Adds group of editor's elements to appropriate tree to some top element.
	/// @param tmpList List with sorted group elements.
	/// @param editorTree Editor's tree
	/// @param item Editor's tree node for adding in it tmpList.
	void addItemsRow(IdList const &tmpIdList, QTreeWidget *editorTree, QTreeWidgetItem *item);

	/// Recursive procedure that collapses node with his children.
	/// @param item Node which will be collapsed with all his children.
	void collapseChildren(QTreeWidgetItem *item);

	/// Recursive procedure that collapses node with his children.
	/// @param item Node which will be collapsed with all his children.
	void collapse(QTreeWidget const *item);

	/// Recursive procedure that expands node with his children.
	/// @param item Node which will be expanded with all his children.
	void expandChildren(QTreeWidgetItem *item);

	/// Expands all children of given tree widget
	void expand(QTreeWidget const *tree);

	/// Saves expanded groups into settings
	void saveConfiguration(PaletteTreeWidget const *tree, QString const &title) const;

	/// Method-comparator for sorting Ids by displayed name.
	/// Needs EditorManager instance to work,
	/// but qSort() prohibits it to be a member of an object.
	/// So making it static does the trick.
	static bool idLessThan(Id const &s1, Id const &s2);

	/// Made static to be used inside idLessThan()
	static EditorManagerInterface *mEditorManager; // Does not take ownership
	PaletteTree *mParentPalette; // Does not take ownership
	MainWindow *mMainWindow; // Does not take ownership
	Id mEditor;
	Id mDiagram;
	PaletteTreeWidget *mEditorTree; // Takes ownership
	PaletteTreeWidget *mUserTree; // Takes ownership
};

}
}
