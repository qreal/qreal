/********************************************************************************
** Form generated from reading UI file 'methodeditordialog.ui'
**
** Created: Mon 25. Oct 21:53:13 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METHODEDITORDIALOG_H
#define UI_METHODEDITORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MethodEditorDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *accessModifierComboBox;
    QLabel *label;
    QLineEdit *nameEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *returnTypeEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QPushButton *addParameterBtn;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLabel *label_5;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MethodEditorDialog)
    {
        if (MethodEditorDialog->objectName().isEmpty())
            MethodEditorDialog->setObjectName(QString::fromUtf8("MethodEditorDialog"));
        MethodEditorDialog->resize(400, 180);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MethodEditorDialog->sizePolicy().hasHeightForWidth());
        MethodEditorDialog->setSizePolicy(sizePolicy);
        MethodEditorDialog->setSizeIncrement(QSize(0, 0));
        MethodEditorDialog->setBaseSize(QSize(0, 0));
        MethodEditorDialog->setSizeGripEnabled(false);
        layoutWidget = new QWidget(MethodEditorDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 381, 141));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
#ifndef Q_OS_MAC
        verticalLayout_2->setSpacing(-1);
#endif
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        accessModifierComboBox = new QComboBox(layoutWidget);
        accessModifierComboBox->setObjectName(QString::fromUtf8("accessModifierComboBox"));

        horizontalLayout->addWidget(accessModifierComboBox);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        nameEdit = new QLineEdit(layoutWidget);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        horizontalLayout->addWidget(nameEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        returnTypeEdit = new QLineEdit(layoutWidget);
        returnTypeEdit->setObjectName(QString::fromUtf8("returnTypeEdit"));

        horizontalLayout_2->addWidget(returnTypeEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        addParameterBtn = new QPushButton(layoutWidget);
        addParameterBtn->setObjectName(QString::fromUtf8("addParameterBtn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(addParameterBtn->sizePolicy().hasHeightForWidth());
        addParameterBtn->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(addParameterBtn);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetMinimumSize);
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);


        verticalLayout_2->addLayout(horizontalLayout_4);

        buttonBox = new QDialogButtonBox(MethodEditorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(200, 150, 181, 25));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy2);
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        retranslateUi(MethodEditorDialog);

        QMetaObject::connectSlotsByName(MethodEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *MethodEditorDialog)
    {
        MethodEditorDialog->setWindowTitle(QApplication::translate("MethodEditorDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        accessModifierComboBox->clear();
        accessModifierComboBox->insertItems(0, QStringList()
         << QApplication::translate("MethodEditorDialog", "+", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MethodEditorDialog", "-", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MethodEditorDialog", "#", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_STATUSTIP
        accessModifierComboBox->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("MethodEditorDialog", "Name: ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MethodEditorDialog", "Return type: ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MethodEditorDialog", "Parametries: ", 0, QApplication::UnicodeUTF8));
        addParameterBtn->setText(QApplication::translate("MethodEditorDialog", "+", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MethodEditorDialog", "name", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MethodEditorDialog", "type", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MethodEditorDialog: public Ui_MethodEditorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METHODEDITORDIALOG_H
