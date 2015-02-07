#pragma once

#include <QtWidgets/QTreeWidget>

#include "mainWindow/palette/paletteElement.h"
#include "mainWindow/palette/draggableElement.h"
#include "plugins/pluginManager/editorManagerInterface.h"

namespace qReal {
namespace gui {

class PaletteTree;

/// Represents tree-like view for elements palette (both editor and user)
class PaletteTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PaletteTreeWidget(PaletteTree &parent, MainWindow &mainWindow
			, const EditorManagerInterface &editorManagerProxy
			, bool editable);

	void addGroups(QList<QPair<QString, QList<PaletteElement>>> &groups
			, QMap<QString, QString> const &descriptions
			, bool hideIfEmpty
			, const QString &diagramFriendlyName
			, bool sort);

	/// Collapses all nodes of all current trees.
	void collapse();

	/// Expands all nodes of all current trees.
	void expand();

	static void sortByFriendlyName(IdList &ids);
	static void sortByFriendlyName(QList<PaletteElement> &elements);

	void editItem(QTreeWidgetItem * const item);

	void setElementVisible(const Id &metatype, bool visible);

	void setVisibleForAllElements(bool visible);

	void setElementEnabled(const Id &metatype, bool enabled);

	void setEnabledForAllElements(bool enabled);

protected:
	void mousePressEvent(QMouseEvent *event);

private slots:
	void addElementPaletteActionTriggered();

private:
	/// Adds group of editor's elements to appropriate tree to some top element.
	/// @param tmpList List with sorted group elements.
	/// @param item Editor's tree node for adding in it tmpList.
	void addItemsRow(QList<PaletteElement> const &items, QTreeWidgetItem *item);

	/// Adds item type to some editor's tree.
	/// @param data Palette element properties (such as title and icon)
	/// @param parent Parent of item's group.
	void addItemType(const PaletteElement &data, QTreeWidgetItem *parent);

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
	static bool idLessThan(const Id &s1, const Id &s2);

	/// Same as idLessThan (compares ids of given operands)
	static bool paletteElementLessThan(const PaletteElement &s1, const PaletteElement &s2);

	/// Made static to be used inside idLessThan()
	static const EditorManagerInterface *mEditorManager;  // Does not take ownership
	MainWindow &mMainWindow;
	PaletteTree &mPaletteTree;
	bool mEditable;

	QHash<Id, DraggableElement *> mPaletteElements;  // Takes ownership.
	QHash<Id, QTreeWidgetItem *> mPaletteItems;  // Takes ownership.

};

}
}
