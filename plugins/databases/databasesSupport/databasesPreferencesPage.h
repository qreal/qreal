/* Copyright 2014-2015 Anastasia Semenova
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

#include <QtCore/QTranslator>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <qrutils/qRealFileDialog.h>
#include <qrkernel/settingsManager.h>

#include <qrgui/preferencesDialog/preferencesPage.h>
#include <../../../qrkernel/settingsManager.h>

#include "ui_databasesPreferencesPage.h"

namespace Ui {
	class DatabasesPreferencesPage;
}

namespace qReal {
namespace databasesSupport {

/// Class for plugin preferences
class DatabasesPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT
public:
	explicit DatabasesPreferencesPage(QWidget *parent = 0);
	~DatabasesPreferencesPage();

	void save();
	virtual void restoreSettings();
public slots:
	void dbmsChanging(QString const &dbmsName);
	void changeReverseEngineerFilePath(QString const &path);
	void browseFileForReverseEngineer();
signals:
	void dbmsChanged(QString const &dbmsName);
private:
	Ui::DatabasesPreferencesPage *mUi;
};

}
}
