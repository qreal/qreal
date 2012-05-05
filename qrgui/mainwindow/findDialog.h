#pragma once

#include <QtGui>
#include <QDialog>
#include <QtCore/QSignalMapper>
#include "../models/logicalModelAssistApi.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog
{
	Q_OBJECT

	public:

		FindDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent = NULL);
		void initIds(QMap<QString, QString> foundData = QMap<QString, QString>());
		~FindDialog();

	signals:

		void findModelByName(const QStringList &searchData);
		void replaceStarted();
		void chosenElement(qReal::Id const &id);

	private slots:

		void replaceClicked();
		void findClicked();
		void enableFindButton(const QString &text);
		void itemChosen(QListWidgetItem *item);

	private:

		QLabel *mLabel;
		QLineEdit *mLineEdit;
		QPushButton *mFindButton;
		QPushButton *mReplaceButton;
		QList<QCheckBox*> mCheckBoxes;
		QListWidget *mListWidget;
		qrRepo::LogicalRepoApi const &mCommonApi;
};
