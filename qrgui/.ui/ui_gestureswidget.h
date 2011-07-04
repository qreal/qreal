/********************************************************************************
** Form generated from reading UI file 'gestureswidget.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTURESWIDGET_H
#define UI_GESTURESWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GesturesWidget
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QGraphicsView *graphicsView;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *GesturesWidget)
    {
        if (GesturesWidget->objectName().isEmpty())
            GesturesWidget->setObjectName(QString::fromUtf8("GesturesWidget"));
        GesturesWidget->resize(897, 580);
        gridLayout = new QGridLayout(GesturesWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(GesturesWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(listWidget, 2, 0, 1, 1);

        graphicsView = new QGraphicsView(GesturesWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setMinimumSize(QSize(256, 0));

        gridLayout->addWidget(graphicsView, 2, 1, 1, 1);

        label = new QLabel(GesturesWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(GesturesWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        retranslateUi(GesturesWidget);

        QMetaObject::connectSlotsByName(GesturesWidget);
    } // setupUi

    void retranslateUi(QWidget *GesturesWidget)
    {
        GesturesWidget->setWindowTitle(QApplication::translate("GesturesWidget", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GesturesWidget", "List of mouse gestures", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GesturesWidget", "Click to see how to draw it:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GesturesWidget: public Ui_GesturesWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTURESWIDGET_H
