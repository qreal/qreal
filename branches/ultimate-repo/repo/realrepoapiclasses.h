#ifndef __REAL_REPO_API_CLASSES_H__
#define __REAL_REPO_API_CLASSES_H__

#include <QList>

namespace QRealTypes
{
        typedef QList<int> QIntList;

        // Metatype description
        enum MetaType
        {
                object, // types of diagram objects (e.g. invoke, exit, if, foreach, throw, reply)
                link, // edges
                dataType, // all common data types: basic (int, double, string, char) and
			  // defined by user ( records/structs, xsd etc.)
                rawType // 'raw' string data types (e.g. wsdl, xsd, project configuration stuff etc.)
        };

        // Type description
        class RealType
        {
        public:
                int getId();

                QString getName() const;
                void setName(const QString);

                QString getDescription() const;
                void setDescription(const QString);

                void setProperty(const QString name, const QString val );
                QString getProperty(const QString name ) const; // returns "" in case of empty property value
                int getPropertiesCount(); 

                enum MetaType getMetaType() const; 
                void setMetaType(const enum MetaType);

                QIntList getObjects() const; // returns all objects of this particular type
	
	private:
		int id;
		int metatype;
		QString name;
		QString description;
		QMap<QString, QString> properties;

		QIntList objects;
        };

	// Node elements
        class RealObject
        {
        public:
                int getId();

                QString getName() const;
                void setName(const QString);

                QString getDescription() const;
                void setDescription(const QString);

                void setProperty(const QString name, const QString val );
                QString getProperty(const QString name ) const; // returns "" in case of empty property value
                int getPropertiesCount();

                int getTypeId() const;
                void setTypeId(const int);

                bool getVisibility() const; // is it visible on the diagrams
                void setVisibility(const bool);

                int getContainerId() const; 
                void setContainerId(const int);

                QString getConfiguration() const;
                void setConfiguration(const QString);

                QIntList getChildElements() const;
                void addChildElement(const int);
                void deleteChildElement(const int);

                QIntList getAllLinks() const;
                QIntList getIncomingLinks() const;
                QIntList getOutcomingLinks() const;
                void addIncomingLink(const int);
                void addOutcomingLink(const int);

	private:
		int id;
		int typeId;
		QString name;
		QString description;
		QMap<QString, QString> properties;
		bool isVisible;
		int containerId;
		QString configuration;

		QIntList children;
		QIntList links;
        };
        
	// Edge elements
        class RealLink
        {
        public:
                int getId();

                QString getName() const;
                void setName(const QString);

                 int getTypeId() const;
                void setTypeId(const int);

		void setProperty(const QString name, const QString val );
                QString getProperty(const QString name ) const; // returns "" in case of empty property value
                int getPropertiesCount(); 
                
                int getFromId() const;  // source node
                void setFromId(const int);

                int getToId() const;    // target node
                void setToId(const int);
	
	private:
		int id;
		int type;

		QMap<QString, QString> properties;
		int fromId;
		int toId;
        };
}

#endif // __REAL_REPO_API_CLASSES_H__

