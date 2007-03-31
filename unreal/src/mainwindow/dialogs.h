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

#ifndef DIALOGS_H
#define DIALOGS_H


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


class AddElementDialog : public QDialog{
    Q_OBJECT
public:
    AddElementDialog(QWidget *parent = 0);


    QLabel *lName;
    QLabel *lDiagram;
    QLabel *lDescription;
    QLabel *lPriority;
    QLabel *lSource;
    QLabel *lStatus;
    QLabel *lX;
    QLabel *lY;
    QLineEdit *eName;
    QLineEdit *eDiagram;
    QLineEdit *eDescription;
    QLineEdit *ePriority;
    QLineEdit *eSource;
    QLineEdit *eStatus;
    QLineEdit *eX;
    QLineEdit *eY;
    QPushButton *okButton;
    QPushButton *closeButton;
};

class AddDiagramDialog : public QDialog{
    Q_OBJECT
public:
    AddDiagramDialog(QWidget *parent = 0);

    QLabel *lName;    
    QLineEdit *eName;    
    QLabel *lStat;    
    QLineEdit *eStat;    
    QPushButton *okButton;
    QPushButton *closeButton;
};



class RemoveDiagramDialog : public QDialog{
    Q_OBJECT
public:
    RemoveDiagramDialog(QWidget *parent = 0);

    QLabel *lName;    
    QLineEdit *eName;    
    QPushButton *okButton;
    QPushButton *closeButton;
};

class RemoveElementDialog : public QDialog{
    Q_OBJECT
public:
    RemoveElementDialog(QWidget *parent = 0);

    QLabel *lName;    
    QLabel *lType;
    QLabel *lDgr;
    QLineEdit *eName;    
    QLineEdit *eType;
    QLineEdit *eDgr;
    QPushButton *okButton;
    QPushButton *closeButton;
};

class RepoOptionsDialog : public QDialog{
    Q_OBJECT
public:
    RepoOptionsDialog(QStringList list, QWidget *parent = 0);

    QLabel    *lHostName;    
    QLineEdit *eHostName;    
    QLabel    *lDBName;    
    QLineEdit *eDBName;    
    QLabel    *lUserName;    
    QLineEdit *eUserName;    
    QLabel    *lPasswd;    
    QLineEdit *ePasswd;    
    QPushButton *okButton;
    QPushButton *closeButton;
};

class AddLinkDialog : public QDialog{
    Q_OBJECT
public:
    AddLinkDialog(QWidget *parent = 0);

    QLabel *lFrom;
    QLabel *lTo;
    QLabel *lName;
    QLabel *lStat;
    QLabel *lDiagram;
    QLineEdit *eFrom;
    QLineEdit *eTo;
    QLineEdit *eName;
    QLineEdit *eStat;
    QLineEdit *eDiagram;
    QPushButton *okButton;
    QPushButton *closeButton;
};

class MoveDialog : public QDialog{
    Q_OBJECT
public:
    MoveDialog(QWidget *parent = 0);

    QLabel *lFrom;
    QLabel *lTo;
    QLabel *lName;
    QLabel *lType;
    QLineEdit *eType;
    QLineEdit *eFrom;
    QLineEdit *eTo;
    QLineEdit *eName;
    QPushButton *okButton;
    QPushButton *closeButton;
};
#endif
