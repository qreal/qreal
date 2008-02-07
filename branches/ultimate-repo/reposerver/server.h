#ifndef __Q_REAL_REPO_SERVER_H__
#define __Q_REAL_REPO_SERVER_H__

#include <QTcpServer>
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

class QRealRepoServer : public QTcpServer
{
	Q_OBJECT

public:
	QRealRepoServer(QObject *parent = 0);
	~QRealRepoServer();
	
public slots:
	void killall();

protected:
	 void incomingConnection(int socketDescriptor);
	 
private:
	int count;

	RepoTypesInfo *typesInfo;
	Root *root;
};

#endif // __Q_REAL_REPO_SERVER_H__
