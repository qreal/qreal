#ifndef NEWCLIENT_H
#define NEWCLIENT_H

#include <QtGui/QMainWindow>

class NewClient : public QMainWindow
{
	Q_OBJECT

public:
	NewClient(QWidget *parent = 0);
	~NewClient();
};

#endif // NEWCLIENT_H
