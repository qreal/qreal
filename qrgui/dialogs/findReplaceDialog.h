#pragma once

#include <QWidget>
#include <QtGui>
#include <QDialog>
#include <QtCore/QSignalMapper>
#include "../models/logicalModelAssistApi.h"

namespace Ui {
	class FindReplaceDialog;
}

class FindReplaceDialog : public QDialog
{

	Q_OBJECT

public:

	explicit FindReplaceDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent = NULL);
	void initIds(QMap<QString, QString> foundData = QMap<QString, QString>());
	void setMode(bool visibility);
	~FindReplaceDialog();

signals:

	void findModelByName(const QStringList &searchData);
	void chosenElement(qReal::Id const &id);
	void replaceClicked(QStringList &searchData);

private slots:

	void replaceHandler();
	void findClicked();
	void itemChosen(QListWidgetItem *item);

private:

	QList<QCheckBox*> mCheckBoxes;
	qrRepo::LogicalRepoApi const &mCommonApi;
	Ui::FindReplaceDialog *mUi;

};
