/********************************************************************************
** Form generated from reading UI file 'editorGeneratorDialog.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORGENERATORDIALOG_H
#define UI_EDITORGENERATORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorGeneratorDialogUi
{
public:
    QListWidget *listWidget;
    QPushButton *buttonMakeEditor;

    void setupUi(QWidget *EditorGeneratorDialogUi)
    {
        if (EditorGeneratorDialogUi->objectName().isEmpty())
            EditorGeneratorDialogUi->setObjectName(QString::fromUtf8("EditorGeneratorDialogUi"));
        EditorGeneratorDialogUi->resize(424, 310);
        listWidget = new QListWidget(EditorGeneratorDialogUi);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 291, 291));
        buttonMakeEditor = new QPushButton(EditorGeneratorDialogUi);
        buttonMakeEditor->setObjectName(QString::fromUtf8("buttonMakeEditor"));
        buttonMakeEditor->setGeometry(QRect(310, 10, 105, 25));

        retranslateUi(EditorGeneratorDialogUi);

        QMetaObject::connectSlotsByName(EditorGeneratorDialogUi);
    } // setupUi

    void retranslateUi(QWidget *EditorGeneratorDialogUi)
    {
        EditorGeneratorDialogUi->setWindowTitle(QApplication::translate("EditorGeneratorDialogUi", "meta_generator", 0, QApplication::UnicodeUTF8));
        buttonMakeEditor->setText(QApplication::translate("EditorGeneratorDialogUi", "Generate!", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditorGeneratorDialogUi: public Ui_EditorGeneratorDialogUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORGENERATORDIALOG_H
