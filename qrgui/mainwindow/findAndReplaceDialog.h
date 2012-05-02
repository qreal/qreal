#pragma once

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindAndReplaceDialog : public QDialog
{
	Q_OBJECT

	public:

		FindAndReplaceDialog(QWidget *parent = NULL);
		~FindAndReplaceDialog();

	signals:

		void replaceClicked(QStringList const &searchData);

	private slots:

		void replaceHandler();
		void enableReplaceButton(const QString &);

	private:

		QLabel *mFindLabel;
		QLineEdit *mFindLineEdit;
		QLabel *mReplaceLabel;
		QLineEdit *mReplaceLineEdit;
		QPushButton *mButton;
		QList<QCheckBox*> mCheckBoxes;
};
