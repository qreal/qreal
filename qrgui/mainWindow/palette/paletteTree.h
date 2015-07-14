/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

#include "mainWindow/mainWindow.h"
#include "mainWindow/palette/paletteTreeWidgets.h"
#include "plugins/pluginManager/proxyEditorManager.h"

class QLineEdit;

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
	void deleteEditor(const Id &id);
	QComboBox* comboBox() const;
	QList<QString> editorsNames() const;
	Id currentEditor() const;

	/// Deletes all PaletteTree widgets.
	void deletePaletteTree();

	/// Creates all PaletteTree widgets.
	void createPaletteTree();

	/// Set item with such id as active in ComboBox.
	void setComboBox(const Id &id);
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
	void installEventFilter(QObject *obj);

	void setElementVisible(const Id &metatype, bool visible);

	void setVisibleForAllElements(bool visible);

	void setElementEnabled(const Id &metatype, bool enabled);

	void setEnabledForAllElements(bool enabled);

	/// Sets user palettes headers and descriptions.
	void customizeExplosionTitles(const QString &userGroupTitle
			, const QString &userGroupDescription);

signals:
	void paletteParametersChanged();

public slots:
	/// Expand all nodes of current tree.
	void expand();

	/// Collapse all nodes of current tree.
	void collapse();

	void setActiveEditor(int index);
	void setActiveEditor(const Id &id);

	/// Recreate PaletteTree.
	void recreateTrees();

	/// Changes widget representation.
	void changeRepresentation();

	/// Rereads user blocks information.
	void refreshUserPalettes();

private:
	/// Returns maximum count of items in all rows of widget
	int maxItemsCountInARow() const;

private slots:
	void onSearchTextChanged(const QRegExp &searchText);

private:
	/// Forbids to make copies of the object.
	explicit PaletteTree(const PaletteTree &paletteTree);

	virtual void resizeEvent(QResizeEvent *);

	void initUi();

	/// Fills palette tree by editors.
	/// @param editorManager Editor manager which all editors with elements are taken from.
	void loadEditors(EditorManagerInterface &editorManagerProxy);

	EditorManagerInterface *mEditorManager = nullptr;

	MainWindow *mMainWindow;

	/// Hash table with editor ids.
	QHash<Id, int> mCategories;

	/// Pointer to current tree.
	PaletteTreeWidgets *mTree;

	/// List with all editor's trees.
	QList<PaletteTreeWidgets *> mEditorsTrees;

	/// List with all editor's names.
	QList<QString> mEditorsNames;

	/// Combobox with editors.
	QComboBox *mComboBox;

	/// Main layout of the widget.
	QVBoxLayout *mLayout;

	/// Current editor number.
	int mCurrentEditor;

	/// Representation flag
	bool mIconsView;

	/// Count of items in a row in icon's representation
	int mItemsCountInARow;
};

}
}
