#pragma once

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

namespace qReal {

	class ConnectionDialog : public QDialog
	{
		Q_OBJECT;

	public:
		ConnectionDialog(QString const &address, int port);
		~ConnectionDialog();

	signals:
		void dataAccepted(const QString &addr, const int port);

	public slots:
		void acceptData();

	private:
		QLabel *mAddrLabel;
		QLabel *mPortLabel;
		QLineEdit *mAddrEdit;
		QLineEdit *mPortEdit;

		QPushButton *mOkButton;
		QPushButton *mCancelButton;

		QHBoxLayout *mLh1;
		QHBoxLayout *mLh2;
		QHBoxLayout *mLh3;
		QVBoxLayout *mLv1;
	};

}
