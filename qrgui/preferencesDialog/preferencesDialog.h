#pragma once

#include <QtCore/QModelIndex>
#include <QtWidgets/QDialog>
#include <QtCore/QSettings>

#include <qrkernel/settingsManager.h>
#include <qrutils/qRealDialog.h>

#include "preferencesPage.h"
#include "preferencesDialogDeclSpec.h"

namespace Ui {
	class PreferencesDialog;
}

namespace qReal {
namespace gui {

class QRGUI_PREFERENCES_DIALOG_EXPORT PreferencesDialog : public utils::QRealDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

	void init();
//	void updatePluginDependendSettings();

	void registerPage(QString const &pageName, PreferencesPage * const page);
	void switchCurrentPage(QString const &pageName);

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);

signals:
	/// Emitted each time when user presses "OK" or "Apply" button.
	void settingsApplied();

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

}
}
