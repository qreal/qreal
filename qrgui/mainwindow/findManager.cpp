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

void FindManager::handleRefsDialog(qReal::Id const &id)
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

QMap<QString, QString> FindManager::findItems(QStringList const &searchData)
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

void FindManager::handleFindDialog(QStringList const &searchData)
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
