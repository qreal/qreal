#pragma once

#include <QtWidgets/QTreeWidget>

#include "../pluginManager/editorManagerInterface.h"

namespace qReal {
namespace gui {

class PaletteTree;

/// Represents tree-like view for elements palette (both editor and user)
class PaletteTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PaletteTreeWidget(PaletteTree &parent, MainWindow &mainWindow
			, EditorManagerInterface &editorManagerProxy
			, bool editable);

	void addGroups(QMap<QString, QStringList> const &groups
			, QMap<QString, QString> const &descriptions
			, IdList const &allElements, bool hideIfEmpty
			, QString const &diagramFriendlyName);

	/// Adds item type to some editor's tree.
	/// @param id Item id.
	/// @param name Item name.
	/// @param description Item description.
	/// @param icon Item icon.
	/// @param tree Editor's tree.
	/// @param parent Parent of item's group.
	void addItemType(Id const &id, QString const &name, QString const &description
			, QIcon const &icon, QSize const &preferredSize
			, QTreeWidgetItem *parent);

	/// Collapses all nodes of all current trees.
	void collapse();

	/// Expands all nodes of all current trees.
	void expand();

	static void sortByFriendlyName(IdList &ids);

	void editItem(QTreeWidgetItem * const item);

protected:
	void mousePressEvent(QMouseEvent *event);

private slots:
	void addElementPaletteActionTriggered();

private:
	/// Adds group of editor's elements to appropriate tree to some top element.
	/// @param tmpList List with sorted group elements.
	/// @param item Editor's tree node for adding in it tmpList.
	void addItemsRow(IdList const &tmpIdList, QTreeWidgetItem *item);

	/// Recursive procedure that collapses node with his children.
	/// @param item Node which will be collapsed with all his children.
	void collapseChildren(QTreeWidgetItem *item);

	/// Recursive procedure that expands node with his children.
	/// @param item Node which will be expanded with all his children.
	void expandChildren(QTreeWidgetItem *item);

	/// Method-comparator for sorting Ids by displayed name.
	/// Needs EditorManager instance to work,
	/// but qSort() prohibits it to be a member of an object.
	/// So making it static does the trick.
	static bool idLessThan(Id const &s1, Id const &s2);

	/// Made static to be used inside idLessThan()
	static EditorManagerInterface *mEditorManager; // Does not take ownership
	MainWindow &mMainWindow;
	PaletteTree &mPaletteTree;
	bool mEditable;
};

}
}
