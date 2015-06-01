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
			, const Id &editor, const Id &diagram);

	/// Adds top item type to some editor's tree.
	/// @param data Parameters of new element
	/// @param tree Editor's tree.
	void addTopItemType(const PaletteElement &data, QTreeWidget *tree);

	/// Collapses all nodes of all current trees.
	void collapse();

	/// Expands all nodes of all current trees.
	void expand();

	/// Change icon's sizes in widget
	void resizeIcons();

	/// Returns maximum count of items in all rows of widget
	int maxItemsCountInARow() const;

	/// Saves expanded groups into settings
	void saveConfiguration(const QString &title) const;

	void setElementVisible(const Id &metatype, bool visible);

	void setVisibleForAllElements(bool visible);

	void setElementEnabled(const Id &metatype, bool enabled);

	void setEnabledForAllElements(bool enabled);

	/// Rereads user blocks information.
	void refreshUserPalette();

	/// Sets user palette header and description.
	void customizeExplosionTitles(const QString &userGroupTitle
			, const QString &userGroupDescription);

	/// Filters contents of both trees showing only items whoose names matches the given regular expression.
	void filter(const QRegExp &regexp);

private:
	void initWidgets();
	void initWidget(PaletteTreeWidget * const tree);
	void initEditorTree();
	void initUserTree();

	/// Saves expanded groups into settings
	void saveConfiguration(const PaletteTreeWidget *tree, const QString &title) const;

	EditorManagerInterface *mEditorManager; // Does not take ownership
	PaletteTree *mParentPalette; // Does not take ownership
	MainWindow *mMainWindow; // Does not take ownership
	Id mEditor;
	Id mDiagram;
	PaletteTreeWidget *mEditorTree; // Takes ownership
	PaletteTreeWidget *mUserTree; // Takes ownership
	QHash<Id, DraggableElement *> mPaletteElements; // Does not take ownership.

	QString mUserGroupTitle;
	QString mUserGroupDescription;
};

}
}
