/********************************************************************************
** Form generated from reading UI file 'fieldeditordialog.ui'
**
** Created: Mon 25. Oct 21:53:13 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIELDEDITORDIALOG_H
#define UI_FIELDEDITORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FieldEditorDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *typeEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *nameEdit;

    void setupUi(QDialog *FieldEditorDialog)
    {
        if (FieldEditorDialog->objectName().isEmpty())
            FieldEditorDialog->setObjectName(QString::fromUtf8("FieldEditorDialog"));
        FieldEditorDialog->resize(395, 108);
        buttonBox = new QDialogButtonBox(FieldEditorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(50, 70, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        widget = new QWidget(FieldEditorDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 381, 60));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        typeEdit = new QLineEdit(widget);
        typeEdit->setObjectName(QString::fromUtf8("typeEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(typeEdit->sizePolicy().hasHeightForWidth());
        typeEdit->setSizePolicy(sizePolicy);
        typeEdit->setMinimumSize(QSize(250, 0));

        horizontalLayout_2->addWidget(typeEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        nameEdit = new QLineEdit(widget);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        sizePolicy.setHeightForWidth(nameEdit->sizePolicy().hasHeightForWidth());
        nameEdit->setSizePolicy(sizePolicy);
        nameEdit->setMinimumSize(QSize(250, 0));

        horizontalLayout->addWidget(nameEdit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(FieldEditorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), FieldEditorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FieldEditorDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(FieldEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *FieldEditorDialog)
    {
        FieldEditorDialog->setWindowTitle(QApplication::translate("FieldEditorDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("FieldEditorDialog", "type", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FieldEditorDialog", "name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FieldEditorDialog: public Ui_FieldEditorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIELDEDITORDIALOG_H
