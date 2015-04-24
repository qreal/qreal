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

#include "findManager.h"

FindManager::FindManager(qrRepo::RepoControlInterface &controlApi
		, qrRepo::LogicalRepoApi &logicalApi
		, qReal::gui::MainWindowInterpretersInterface *mainWindow
		, FindReplaceDialog *findReplaceDialog
		, QObject *parent)
	: QObject(parent)
	, mControlApi(controlApi)
	, mLogicalApi(logicalApi)
	, mFindReplaceDialog(findReplaceDialog)
	, mMainWindow(mainWindow)
{
}

void FindManager::handleRefsDialog(const qReal::Id &id)
{
	mMainWindow->selectItemOrDiagram(id);
}

qReal::IdList FindManager::foundByMode(QString key, QString currentMode, bool sensitivity
		, bool regExpression)
{
	// TODO: replace mode string with modifiers
	if (currentMode == tr("by name")) {
		return mControlApi.findElementsByName(key, sensitivity, regExpression);
	} else if (currentMode == tr("by type")) {
		return mLogicalApi.elementsByType(key, sensitivity, regExpression);
	} else if (currentMode == tr("by property")) {
		return mControlApi.elementsByProperty(key, sensitivity, regExpression);
	} else if (currentMode == tr("by property content")) {
		return mControlApi.elementsByPropertyContent(key, sensitivity, regExpression);
	} else {
		return qReal::IdList();
	}
}

QMap<QString, QString> FindManager::findItems(const QStringList &searchData)
{
	QMap<QString, QString> found;
	bool sensitivity = searchData.contains(tr("case sensitivity"));
	bool regExpression = searchData.contains(tr("by regular expression"));

	for(int i = 1; i < searchData.length(); i++) {
		if (searchData[i] != tr("case sensitivity") && searchData[i] != tr("by regular expression")) {
			qReal::IdList byMode = foundByMode(searchData.first(), searchData[i], sensitivity
					, regExpression);
			foreach (qReal::Id currentId, byMode) {
				if (found.contains(currentId.toString())) {
					found[currentId.toString()] += tr(", ") + searchData[i];
					continue;
				}
				found.insert(currentId.toString(), tr("   :: ") + searchData[i]);
			}
		}
	}
	return found;
}

void FindManager::handleFindDialog(const QStringList &searchData)
{
	mFindReplaceDialog->initIds(findItems(searchData));
}

void FindManager::handleReplaceDialog(QStringList &searchData)
{
	if (searchData.contains(tr("by name"))) {
		qReal::IdList toRename = foundByMode(searchData.first(), tr("by name")
				, searchData.contains(tr("case sensitivity"))
				, searchData.contains(tr("by regular expression")));
		foreach (qReal::Id currentId, toRename) {
			mLogicalApi.setName(currentId, searchData[1]);
		}
	}

	if (searchData.contains(tr("by property content"))) {
		qReal::IdList toRename = foundByMode(searchData.first(), tr("by property content")
				, searchData.contains(tr("case sensitivity"))
				, searchData.contains(tr("by regular expression")));
		mLogicalApi.replaceProperties(toRename, searchData[0], searchData[1]);
	}
}
