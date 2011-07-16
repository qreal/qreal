#pragma once

#include <QDialog>
#include <QModelIndex>

#include "preferencesPages/page.h"

#include "../kernel/settingsManager.h"

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT

public:
	PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent = 0);
	~PreferencesDialog();

	enum PageIndexes {
		editor = 0,
		behaviour = 1,
		miscellanious = 2,
		compiler = 3,
		debugger = 4,
		robotSettings = 5
	};

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void iconsetChanged();
	void settingsApplied();

private slots:
	void cancel();
	void applyChanges();
	void saveAndClose();

	void chooseTab(const QModelIndex &);

private:
	Ui::PreferencesDialog *ui;

	PreferencesPage *mBehaviourPage;
	PreferencesPage *mCompilerPage;
	PreferencesPage *mDebuggerPage;
	PreferencesPage *mEditorPage;
	PreferencesPage *mMiscellaniousPage;
	PreferencesPage *mRobotSettingsPage;
};
