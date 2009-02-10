#include <QDebug>
#include <QMessageBox>
#include "connectiondialog.h"

ConnectionDialog::ConnectionDialog()
{
	addrEdit = new QLineEdit("127.0.0.1");
	portEdit = new QLineEdit("6666");
	
	addrLabel = new QLabel("Address:");
	portLabel = new QLabel("Port:");

	okButton = new QPushButton("Ok");
	cancelButton = new QPushButton("Cancel");

	connect(okButton, SIGNAL(clicked()), this, SLOT(acceptData()));
	connect(this, SIGNAL(dataAccepted(const QString&, const int)), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	lh1 = new QHBoxLayout;
	lh1->addWidget(addrLabel);
	lh1->addWidget(addrEdit);
	
	lh2 = new QHBoxLayout;
	lh2->addWidget(portLabel);
	lh2->addWidget(portEdit);
	
	lh3 = new QHBoxLayout;
	lh3->addWidget(okButton);
	lh3->addWidget(cancelButton);

	lv1 = new QVBoxLayout();
	lv1->addLayout(lh1);
	lv1->addLayout(lh2);
	lv1->addLayout(lh3);

	this->setLayout(lv1);
}

ConnectionDialog::~ConnectionDialog()
{
	delete lv1;
}

void ConnectionDialog::acceptData()
{
	int port = portEdit->text().toInt();
	if( port <= 1024 || port >= 65535 ){
		QMessageBox::warning(0, "error", "invalid port number");
		port = 6666;
	}			
	emit dataAccepted(addrEdit->text(), port);
}

