#pragma once

#include <QDialog>

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	PreferencesDialog(QAction * const showGridAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent = 0);
	~PreferencesDialog();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesDialog *ui;
	QAction * const mShowGridAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;

	void applyChanges();
	void initPreferences();
	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);
private slots:
	void on_cancelButton_clicked();
	void on_applyButton_clicked();
	void on_okButton_clicked();
	void systemChoosingButtonClicked();
};
