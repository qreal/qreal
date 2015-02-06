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
