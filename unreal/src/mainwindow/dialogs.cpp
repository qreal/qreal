//=====================================================================
// Project:      unREAL
// File Name:    dialogs.cpp
// Description:  Some useful Dialogs 
//
// Created:      03-Feb-07
// Revision:     21-Mar-07
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include "dialogs.h"


AddDiagramDialog::AddDiagramDialog(QWidget *parent) : QDialog(parent) {
//dbg;    
    lName = new QLabel(tr("name: "));
    eName = new QLineEdit("req_diagram_1");
    lStat = new QLabel(tr("status: "));
    eStat = new QLineEdit("");

    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lStat);
    h2->addWidget(eStat);
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);     
    
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h6);
    
    setLayout(v);
    
    setWindowTitle(tr("add diagram")); 
}

AddElementDialog::AddElementDialog(QWidget *parent) : QDialog(parent) {
//dbg;
    lName = new QLabel(tr("name: "));
    lDiagram = new QLabel(tr("diagaram: "));
    lDescription = new QLabel(tr("desc: "));
    lPriority = new QLabel(tr("prior: "));
    lSource = new QLabel("source: ");
    lStatus = new QLabel(tr("status: "));
    lX = new QLabel(tr("x: "));
    lY = new QLabel(tr("x: "));
    eName = new QLineEdit("element 1");
    eDiagram = new QLineEdit("req_diagram_1");
    eDescription = new QLineEdit("desc 1");
    ePriority = new QLineEdit("1");
    eSource = new QLineEdit("source 1");
    eStatus = new QLineEdit("status 1");
    eX = new QLineEdit("0");
    eY = new QLineEdit("0");
    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
    QHBoxLayout *h11 = new QHBoxLayout;
    h11->addWidget(lDiagram);
    h11->addWidget(eDiagram);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lDescription);
    h2->addWidget(eDescription);
    QHBoxLayout *h3 = new QHBoxLayout;
    h3->addWidget(lPriority);
    h3->addWidget(ePriority);
    QHBoxLayout *h4 = new QHBoxLayout;
    h4->addWidget(lSource);
    h4->addWidget(eSource);
    QHBoxLayout *h5 = new QHBoxLayout;
    h5->addWidget(lStatus);    
    h5->addWidget(eStatus);               
    QHBoxLayout *h51 = new QHBoxLayout;
    h51->addWidget(lX);    
    h51->addWidget(eX);               
    QHBoxLayout *h52 = new QHBoxLayout;
    h52->addWidget(lY);    
    h52->addWidget(eY);               
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);               
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h11);
    v->addLayout(h2);
    v->addLayout(h3);
    v->addLayout(h4);
    v->addLayout(h5);
    v->addLayout(h51);
    v->addLayout(h52);
    v->addLayout(h6);
    setLayout(v);
    
    setWindowTitle(tr("add element")); 

}


RemoveDiagramDialog::RemoveDiagramDialog(QWidget *parent) : QDialog(parent) {
//dbg;    
    lName = new QLabel(tr("name: "));
    eName = new QLineEdit("req_diagram_");
    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);     
    
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h6);
    
    setLayout(v);
    
    setWindowTitle(tr("remove diagram")); 
}


RemoveElementDialog::RemoveElementDialog(QWidget *parent) : QDialog(parent) {
//dbg;    
    lName = new QLabel(tr("name: "));
    eName = new QLineEdit("req_diagram_1/element 1");
    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);     
    
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h6);
    
    setLayout(v);
    
    setWindowTitle(tr("remove element")); 
}

RepoOptionsDialog::RepoOptionsDialog(QStringList list, QWidget *parent) : QDialog(parent) {
//dbg;    
    QString hostName = list.at(0);
    QString dbName   = list.at(1);
    QString userName = list.at(2);

    lHostName = new QLabel(tr("host name: "));
    eHostName = new QLineEdit(hostName);
    lDBName   = new QLabel(tr("db name: "));
    eDBName   = new QLineEdit(dbName);
    lUserName = new QLabel(tr("user name: "));
    eUserName = new QLineEdit(userName);
    lPasswd   = new QLabel(tr("password: "));
    ePasswd   = new QLineEdit("");
    
    okButton = new QPushButton(tr("reconnect"));
    closeButton = new QPushButton(tr("abort"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lHostName);
    h1->addWidget(eHostName);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lDBName);
    h2->addWidget(eDBName);
    QHBoxLayout *h3 = new QHBoxLayout;
    h3->addWidget(lUserName);
    h3->addWidget(eUserName);
    QHBoxLayout *h4 = new QHBoxLayout;
    h4->addWidget(lPasswd);
    h4->addWidget(ePasswd);
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);     
    
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
    v->addLayout(h4);
    v->addLayout(h6);
    
    setLayout(v);
    
    setWindowTitle(tr("repository settings")); 
}

AddLinkDialog::AddLinkDialog(QWidget *parent) : QDialog(parent) {
//dbg;
    lName = new QLabel(tr("name: "));
    eName = new QLineEdit("link 1");
    lDiagram = new QLabel(tr("diagram: "));
    eDiagram = new QLineEdit(tr("req_diagram_1"));
    lFrom = new QLabel(tr("from: "));
    eFrom = new QLineEdit("");
    lStat = new QLabel(tr("status:"));
    eStat = new QLineEdit("");
    lTo   = new QLabel(tr("to:   "));
    eTo   = new QLineEdit("");
    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
    QHBoxLayout *h11 = new QHBoxLayout;
    h11->addWidget(lDiagram);
    h11->addWidget(eDiagram);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lFrom);
    h2->addWidget(eFrom);
    QHBoxLayout *h3 = new QHBoxLayout;
    h3->addWidget(lTo);
    h3->addWidget(eTo);
    QHBoxLayout *h4 = new QHBoxLayout;
    h4->addWidget(lStat);
    h4->addWidget(eStat);
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);     
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h11);
    v->addLayout(h2);
    v->addLayout(h3);
    v->addLayout(h4);
    v->addLayout(h6);
    setLayout(v);
    
    setWindowTitle(tr("add link")); 

}

MoveDialog::MoveDialog(QWidget *parent) : QDialog(parent) {
//dbg;
    lName = new QLabel(tr("name: "));
    eName = new QLineEdit("aaa");
    lFrom = new QLabel(tr("from: "));
    eFrom = new QLineEdit("");
    lTo   = new QLabel(tr("to:   "));
    eTo   = new QLineEdit("req_diagram_1");
    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(lFrom);
    h2->addWidget(eFrom);
    QHBoxLayout *h3 = new QHBoxLayout;
    h3->addWidget(lTo);
    h3->addWidget(eTo);
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);     
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
    v->addLayout(h6);
    setLayout(v);
    
    setWindowTitle(tr("move")); 

}

