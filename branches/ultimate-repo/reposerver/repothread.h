#ifndef __Q_REAL_REPO_SERVER_THREAD_H__
#define __Q_REAL_REPO_SERVER_THREAD_H__

#include <QThread>
#include <QTcpSocket>

#include "../common/defs.h"
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

class QRealRepoServerThread : public QThread
{
	Q_OBJECT

public:
	QRealRepoServerThread(int socketDescriptor,  QObject *parent, Root *root, RepoTypesInfo *, int);
	
	void run();

public slots:

signals:
	void error(QTcpSocket::SocketError socketError);
			 
private:
	void handleCommand( QString cmd, QTcpSocket *socket );
	int getId();

	int socketDescriptor;
	bool canRead;
	Root *root;
	RepoTypesInfo *typesInfo;
	int counter;
};	


#endif // __REAL_REPO_SERVER_THREAD_H__
