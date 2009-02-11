#include "connectiondialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QSettings>

using namespace qReal;

ConnectionDialog::ConnectionDialog(QString const &address, int port)
{
	mAddrEdit = new QLineEdit(address);
	mPortEdit = new QLineEdit(QString("%1").arg(port));

	mAddrLabel = new QLabel("Address:");
	mPortLabel = new QLabel("Port:");

	mOkButton = new QPushButton("Ok");
	mCancelButton = new QPushButton("Cancel");

	connect(mOkButton, SIGNAL(clicked()), SLOT(acceptData()));
	connect(this, SIGNAL(dataAccepted(const QString&, const int)), SLOT(accept()));
	connect(mCancelButton, SIGNAL(clicked()), SLOT(reject()));

	mLh1 = new QHBoxLayout;
	mLh1->addWidget(mAddrLabel);
	mLh1->addWidget(mAddrEdit);

	mLh2 = new QHBoxLayout;
	mLh2->addWidget(mPortLabel);
	mLh2->addWidget(mPortEdit);

	mLh3 = new QHBoxLayout;
	mLh3->addWidget(mOkButton);
	mLh3->addWidget(mCancelButton);

	mLv1 = new QVBoxLayout();
	mLv1->addLayout(mLh1);
	mLv1->addLayout(mLh2);
	mLv1->addLayout(mLh3);

	setLayout(mLv1);
}

ConnectionDialog::~ConnectionDialog()
{
	delete mLv1;
}

void ConnectionDialog::acceptData()
{
	int port = mPortEdit->text().toInt();
	if( port <= 1024 || port >= 65535 ){
		QMessageBox::warning(0, "error", "invalid port number");
		port = 6666;
	}
	emit dataAccepted(mAddrEdit->text(), port);
}

