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
		void init(QStringList names);

	signals:

		void chosenElement(const QString &str);

	private slots:

		void refClicked(QListWidgetItem *ref);

	private:

		QListWidget *mReferences;
		QHBoxLayout *mMainLayout;
};
