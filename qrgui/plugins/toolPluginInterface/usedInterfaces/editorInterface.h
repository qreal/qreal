/* Copyright 2016 Dmitry Mordvinov
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

#include <QtWidgets/QAction>

namespace qReal {

/// Interface for all QReal editors. Editor actions on toolbar and in main menu such as zoom in-out and undo-redo
/// will be automaticly attached and detached if this editor is registered via MainWindowDockInterface::registerEditor.
class EditorInterface
{
public:
	EditorInterface()
		: mZoomInAction(nullptr)
		, mZoomOutAction(nullptr)
		, mUndoAction(nullptr)
		, mRedoAction(nullptr)
		, mCopyAction(nullptr)
		, mPasteAction(nullptr)
		, mCutAction(nullptr)
		, mFindAction(nullptr)
		, mReplaceByAction(nullptr)
		, mFocusAction(nullptr)
	{
	}

	virtual ~EditorInterface()
	{
	}

	/// Should be reimplemented to return unique string identifier used by controller undo stack.
	virtual QString editorId() const = 0;

	/// Can be reimplemented to enable zooming actions on toolbar and in main menu  when this editor is active.
	virtual bool supportsZooming() const
	{
		return false;
	}

	/// Can be reimplemented to enable copying actions on toolbar and in main menu  when this editor is active.
	virtual bool supportsCopying() const
	{
		return false;
	}

	/// Can be reimplemented to enable pasting actions on toolbar and in main menu when this editor is active.
	virtual bool supportsPasting() const
	{
		return false;
	}

	/// Can be reimplemented to enable cutting actions in main menu when this editor is active.
	virtual bool supportsCutting() const
	{
		return false;
	}

	/// Can be reimplemented to enable search actions in main menu when this editor is active.
	virtual bool supportsSearching() const
	{
		return false;
	}

	/// Can be reimplemented to enable replace-by actions in main menu when this editor is active.
	virtual bool supportsReplacingBy() const
	{
		return false;
	}

	/// Can be reimplemented to zoom the picture shown in editor in. Implementation has sense only when
	/// supportsZooming() returns true.
	virtual void zoomIn() {}

	/// Can be reimplemented to zoom the picture shown in editor out. Implementation has sense only when
	/// supportsZooming() returns true.
	virtual void zoomOut() {}

	/// Can be to copy current editor items into the clipoard. Implementation has sense only when
	/// supportsCopyPaste() returns true.
	virtual void copy() {}

	/// Can be to copy current editor items into the clipoard. Implementation has sense only when
	/// supportsCopyPaste() returns true.
	virtual void paste() {}

	/// Can be to copy current editor items into the clipoard. Implementation has sense only when
	/// supportsCopyPaste() returns true.
	virtual void cut() {}

	/// Can be to find items in editors space. Implementation has sense only when
	/// supportsFinding() returns true.
	virtual void find() {}

	/// Can be to reblace items by smth in editors space. Implementation has sense only when
	/// supportsReplacingBy() returns true.
	virtual void replaceBy() {}

	/// Can be reimplemented to force focus capturing when system requires that. By default simply triggers focusAction.
	virtual void forceFocus()
	{
		onFocusIn();
	}

	/// Configures editor with the given set of editor actions.
	virtual void configure(QAction &zoomIn, QAction &zoomOut, QAction &undo, QAction &redo
		, QAction &copy, QAction &paste, QAction &cut, QAction &find, QAction &replaceBy)
	{
		mZoomInAction = &zoomIn;
		mZoomOutAction = &zoomOut;
		mUndoAction = &undo;
		mRedoAction = &redo;
		mCopyAction = &copy;
		mPasteAction = &paste;
		mCutAction = &cut;
		mFindAction = &find;
		mReplaceByAction = &replaceBy;
	}

	/// Returns action that is triggered when editor is focused. Conceptually this is a simple signal, but
	/// signals are impossible here becuse inheritors will have ambiguous QObject base.
	/// Implementors should call onFocusIn() to trigger focus action.
	const QAction &focusAction() const
	{
		return mFocusAction;
	}

protected:
	void onFocusIn() {
		mFocusAction.trigger();
	}

	QList<QAction *> editorActions() const
	{
		return { mZoomInAction, mZoomOutAction, mUndoAction, mRedoAction
				, mCopyAction, mPasteAction, mCutAction, mFindAction, mReplaceByAction };
	}

	QAction *mZoomInAction;  // Does not have ownership.
	QAction *mZoomOutAction;  // Does not have ownership.
	QAction *mUndoAction;  // Does not have ownership.
	QAction *mRedoAction;  // Does not have ownership.
	QAction *mCopyAction;  // Does not have ownership.
	QAction *mPasteAction;  // Does not have ownership.
	QAction *mCutAction;  // Does not have ownership.
	QAction *mFindAction;  // Does not have ownership.
	QAction *mReplaceByAction;  // Does not have ownership.

private:
	QAction mFocusAction;
};

}
