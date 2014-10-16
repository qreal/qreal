#pragma once

#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>
#include <QtCore/QSignalMapper>

#include <qrutils/qRealDialog.h>
#include <qrgui/models/logicalModelAssistApi.h>

#include "qrgui/dialogs/dialogsDeclSpec.h"

namespace Ui {
	class FindReplaceDialog;
}

/// Class that manages ui of find & replace menu.
class QRGUI_DIALOGS_EXPORT FindReplaceDialog : public utils::QRealDialog
{
	Q_OBJECT

public:
	/// constructor
	explicit FindReplaceDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent = NULL);

	/// Loads found data to.
	/// @param foundData - found data.
	void initIds(QMap<QString, QString> foundData = QMap<QString, QString>());

	/// Stets dialog state as starter.
	void stateClear();
	~FindReplaceDialog();

signals:
	/// Signal of find button clicked.
	/// @param searchData - data for search.
	void findModelByName(const QStringList &searchData);

	/// Signal of found item chosen.
	/// @param id - id of chosen element.
	void chosenElement(qReal::Id const &id);

	/// Signal of replace button clicked.
	/// @param searchData - data for search & replace.
	void replaceClicked(QStringList &searchData);

private slots:
	/// Collects necessary information and emits signal of replace clicked.
	void replaceHandler();

	/// Collects necessary information and emits signal of find clicked.
	void findClicked();

	/// Collects necessary information and emits signal of item chosen.
	/// @param item - item chosen
	void itemChosen(QListWidgetItem *item);

	/// Enables replace button if find mode is valid for replace.
	void tryEnableReplaceButton();

private:
	/// Checkboxes with find modes.
	QList<QCheckBox*> mCheckBoxes;

	/// Repository's interface.
	qrRepo::LogicalRepoApi const &mCommonApi;

	/// Dialods ui.
	Ui::FindReplaceDialog *mUi;
};
