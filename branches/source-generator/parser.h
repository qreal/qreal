#ifndef PARSER_h
#define PARSER_H

#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QFile>
#include <QStringList>


class Entity;

class Parser{

public:
    Parser( QStringList );
    ~Parser();
   
private:
    
    QStringList files;
    void run( QString );
    Entity* find( QString );
    void display();
    void genEnums();
    void genSQLScripts();
    void genMappings();
    void genClasses();
    void genFactory();
    
    QList<Entity*> objects;
    QMap<QString, QStringList> enumerations;
    
    
    QDomNodeList nodes;
   
};

struct Entity{

    QString id;
    QString parent;
    QList< QPair<QString, QString> > properties;
    QStringList associations;
    int height;
    int width;
};

#endif
