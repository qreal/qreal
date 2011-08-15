#pragma once

#include <QDialog>

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent = 0);
	~PreferencesDialog();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesDialog *ui;
	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;
	int mWithGrid;
	int mIndexGrid;

	void applyChanges();
	void initPreferences();
	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);
private slots:
	void on_diffColorsButton_clicked();
	void on_browseDiffCheckoutPathtPushButton_clicked();
	void on_browseSvnClientPushButton_clicked();
	void on_workDirPushButton_clicked();
	void on_builderPathButton_clicked();
	void on_browseDebPathButton_clicked();
	void on_cancelButton_clicked();
	void on_applyButton_clicked();
	void on_okButton_clicked();
	void systemChoosingButtonClicked();
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);
signals:
	void gridChanged();
};
