#pragma once

#include <QDialog>
#include <QModelIndex>

#include "preferencesPages/preferencesPage.h"
#include "../../qrkernel/settingsManager.h"

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT

public:
	explicit  PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

	void init(QAction * const showGridAction, QAction * const showAlignmentAction
		, QAction * const activateGridAction, QAction * const activateAlignmentAction);

	void registerPage(QString const &pageName, PreferencesPage * const page);
	void switchCurrentTab(QString const &tabName);

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void iconsetChanged();
	void settingsApplied();
	void fontChanged();
	void paletteRepresentationChanged();

public slots:
	void changePaletteParameters();

private slots:
	void cancel();
	void applyChanges();
	void saveAndClose();
	void chooseTab(const QModelIndex &);

private:
	Ui::PreferencesDialog *mUi;
	QMap<QString, PreferencesPage *> mCustomPages;
};
