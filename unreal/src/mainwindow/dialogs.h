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

#ifndef DIALOGS_H
#define DIALOGS_H


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


class  AddElementDialog : public QDialog{
    Q_OBJECT
public:
    AddElementDialog(QWidget *parent = 0);


    QLabel *lName;
    QLabel *lDescription;
    QLabel *lPriority;
    QLabel *lSource;
    QLabel *lStatus;
    QLineEdit *eName;
    QLineEdit *eDescription;
    QLineEdit *ePriority;
    QLineEdit *eSource;
    QLineEdit *eStatus;
    QPushButton *okButton;
    QPushButton *closeButton;
};

class  AddDiagramDialog : public QDialog{
    Q_OBJECT
public:
    AddDiagramDialog(QWidget *parent = 0);


    QLabel *lName;    
    QLineEdit *eName;    
    QPushButton *okButton;
    QPushButton *closeButton;
};






#endif
