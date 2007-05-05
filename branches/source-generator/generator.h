#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QFile>
#include <QDir>

#include "entity.h"

// the generator itself
class Generator
{
public:
    Generator( QStringList );
    ~Generator();

private:

    static const int NUM = 1;

    int untitled;

    void parseFile( QString );
    void parseEnum( QDomNode );
    void parseNode( QDomNode );
    void parseEdge( QDomNode );
    
    void parseGeneralizations ( Entity*, QDomNode );
    void parseProperties      ( Entity*, QDomNode );
    void parseAssociations    ( Entity*, QDomNode, bool isNode = false );
    //void parseNodeAssociations( Entity*, QDomNode );
    void parseSVG             ( Entity*, QDomNode );

    void genEnums();
    void genSQLScripts();
    void genMappings();
    void genClasses();
    void genFactory();
    void genEdgesFunction();

    void propagateAll();
    void propagateProperties( Entity* );
    void propagateAssocs( Edge* );

    int position( QString );

    Entity* find( QString id );
    Edge* findEdge( QString id );

    QList< Entity* > objects; 
    QList< Edge* > edges;
    
    QMap<QString, QStringList> enumerations;

    QString res;
    QString resources;

    QDir dir;
};

#endif
