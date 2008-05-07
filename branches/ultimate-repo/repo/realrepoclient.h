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

	int setName( int id, QString name );
	
	void setPosition( int id, int parent, int x, int y);
	
	void setDescription( int id, QString desc );
	QString getDescription( int id );
	
	int setConfiguration( int id, QString conf);
	QString getConfiguration( int id );
	
	int setPropValue( int id, QString name, QString value);
	QString getPropValue(  int id, QString name );

	int createEntity( int type, QString name );
	int createEntityWithParent(int type, QString name, int parent);
	int createLink(QString name);

	void addLink( int id, int link_id, int direction );

	int getTypesCount();
	QIntList getAllTypes();  
	TypeInfo getTypeInfo( int );

	QString getObjectsByType( int type );
	QIntList getObjectsListByType( int type );
	
	QString getObjectData( int id );
	QString getEntireObject( int id );
	RealObject getObjectById( int id );
	RealLink getLinkById( int id );

	QString getLinksByObject( int id, int direction );
	QString getObjectsByLink( int id );

	QString getChildren( int id );

	QString getPosition(int id );
	int setPosition( int id, int x, int y );

	int getLastError();

	int state(){ return socket->state(); }

	int isValid() { return socket->isValid(); }

	QIntList getTypesByMetaType( const MetaType );
	
	RealType getTypeById( const int id ); 
	RealType getTypeByName( const QString name ); 
	int getTypeIdByName( const QString name ); 

public slots:
	void displayError( QAbstractSocket::SocketError );

private:
	QString sendData( QString data ); 

	QTcpSocket *socket;
	quint16 blockSize;
	int m_error;
};

#endif // __Q_REAL_REPO_CLIENT_H__
