#ifndef __REAL_REPO_API_CLASSES_H__
#define __REAL_REPO_API_CLASSES_H__

#include <QList>
#include <QMap>
#include <QString>
#include <QDebug>
#include "defs.h"

#include "defs.h"

class RealRepoClient;

namespace QRealTypes
{
	typedef QList<int> QIntList;

	// common stuff for all classes
	class RealNamedEntity
	{
	public:
		RealNamedEntity() 
		{
			m_name = "noname";
			m_description = "noname";
			m_properties.clear();
			client = 0;
		}

		int getId();
		void setId( const int id ); // to be removed soon

		QString getName();
		void setName( const QString& );

		int getTypeId() const;
		void setTypeId( const int );

		QString getDescription() const;
		void setDescription( const QString& );

		void setProperty( const QString& name, const QString& val );
		QString getProperty( const QString& name ) const; // returns "" in case of empty property value
		int getPropertiesCount() const; 

		void setRepoClient( RealRepoClient * );

	protected:
		int m_id;
		int m_type;
		QString m_name;
		QString m_description;
		QMap<QString, QString> m_properties;	
		RealRepoClient  *client;
	};

	// Metatype description
	enum MetaType
	{
		object, // types of diagram objects (e.g. invoke, exit, if, foreach, throw, reply)
		link, // edges
		dataType, // all common data types: basic (int, double, string, char) and
			// defined by user ( records/structs, xsd etc.)
		rawType, // 'raw' string data types (e.g. wsdl, xsd, project configuration stuff etc.)
	};

	typedef enum MetaType MetaType;

	// Type description
	class RealType : public RealNamedEntity
	{
	public:
		RealType() { m_objects.clear(); }
		// In debug purposes -- subject to be removed. 
		~RealType() { /*qDebug() << "-destroying " << getName() << endl;*/ }

		MetaType getMetaType() const; 
		void setMetaType( const MetaType );

		QIntList getObjects() const; // returns all objects of this particular type
		
		void addObject( int id );
		void deleteObject( int id );

		QString toString() const;
		void loadFromString( const QString& );
	
	private:

		MetaType m_metatype;
		QIntList m_objects;
	};

	// Node elements
	class RealObject : public RealNamedEntity
	{
 	public:
		bool getVisibility() const; // is it visible on the diagrams or not
		void setVisibility( const bool );

		int getContainerId() const; 
		void setContainerId( const int );

		QString getConfiguration() const;
		void setConfiguration( const QString& );

		QIntList getChildElements() const;
		void addChildElement( const int );
		void deleteChildElement( const int );

		QIntList getAllLinks();
		QIntList getIncomingLinks();
 		QIntList getOutcomingLinks();

		void addIncomingLink( const int );
		void addOutcomingLink( const int );

		void removeIncomingLink( const int );
		void removeOutcomingLink( const int );

	private:
		bool m_visibility;
		int m_containerId;
		QString m_configuration;

		QIntList m_children;
		QIntList m_incomingLinks;
		QIntList m_outcomingLinks;
	};

	// Edge elements
	class RealLink : public RealNamedEntity
	{
	public:
		int getFromId() const;  // source node
		void setFromId( const int );

		int getToId() const;    // target node
		void setToId( const int );

		void removeFrom( const int );
		void removeTo( const int );
	
	private:

		int m_fromId;
		int m_toId;
	};
}

#endif // __REAL_REPO_API_CLASSES_H__

