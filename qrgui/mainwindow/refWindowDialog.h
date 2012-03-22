#ifndef FINDBYNAMEREFWINDOW_H
#define FINDBYNAMEREFWINDOW_H

#include <QtCore/QSignalMapper>
#include <QDialog>
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

		void refClicked(const QString &refName);

	private:

		QList<QPushButton*> *mButtons;
		QSignalMapper *mRefButtonsMapper;
};

#endif // FINDBYNAMEREFWINDOW_H
