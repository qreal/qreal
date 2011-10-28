#pragma once

#include <QDialog>
#include <QModelIndex>

#include "preferencesPages/page.h"

#include "../../qrkernel/settingsManager.h"

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT

public:
	enum PageIndexes {
		editor = 0,
		behaviour = 1,
		miscellanious = 2,
		compiler = 3,
		debugger = 4,
//		features = 5
	};

	PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

	void init(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction);

	void registerPage(QString const &pageName, PreferencesPage * const page);
	void switchCurrentTab(QString const &tabName);

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void iconsetChanged();
	void settingsApplied();
	void fontChanged();

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
	PreferencesPage *mFeaturesPage;

	QMap<QString, PreferencesPage *> mCustomPages;

	void hidePages();
};
