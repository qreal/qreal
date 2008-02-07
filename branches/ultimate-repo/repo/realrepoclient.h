#ifndef __Q_REAL_REPO_CLIENT_H__
#define __Q_REAL_REPO_CLIENT_H__

#include <QTcpSocket>
#include "../common/defs.h"
#include "../common/classes.h"

class RealRepoClient : public QObject
{
	Q_OBJECT;

public:
	RealRepoClient( QObject *parent = 0);
	~RealRepoClient();

	int setName( int type, int id, QString name );
	void setPosition( int type, int id, int parent, int x, int y);
	int setConfiguration( int type, int id, QString conf);
	QString getConfiguration( int type, int id );
	int setPropValue( int type, int id, QString name, QString value);
	QString getPropValue( int type, int id, QString name );

	int createEntity(int type, QString name);
	int createEntity(int type, QString name, int parent);

	int getTypesCount();
	TypeInfo getTypeInfo( int );

	QString getObjectsByType( int type );
	QString getObjectData( int id );
	QString getEntireObject( int type, int id );

	QString getLinksByObject( int type, int id );
	QString getObjectsByLink( int type, int id );

	QString getChildren( int type, int id );

	QString getPosition(int type, int id );
	int setPosition(int type, int id, int x, int y );

	int getLastError();

public slots:
	void displayError( QAbstractSocket::SocketError );

private:
	QString sendData( QString data ); 

	QTcpSocket *socket;
	quint16 blockSize;
	int error;
};

#endif // __Q_REAL_REPO_CLIENT_H__
