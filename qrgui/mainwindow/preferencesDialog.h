#pragma once

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>

#include <../../kernel/exception/settingsManager.h>

namespace Ui {
	class PreferencesDialog;
	class BehaviourForm;
	class CompilerForm;
	class DebuggerForm;
	class EditorForm;
	class MiscellaniousForm;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent = 0);
	~PreferencesDialog();

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void settingsApplied();

private slots:
	void cancel();
	void applyChanges();
	void saveAndClose();
	void changeSystem();
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);
	void changeWorkingDirectory();
	void changeBuilderPath();
	void changeDebuggerPath();
	void chooseTab(const QModelIndex &);

private:
	void initTabs();
	void initPreferences();
	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);

	enum PageIndexes {
		editor = 0,
		behaviour = 1,
		miscellanious = 2,
		compiler = 3,
		debugger = 4
	};

	Ui::PreferencesDialog *ui;
	Ui::BehaviourForm *mBehaviourUi;
	Ui::CompilerForm *mCompilerUi;
	Ui::DebuggerForm *mDebuggerUi;
	Ui::EditorForm *mEditorUi;
	Ui::MiscellaniousForm *mMiscellaniousUi;

	QWidget *mBehaviourPage;
	QWidget *mCompilerPage;
	QWidget *mDebuggerPage;
	QWidget *mEditorPage;
	QWidget *mMiscellaniousPage;

	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;
	int mWithGrid;
	int mIndexGrid;
};
