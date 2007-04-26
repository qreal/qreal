#ifndef PARSER_h
#define PARSER_H

#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QFile>
#include <QDir>
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
    QList<Entity*> links;
    QMap<QString, QStringList> enumerations;

    QString res;
    QString resource;
    QDir dir;
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
