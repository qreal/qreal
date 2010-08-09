/********************************************************************************
** Form generated from reading UI file 'windows.ui'
**
** Created: Mon 12. Jul 15:50:43 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWS_H
#define UI_WINDOWS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Windows
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QFrame *frame;

    void setupUi(QMainWindow *Windows)
    {
        if (Windows->objectName().isEmpty())
            Windows->setObjectName(QString::fromUtf8("Windows"));
        Windows->resize(600, 400);
        centralWidget = new QWidget(Windows);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(110, 60, 91, 41));
        Windows->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Windows);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        Windows->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Windows);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Windows->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Windows);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Windows->setStatusBar(statusBar);
        dockWidget = new QDockWidget(Windows);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(80, 50));
        dockWidget->setLayoutDirection(Qt::RightToLeft);
        dockWidget->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(dockWidgetContents);
        frame->setObjectName(QString::fromUtf8("frame"));
        QPalette palette;
        QBrush brush(QColor(0, 255, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        frame->setPalette(palette);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame);

        dockWidget->setWidget(dockWidgetContents);
        Windows->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        retranslateUi(Windows);

        QMetaObject::connectSlotsByName(Windows);
    } // setupUi

    void retranslateUi(QMainWindow *Windows)
    {
        Windows->setWindowTitle(QApplication::translate("Windows", "Windows", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Windows", "PushButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Windows: public Ui_Windows {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWS_H
