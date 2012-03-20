#ifndef FINDDIALOG_H
#define FINDDIALOG_H
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

	signals:

		void findModelByName(const QString &str);

	private slots:

		void findClicked();
		void enableFindButton(const QString &text);

	private:

		QLabel *mLabel;
		QLineEdit *mLineEdit;
		QPushButton *mFindButton;
};

#endif // FINDDIALOG_H
