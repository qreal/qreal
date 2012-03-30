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

		RefWindowDialog(QWidget *parent = NULL);
		~RefWindowDialog();
		void init(qReal::IdList ids, QStringList names);

	signals:

		void chosenElement(qReal::Id const &id);

	private slots:

		void itemChosen(QListWidgetItem *item);

	private:

		QListWidget *mListWidget;
		QHBoxLayout *mMainLayout;
};
