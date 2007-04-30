#ifndef ENTITY_H
#define ENTITY_H

#include <QStringList>
#include <QList>
#include <QPair>


enum elementsType { NODE, EDGE };

// parent for nodes and edges class
class Entity
{
public:
    Entity(){ propsPropagated = false; }

    void addParent( QString );
    void addProperty( QString, QString );

    int height; 
    int width;

    QString id;
    QStringList parents;

    QList< QPair<QString, QString> > properties;

    elementsType type;

    bool propsPropagated;
};

// node class
class Node : public Entity
{
public:
    //TODO: containers 
    QStringList associations; 
    
};

// edge class
class Edge : public Entity
{
public:
    QStringList beginsWith;
    QStringList endsWith;
    QStringList beginRole;
    QStringList endRole;
};



#endif
