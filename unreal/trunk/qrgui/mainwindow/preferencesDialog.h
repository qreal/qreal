#pragma once

#include <QDialog>

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::PreferencesDialog *ui;
	void applyChanges();
	void initPreferences();
private slots:
	void on_cancelButton_clicked();
 void on_applyButton_clicked();
	void on_okButton_clicked();
};
