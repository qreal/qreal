#pragma once

#include <QObject>
#include <QtGui>
#include "mainWindowInterpretersInterface.h"
#include "../models/logicalModelAssistApi.h"
#include "../../qrgui/dialogs/findReplaceDialog.h"

class MainWindowInterpretersInterface;

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
	void handleFindDialog(QStringList const &searchData);

	/// handler for refs dialog reference chosen
	/// @param id - id of element that was chosen to show and highlight
	void handleRefsDialog(qReal::Id const &id);

	/// handler for find & replace dialog 'button replace' pressed
	/// @param searchData - data was input to find & replace
	void handleReplaceDialog(QStringList &searchData);

private:
	/// Finds items by input name and search mode
	/// @param key - name
	/// @param currentMode - mode
	qReal::IdList foundByMode(QString key, QString currentMode, bool sensitivity);

	/// Finds items by input name and search modes - searchData
	/// @param searchData - name and search modes
	QMap<QString, QString> findItems(QStringList const &searchData);

	qrRepo::RepoControlInterface &mControlApi;

	qrRepo::LogicalRepoApi &mLogicalApi;

	/// mFindDialog - Dialog for searching elements.
	FindReplaceDialog *mFindReplaceDialog;

	qReal::gui::MainWindowInterpretersInterface *mMainWindow;
};
