#pragma once

#include <QtCore/QModelIndex>
#include <QtWidgets/QDialog>
#include <QtCore/QSettings>

#include <qrkernel/settingsManager.h>
#include <qrutils/qRealDialog.h>

#include "dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public utils::QRealDialog
{
	Q_OBJECT

public:

	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

	void init(QAction * const showGridAction, QAction * const showAlignmentAction
		, QAction * const activateGridAction, QAction * const activateAlignmentAction);
	void updatePluginDependendSettings();

	void registerPage(QString const &pageName, PreferencesPage * const page);
	void switchCurrentTab(QString const &tabName);

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);

signals:
	void gridChanged();
	void iconsetChanged();
	void toolbarSizeChanged(int size);
	void settingsApplied();
	void fontChanged();
	void paletteRepresentationChanged();
	void usabilityTestingModeChanged(bool on);

public slots:
	void changePaletteParameters();

private slots:
	void cancel();
	void applyChanges();
	void restoreSettings();
	void saveAndClose();
	void chooseTab(const QModelIndex &);
	void exportSettings();
	void importSettings();

private:
	Ui::PreferencesDialog *mUi;
	QMap<QString, PreferencesPage *> mCustomPages;
	QMap<QString, int> mPagesIndexes;
};
