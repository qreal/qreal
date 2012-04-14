#pragma once

#include <QDialog>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog
{
	Q_OBJECT

	public:

		FindDialog(QWidget *parent = NULL);
		~FindDialog();

	signals:

		void findModelByName(const QStringList &searchData);

	private slots:

		void findClicked();
		void enableFindButton(const QString &text);

	private:

		QLabel *mLabel;
		QLineEdit *mLineEdit;
		QPushButton *mFindButton;
		QList<QCheckBox*> mCheckBoxes;
};
