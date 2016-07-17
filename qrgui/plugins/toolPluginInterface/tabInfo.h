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

#include <qrkernel/ids.h>

namespace qReal {

/// Describes the contents of the tabs opened in main window`s central tab area. Tabs are categorized by their types,
/// also detailed information can be obtained for each special category.
class TabInfo
{
public:
	/// Enumerates all possible types of tabs that can be opened in main window`s central tab area.
	enum class TabType
	{
		/// Represents diagram editor, the core of visual programming environment
		editor = 0
		/// Represents code editor, the core of textual programming environment
		, code
		/// Other type of tab, such as start widget or mouse gestures
		, other
	};

	/// Constructs invalid tab info, can be used when no tab is opened.
	TabInfo()
		: mType(TabType::other)
		, mOpenedWidget(nullptr)
	{
	}

	/// Constructs tab info with 'other' type. Start widget or mouse gestures tab widgets can be passed here.
	explicit TabInfo(QWidget *activeWidget)
		: mType(TabType::other)
		, mOpenedWidget(activeWidget)
	{
	}

	/// Constructs tab info with 'editor' type. The id of the opened diagram and a reference to the editor itself must
	/// be passed.
	TabInfo(const Id &rootDiagramId, QWidget *editorView)
		: mType(TabType::editor)
		, mRootDiagramId(rootDiagramId)
		, mOpenedWidget(editorView)
	{
	}

	/// Constructs tab info with 'code' type. A path to an opened text file and a reference to the editor itself must
	/// be passed.
	TabInfo(const QString &pathToOpenedTextFile, QWidget *editor)
		: mType(TabType::code)
		, mPathToOpenedTextFile(pathToOpenedTextFile)
		, mOpenedWidget(editor)
	{
	}

	/// The expandable, "universal" overload for easy supporting new tab types.
	TabInfo(TabType type, const Id &rootDiagramId, const QString &pathToOpenedTextFile, QWidget *widget)
		: mType(type)
		, mRootDiagramId(rootDiagramId)
		, mPathToOpenedTextFile(pathToOpenedTextFile)
		, mOpenedWidget(widget)
	{
	}

	/// Returns the category of the opened tab.
	TabType type() const
	{
		return mType;
	}

	/// Returns the graphical id of the diagram opened in visual editor or null id if the tab of other type.
	/// The type of the tab can be first asserted using type() method.
	Id rootDiagramId() const
	{
		return mRootDiagramId;
	}

	/// Returns absolute or relative path to the file opened in text editor. The resulting string will be empty
	/// if current tab is not a text editor one. But it can still be empty if text editor is just editing some text
	/// without binding to a concrete file.
	QString pathToOpenedTextFile() const
	{
		return mPathToOpenedTextFile;
	}

	/// Returns a reference to a widget that shown in opened tab. The result can be nullptr if,
	/// for example, no tab is opened.
	QWidget *tabWidget() const
	{
		return mOpenedWidget;
	}

private:
	const TabType mType;
	const Id mRootDiagramId;
	const QString mPathToOpenedTextFile;

	/// Does not take ownership.
	QWidget * const mOpenedWidget;
};

}
