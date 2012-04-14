#pragma once

#include <QtCore/QSignalMapper>
#include <QDialog>
#include <QtGui>
#include <../mainwindow/mainWindow.h>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class RefWindowDialog : public QDialog
{
	Q_OBJECT

	public:

		RefWindowDialog(qrRepo::LogicalRepoApi const &logicalRepoApi, QWidget *parent = NULL);
		~RefWindowDialog();
		void initIds(QMap<QString, QString> foundData);

	signals:

		void chosenElement(qReal::Id const &id);

	private slots:

		void itemChosen(QListWidgetItem *item);

	private:

		QListWidget *mListWidget;
		QHBoxLayout *mMainLayout;
		qrRepo::LogicalRepoApi const &mCommonApi;
};
