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

signals:
	void gridChanged();
	void iconsetChanged();

private slots:
	void cancel();
	void applyChanges();
	void saveAndClose();
	void systemChoosingButtonClicked();
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);

	void browseImagesPath();

private:
	void initPreferences();
	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);

	Ui::PreferencesDialog *ui;
	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;
	int mWithGrid;
	int mIndexGrid;

	QString mLastIconsetPath;

};
