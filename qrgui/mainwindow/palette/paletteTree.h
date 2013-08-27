#pragma once

#include <QtWidgets/QTreeWidget>
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <qrkernel/ids.h>

#include "mainwindow/mainWindow.h"
#include "pluginManager/proxyEditorManager.h"
#include "mainwindow/palette/paletteTreeWidgets.h"

namespace  qReal {
namespace gui {

/// Class for representing tree with elements of the editors.
class PaletteTree: public QWidget
{
	Q_OBJECT

public:
	explicit PaletteTree(QWidget *parent = 0);
	~PaletteTree();

	/** Adds all editor's elements to appropriate tree.
	  @param editorManager Editor manager which all editors with elements are taken from.
	  @param editor Editor
	  @param diagram Diagram that corresponds to chosen editor.
	*/
	void addEditorElements(EditorManagerInterface &editorManagerProxy, const Id &editor, const Id &diagram);

	/// Initialize connection editor's combobox with slot that sets active editor.
	void initDone();

	/** Delete chosen editor.
	  @param id Editor id.
	*/
	void deleteEditor(Id const &id);
	QComboBox* comboBox() const;
	QVector<QString> editorsNames() const;
	Id currentEditor() const;

	/// Set item with such id as active in ComboBox.
	void setComboBox(Id const &id);
	void setIconsView(bool iconsView);
	bool iconsView() const;
	void setItemsCountInARow(int count);
	int itemsCountInARow() const;

	/// Set item with such index as active in ComboBox.
	void setComboBoxIndex(int index);

	/// Set saved item index as current in ComboBox.
	void setComboBoxIndex();

	/// Saves selected editor and expanded groups into settings
	void saveConfiguration();

	/** Load palette and set some representation.
	  @param isIconsView This variable corresponds to representation.
	  @param itemsCount Items count in a row.
	  @param editorManager Editor manager which all editors with elements are taken from.
	*/
	void loadPalette(bool isIconsView, int itemsCount, EditorManagerInterface *editorManagerProxy);
	void initMainWindow(MainWindow *mainWindow);

signals:
	void paletteParametersChanged();

public slots:
	/// Change expanded/collapsed state of current tree.
	void changeExpansionState();

	/// Expand all nodes of current tree.
	void expand();

	/// Collapse all nodes of current tree.
	void collapse();

	void setActiveEditor(int index);
	void setActiveEditor(Id const &id);

	/// Recreate PaletteTree.
	void recreateTrees();

	/// Changes widget representation.
	void changeRepresentation();

private:
	/// Change icon and tooltip
	void setExpansionButtonAppearance();

	/// Returns maximum count of items in all rows of widget
	int maxItemsCountInARow() const;

private:
	/// Forbids to make copies of the object.
	explicit PaletteTree(PaletteTree const &paletteTree);

	virtual void resizeEvent(QResizeEvent *);

	/// Creates all PaletteTree widgets.
	void createPaletteTree();

	/// Deletes all PaletteTree widgets.
	void deletePaletteTree();

	/// Fills palette tree by editors.
	/// @param editorManager Editor manager which all editors with elements are taken from.
	void loadEditors(EditorManagerInterface &editorManagerProxy);

	EditorManagerInterface *mEditorManager;

	MainWindow *mMainWindow;

	/// Hash table with editor ids.
	QHash<Id, int> mCategories;

	/// Pointer to current tree.
	PaletteTreeWidgets *mTree;

	/// Button that changes expansion state of current tree
	QToolButton *mChangeExpansionState;

	/// Button that changes palette representation.
	QToolButton *mChangeRepresentation;

	/// Vector with all editor's trees.
	QVector <PaletteTreeWidgets *> mEditorsTrees;

	/// Vector with all editor's names.
	QVector <QString> mEditorsNames;

	/// Combobox with editors.
	QComboBox *mComboBox;

	/// Main layout of the widget.
	QVBoxLayout *mLayout;

	/// Current editor number.
	int mCurrentEditor;

	/// Representation flag
	bool mIconsView;

	/// Whether expand/collapse button expands tree
	bool mNodesStateButtonExpands;

	/// Count of items in a row in icon's representation
	int mItemsCountInARow;
};

}
}
