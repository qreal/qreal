/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

	void registerPage(const QString &pageName, PreferencesPage * const page);
	void switchCurrentPage(const QString &pageName);

	/// Returns a list of registered preferences pages.
	QList<PreferencesPage *> pages() const;

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
