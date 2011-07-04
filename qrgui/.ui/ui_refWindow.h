/********************************************************************************
** Form generated from reading UI file 'refWindow.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFWINDOW_H
#define UI_REFWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RefWindow
{
public:
    QGridLayout *gridLayout;
    QPushButton *mButtonOk;
    QPushButton *mButtonCancel;
    QListWidget *listWidget;

    void setupUi(QWidget *RefWindow)
    {
        if (RefWindow->objectName().isEmpty())
            RefWindow->setObjectName(QString::fromUtf8("RefWindow"));
        RefWindow->resize(400, 300);
        gridLayout = new QGridLayout(RefWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mButtonOk = new QPushButton(RefWindow);
        mButtonOk->setObjectName(QString::fromUtf8("mButtonOk"));

        gridLayout->addWidget(mButtonOk, 1, 0, 1, 1);

        mButtonCancel = new QPushButton(RefWindow);
        mButtonCancel->setObjectName(QString::fromUtf8("mButtonCancel"));

        gridLayout->addWidget(mButtonCancel, 1, 1, 1, 1);

        listWidget = new QListWidget(RefWindow);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 0, 0, 1, 2);


        retranslateUi(RefWindow);
        QObject::connect(mButtonCancel, SIGNAL(clicked()), RefWindow, SLOT(close()));
        QObject::connect(mButtonOk, SIGNAL(clicked()), RefWindow, SLOT(setPropertyValue()));

        QMetaObject::connectSlotsByName(RefWindow);
    } // setupUi

    void retranslateUi(QWidget *RefWindow)
    {
        RefWindow->setWindowTitle(QApplication::translate("RefWindow", "Reference window", 0, QApplication::UnicodeUTF8));
        mButtonOk->setText(QApplication::translate("RefWindow", "&OK", 0, QApplication::UnicodeUTF8));
        mButtonCancel->setText(QApplication::translate("RefWindow", "&Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RefWindow: public Ui_RefWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFWINDOW_H
