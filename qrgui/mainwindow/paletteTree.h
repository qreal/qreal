#pragma once

#include <QTreeWidget>
#include <QtCore/QHash>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QPushButton>
#include <QComboBox>
#include <QtGui/QVBoxLayout>
#include "../pluginManager/editorManager.h"
#include "../../qrkernel/ids.h"
namespace  qReal{
namespace gui{

/// Class for representing tree with editors elements.
class PaletteTree: public QWidget
{
Q_OBJECT
public:
	explicit PaletteTree(QWidget *parent = 0);

	/** Adds item type to some editors tree.
	  @param id Item id.
	  @param name Item name.
	  @param description Item description.
	  @param icon Item icon.
	  @param tree Editors tree.
	  @param parent Parent group of items.
	*/
	void addItemType(Id const &id, QString const &name, QString const &description
			, QIcon const &icon,QTreeWidget *tree, QTreeWidgetItem *parent);

	/** Adds all editors elements to appropriate tree.
	  @param editorManager Editor manager which all editors with elements are taken from.
	  @param editor Editor
	  @param diagram Diagram that corresponds to the chosen editor.
	*/
	void addEditorElements(EditorManager &editorManager, const Id &editor, const Id &diagram);

	/// Initialize connection editors combobox with slot that sets active editor.
	void initDone();

	/** Delete chosen editor.
	  @param id Editor id.
	*/
	void deleteEditor(Id const &id);
	QComboBox* comboBox() const;
	QVector<QString> editorsNames() const;
	Id currentEditor() const;
	void setComboBox(Id id);

	/// Sets current index in ComboBox.
	void setComboBoxIndex(int index);
	~PaletteTree();
public slots:
	/// Collapse all nodes of current tree.
	void collapse();

	/// Expand all nodes of current tree.
	void expand();
	void setActiveEditor(int index);
	void setActiveEditor(Id id);

	/// Recreate PaletteTree.
	void recreateTrees();
private:

	/// Class for representing editor elements.
	class DraggableElement : public QWidget
	{
	public:
			DraggableElement(Id const &id, QString const &name
					, QString const &description
					, QIcon const &icon, QWidget *parent = NULL);

			QIcon icon() const
			{
				return mIcon;
			}

			QString text() const
			{
				return mText;
			}

			Id id() const
			{
				return mId;
			}

	private:
			Id mId;
		QIcon mIcon;
			QString mText;
			virtual void dragEnterEvent(QDragEnterEvent *event);
			virtual void dropEvent(QDropEvent *event);
			virtual void mousePressEvent(QMouseEvent *event);
	};

	/// EditorManager instance used to sort palette's content.
	/// Made static to be used inside idLessThan()
	static EditorManager *mEditorManager;

	/// Forbids to make copies of the object.
	explicit PaletteTree(const PaletteTree &paletteTree);

	/** Method-comparator for sorting Ids by displayed name.
	Needs EditorManager instance to work,
	but qSort() prohibits it to be a member of an object.
	So making it static does the trick.
	*/
	static bool idLessThan(const Id &s1, const Id &s2);

	/** Recursive procedure that collapses node with his children.
	  @param item Node which will be collapsed with all his children.
	*/
	void collapseChildren(QTreeWidgetItem *item);

	/** Recursive procedure that expands node with his children.
	  @param item Node which will be expanded with all his children.
	*/
	void expandChildren(QTreeWidgetItem *item);

	/// Creates all PaletteTree widgets.
	void createPaletteTree();

	/// Deletes all PaletteTree widgets.
	void deletePaletteTree();

	/// Hash table with editor ids.
	QHash<Id, int> mCategories;

	/// Pointer to current tree.
	QTreeWidget *mTree;

	/// Button that collapses all nodes of current tree.
	QPushButton *mCollapseAll;

	/// Button that expands all nodes of current tree.
	QPushButton *mExpandAll;

	/// Vector with all editors trees.
	QVector <QTreeWidget *> mEditorsTrees;

	/// Vector with all editors names.
	QVector <QString> mEditorsNames;

	/// Combobox with editors.
	QComboBox *mComboBox;

	/// Main layout of the widget.
	QVBoxLayout *mLayout;

	/// Current editor number.
	int mCurrentEditor;
};
}
}
