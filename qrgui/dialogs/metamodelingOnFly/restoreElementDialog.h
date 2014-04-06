#pragma once

#include <QtWidgets/QDialog>
#include <QtGui/QStandardItem>

#include "pluginManager/editorManagerInterface.h"

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
	/// @param mainWindow Reference to QReal main window.
	/// @param interpreterEditorManager Editor manager.
	/// @param elementsWithTheSameNameList IdList of elements with the same name.
	explicit RestoreElementDialog(QWidget *parent, MainWindow &mainWindow, EditorManagerInterface const &interpreterEditorManager
			, IdList const &elementsWithTheSameNameList);

	/// Destructor.
	~RestoreElementDialog();

signals:
	void createNewChosen();
	void restoreChosen(int result);

private slots:
	void restoreButtonClicked();
	void createButtonClicked();

private:
	Ui::RestoreElementDialog *mUi;
	MainWindow &mMainWindow;
	EditorManagerInterface const &mInterpreterEditorManager;
	IdList mElementsWithTheSameNameList;

	void fillSameNameElementsTV();
	QList<QStandardItem *> prepareRow(QString const &first, QString const &second, QString const &third);
};
}
