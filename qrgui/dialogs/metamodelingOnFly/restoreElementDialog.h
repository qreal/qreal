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

#include <QtWidgets/QDialog>
#include <QtGui/QStandardItem>

#include "plugins/pluginManager/editorManagerInterface.h"

namespace Ui {
class RestoreElementDialog;
}

namespace qReal {

/// Window where you can restore the element with the same name and type, which was renamed or removed.
class RestoreElementDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parent Parent widget, who is responsible for deletion of this dialog.
	/// @param interpreterEditorManager Editor manager.
	/// @param elementsWithTheSameNameList IdList of elements with the same name.
	RestoreElementDialog(QWidget *parent
			, const EditorManagerInterface &interpreterEditorManager
			, const IdList &elementsWithTheSameNameList);

	/// Destructor.
	~RestoreElementDialog();

signals:
	/// Emitted when element was restored and everything must be reloaded.
	void jobDone();
	void createNewChosen();
	void restoreChosen(int result);

private slots:
	void restoreButtonClicked();
	void createButtonClicked();

private:
	Ui::RestoreElementDialog *mUi; // Has ownership
	const EditorManagerInterface &mInterpreterEditorManager;
	IdList mElementsWithTheSameNameList;

	void fillSameNameElementsTreeView();
	// Object has ownership over list elements.
	QList<QStandardItem *> prepareRow(const QString &first, const QString &second, const QString &third);
};
}
