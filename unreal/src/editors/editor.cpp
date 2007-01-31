//=====================================================================
// Project:      unREAL
// File Name:    editor.cpp
// Description:  Editor 
//
// Created:      30-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include "editor.h"
#include "dbg.h"
#include <QDebug>

Editor::Editor(){
//dbg;   

    createActions();
    createNodes();
    createEdges();
    execDBScripts();
}   


Editor::~Editor(){
//dbg;
    qDeleteAll(actions.begin(), actions.end());
    qDeleteAll(nodes.begin(), nodes.end());
    qDeleteAll(edges.begin(), edges.end());
}


void Editor::createActions(){
//dbg;
    QAction *tmp = new QAction(QObject::tr("Featured Element"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Concept Alternative"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Leaf"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Parent Node"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Parent Of Mandatory Features"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Parent of Or-Features"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Parent of Optional Features"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Parent of Alternative Features"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Parent of Optional-Alternative Features"), 0);
    actions << tmp;
    tmp = new QAction(QObject::tr("Feature P2N Relationship"), 0);
    actions << tmp;
}

void Editor::createNodes(){
    
    Element *el = new Element();
   
    //Featured Element
    el->svg = "shapes/nFeatured.svg";
    Property *p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Featured";
    nodes.insert("nFeatured", el);

    el = new Element();

    //Concept Alternative
    el->svg = "shapes/nConcAlternative.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Featured";
    nodes.insert("nConcAlternative", el);

    el = new Element();

    //Leaf
    el->svg = "shapes/nLeaf.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Featured";
    nodes.insert("nLeaf", el);

    el = new Element();

    //Parent Node
    el->svg = "shapes/nParent.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Parent";
    nodes.insert("nParent", el);

    el = new Element();

    //Parent Of Mandatory Features
    el->svg = "shapes/nParentMandatory.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Parent";
    nodes.insert("nParentMandatory", el);

    el = new Element();

    //Parent Of Or-Features
    el->svg = "shapes/nParentOr.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Parent";
    nodes.insert("nParentOr", el);

    el = new Element();

    //Parent Of Optional Features
    el->svg = "shapes/nParentOptional.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Parent";
    nodes.insert("nParentOptional", el);

    el = new Element();

    //Parent Of Alternative Features
    el->svg = "shapes/nParentAlternative.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Parent";
    nodes.insert("nParentAlternative", el);

    el = new Element();

    //Parent Of Optional-Alternative Features
    el->svg = "shapes/nParentOpAlternative.svg";
    p = new Property();
    p->name = "name";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "description";
    p->type = "text";
    el->properties << p;
    p = new Property();
    p->name = "priority";
    p->type = "int";
    el->properties << p;
    p = new Property();
    p->name = "source";
    p->type = "string";
    el->properties << p;
    p = new Property();
    p->name = "static";
    p->type = "string";
    el->properties << p;
    el->links << "aP2N_Parent";
    nodes.insert("nParentOpAlternative", el);

}

void Editor::createEdges(){
//dbg;
    Link *link = new Link();
    
    //Feature P2N Relationship
    link->svg = "";
    link->beginsWith << "nFeatured";
    link->endsWith   << "nParent";
    edges.insert("eP2N", link);
}

void Editor::execDBScripts(){
//dbg;

    QSqlQuery q;
    QSqlRecord r;
    q.exec("select id from diagram where name='nFeatured'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nFeatured', 'objects')");
        q.exec("create table nFeatured (id integer primary key auto_increment, name varchar(20),"  
            " description text, priority integer, source varchar(20), status varchar(20))");
    }    
    else 
        qDebug() << "value:" << r.value(0).toString();
    
    q.exec("select id from diagram where name='nConcAlternative'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nConcAlternative', 'objects')");
        q.exec("create table nConcAlternative (id integer primary key auto_increment,"  
            " name varchar(20), description text, priority integer, source varchar(20), status varchar(20))");
    }
    
    q.exec("select id from diagram where name='nLeaf'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nLeaf', 'objects')");
        q.exec("create table nLeaf (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    }
    
    q.exec("select id from diagram where name='nParent'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nParent', 'objects')");
        q.exec("create table nParent (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    }   
    
    q.exec("select id from diagram where name='nParentMandatory'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nParentMandatory', 'objects')");
        q.exec("create table nParentMandatory (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    } 
    
    q.exec("select id from diagram where name='nParentOr'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nParentOr', 'objects')");
        q.exec("create table nParentOr (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    }
    
    q.exec("select id from diagram where name='nParentOptional'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nParentOptional', 'objects')");
        q.exec("create table nParentOptional (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    }
    
    q.exec("select id from diagram where name='nParentAlternative'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nParentAlternative', 'objects')");
        q.exec("create table nParentAlternative (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    }
    
    q.exec("select id from diagram where name='nParentOpAlternative'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('nParentOpAlternative', 'objects')");
        q.exec("create table nParentOpAlternative (id integer primary key auto_increment, name varchar(20),"
            " description text, priority integer, source varchar(20), status varchar(20))");
    }
    
    q.exec("select id from diagram where name='eP2N'");
    //r = q.record();
    //if (r.isEmpty()){
    if (q.size() == 0){
        q.exec("insert into diagram (name, type) values ('eP2N', 'objects')");
        q.exec("create table eP2N (id integer primary key auto_increment, beginsWith varchar(20), endsWith varchar(20))");
    }
       
}
