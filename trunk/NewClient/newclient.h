#ifndef NEWCLIENT_H
#define NEWCLIENT_H

#include <QtGui/QWidget>

class NewClient : public QWidget
{
	Q_OBJECT

public:
	NewClient(QWidget *parent = 0);
	~NewClient();
};

#endif // NEWCLIENT_H
