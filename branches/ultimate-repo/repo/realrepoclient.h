#ifndef __Q_REAL_REPO_CLIENT_H__
#define __Q_REAL_REPO_CLIENT_H__

#include <QTcpSocket>
#include "../common/defs.h"
#include "../common/classes.h"
#include "../common/realrepoapiclasses.h"

using namespace QRealTypes; 

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
	QIntList getAllTypes();  
	TypeInfo getTypeInfo( int );

	QString getObjectsByType( int type );
	QIntList getObjectsListByType( int type );
	
	QString getObjectData( int id );
	QString getEntireObject( int id );
	RealObject getObjectById( int id );
	RealLink getLinkById( int id );

	QString getLinksByObject( int type, int id );
	QString getObjectsByLink( int type, int id );

	QString getChildren( int type, int id );

	QString getPosition(int type, int id );
	int setPosition(int type, int id, int x, int y );

	int getLastError();

	int state(){ return socket->state(); }

	int isValid() { return socket->isValid(); }

	QIntList getTypesByMetaType( const MetaType );
	
	RealType getTypeById( const int id ); 

	// TODO: implement these calls
	/* 
	int createType(const QString name); // returns id of created type
	void deleteType(const int id);
	*/

public slots:
	void displayError( QAbstractSocket::SocketError );

private:
	QString sendData( QString data ); 

	QTcpSocket *socket;
	quint16 blockSize;
	int m_error;
};

#endif // __Q_REAL_REPO_CLIENT_H__
