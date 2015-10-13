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

#include <QtCore/QObject>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/models/logicalModelAssistApi.h>
#include <qrgui/dialogs/findReplaceDialog.h>

/// Class that manages operations of find & replace.
class FindManager : public QObject
{
	Q_OBJECT
public:
	explicit FindManager(qrRepo::RepoControlInterface &controlApi
			, qrRepo::LogicalRepoApi &logicalApi
			, qReal::gui::MainWindowInterpretersInterface *mainWindow
			, FindReplaceDialog *findReplaceDialog
			, QObject *paresnt = 0);

public slots:
	/// handler for find dialog 'button find' pressed
	/// @param searchData - data was input to find
	void handleFindDialog(const QStringList &searchData);

	/// handler for refs dialog reference chosen
	/// @param id - id of element that was chosen to show and highlight
	void handleRefsDialog(const qReal::Id &id);

	/// handler for find & replace dialog 'button replace' pressed
	/// @param searchData - data was input to find & replace
	void handleReplaceDialog(QStringList &searchData);

private:
	/// Finds items by input name and search mode
	/// @param key - name
	/// @param currentMode - mode
	qReal::IdList foundByMode(QString key, QString currentMode, bool sensitivity, bool regExpression);

	/// Finds items by input name and search modes - searchData
	/// @param searchData - name and search modes
	QMap<QString, QString> findItems(const QStringList &searchData);

	qrRepo::RepoControlInterface &mControlApi;

	qrRepo::LogicalRepoApi &mLogicalApi;

	/// mFindDialog - Dialog for searching elements.
	FindReplaceDialog *mFindReplaceDialog;

	qReal::gui::MainWindowInterpretersInterface *mMainWindow;
};
