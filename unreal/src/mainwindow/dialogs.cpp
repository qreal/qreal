//=====================================================================
// Project:      unREAL
// File Name:    dialogs.cpp
// Description:  Some useful Dialogs 
//
// Created:      03-Feb-07
// Revision:     
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include "dialogs.h"


AddDiagramDialog::AddDiagramDialog(QWidget *parent) : QDialog(parent) {
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
    
    setWindowTitle(tr("add diagram")); 
}

AddElementDialog::AddElementDialog(QWidget *parent) : QDialog(parent) {
//dbg;
    lName = new QLabel(tr("name: "));
    lDescription = new QLabel(tr("desc: "));
    lPriority = new QLabel(tr("prior: "));
    lSource = new QLabel("source: ");
    lStatus = new QLabel(tr("status: "));
    eName = new QLineEdit("element 1");
    eDescription = new QLineEdit("desc 1");
    ePriority = new QLineEdit("1");
    eSource = new QLineEdit("source 1");
    eStatus = new QLineEdit("status 1");
    okButton = new QPushButton(tr("ok"));
    closeButton = new QPushButton(tr("close"));
    
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    
    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addWidget(lName);
    h1->addWidget(eName);
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
    QHBoxLayout *h6 = new QHBoxLayout;
    h6->addWidget(okButton);    
    h6->addWidget(closeButton);               
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
    v->addLayout(h4);
    v->addLayout(h5);
    v->addLayout(h6);
    setLayout(v);
    
    setWindowTitle(tr("add element")); 
}
