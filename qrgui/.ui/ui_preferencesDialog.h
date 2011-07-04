/********************************************************************************
** Form generated from reading UI file 'preferencesDialog.ui'
**
** Created: Fri Jul 1 16:47:31 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *okButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QListWidget *listWidget;
    QWidget *pageContentWigdet;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName(QString::fromUtf8("PreferencesDialog"));
        PreferencesDialog->resize(580, 386);
        PreferencesDialog->setMinimumSize(QSize(350, 300));
        PreferencesDialog->setMaximumSize(QSize(600, 1000));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/qreal.png"), QSize(), QIcon::Normal, QIcon::Off);
        PreferencesDialog->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(PreferencesDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        okButton = new QPushButton(PreferencesDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout_2->addWidget(okButton, 1, 1, 1, 1);

        applyButton = new QPushButton(PreferencesDialog);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        gridLayout_2->addWidget(applyButton, 1, 2, 1, 1);

        cancelButton = new QPushButton(PreferencesDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout_2->addWidget(cancelButton, 1, 3, 1, 1);

        listWidget = new QListWidget(PreferencesDialog);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/preferences/editor.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
        __qlistwidgetitem->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/preferences/behaviour.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listWidget);
        __qlistwidgetitem1->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/preferences/miscellaneous.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(listWidget);
        __qlistwidgetitem2->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/preferences/tools.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(listWidget);
        __qlistwidgetitem3->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/preferences/bug.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(listWidget);
        __qlistwidgetitem4->setIcon(icon5);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setFlow(QListView::TopToBottom);
        listWidget->setViewMode(QListView::ListMode);
        listWidget->setUniformItemSizes(true);
        listWidget->setWordWrap(true);

        gridLayout_2->addWidget(listWidget, 0, 0, 1, 1);

        pageContentWigdet = new QWidget(PreferencesDialog);
        pageContentWigdet->setObjectName(QString::fromUtf8("pageContentWigdet"));

        gridLayout_2->addWidget(pageContentWigdet, 0, 1, 1, 3);


        retranslateUi(PreferencesDialog);

        listWidget->setCurrentRow(-1);


        QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferencesDialog)
    {
        PreferencesDialog->setWindowTitle(QApplication::translate("PreferencesDialog", "Preferences", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("PreferencesDialog", "OK", 0, QApplication::UnicodeUTF8));
        applyButton->setText(QApplication::translate("PreferencesDialog", "Apply", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("PreferencesDialog", "Cancel", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("PreferencesDialog", "Editor", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("PreferencesDialog", "Behaviour", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("PreferencesDialog", "Miscellanious", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("PreferencesDialog", "Compiler", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("PreferencesDialog", "Debugger", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
