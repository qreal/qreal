/********************************************************************************
** Form generated from reading UI file 'mousegestures.ui'
**
** Created: Mon 29. Mar 17:23:22 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOUSEGESTURES_H
#define UI_MOUSEGESTURES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MouseGestures
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *horizontalSpacer;
    QTextEdit *teObject;
    QTableWidget *twObjectPathTable;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MouseGestures)
    {
        if (MouseGestures->objectName().isEmpty())
            MouseGestures->setObjectName(QString::fromUtf8("MouseGestures"));
        MouseGestures->resize(507, 569);
        MouseGestures->setDockNestingEnabled(true);
        MouseGestures->setProperty("autoScrillMargin", QVariant(true));
        actionOpen = new QAction(MouseGestures);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        centralWidget = new QWidget(MouseGestures);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalSpacer = new QSpacerItem(486, 298, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        teObject = new QTextEdit(centralWidget);
        teObject->setObjectName(QString::fromUtf8("teObject"));
        teObject->setMaximumSize(QSize(16777215, 59));
        teObject->setReadOnly(true);

        verticalLayout->addWidget(teObject);

        twObjectPathTable = new QTableWidget(centralWidget);
        if (twObjectPathTable->columnCount() < 3)
            twObjectPathTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        twObjectPathTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        twObjectPathTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        twObjectPathTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        twObjectPathTable->setObjectName(QString::fromUtf8("twObjectPathTable"));
        twObjectPathTable->setAutoFillBackground(false);

        verticalLayout->addWidget(twObjectPathTable);

        MouseGestures->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MouseGestures);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 507, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MouseGestures->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MouseGestures);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MouseGestures->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MouseGestures);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MouseGestures->setStatusBar(statusBar);
        toolBar = new QToolBar(MouseGestures);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MouseGestures->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);

        retranslateUi(MouseGestures);

        QMetaObject::connectSlotsByName(MouseGestures);
    } // setupUi

    void retranslateUi(QMainWindow *MouseGestures)
    {
        MouseGestures->setWindowTitle(QApplication::translate("MouseGestures", "MouseGestures", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MouseGestures", "Open", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = twObjectPathTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MouseGestures", "object", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = twObjectPathTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MouseGestures", "path", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = twObjectPathTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MouseGestures", "key", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MouseGestures", "File", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MouseGestures", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MouseGestures: public Ui_MouseGestures {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOUSEGESTURES_H
