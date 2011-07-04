/********************************************************************************
** Form generated from reading UI file 'preferencesCompilerForm.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESCOMPILERFORM_H
#define UI_PREFERENCESCOMPILERFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CompilerForm
{
public:
    QRadioButton *linuxButton;
    QRadioButton *otherButton;
    QRadioButton *windowsButton;
    QWidget *compilerSettingsWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_13;
    QLineEdit *pathToQmake;
    QLabel *label_17;
    QLineEdit *pathToMake;
    QLabel *label_19;
    QLineEdit *pluginExtension;
    QLineEdit *prefix;
    QLabel *label_20;

    void setupUi(QWidget *CompilerForm)
    {
        if (CompilerForm->objectName().isEmpty())
            CompilerForm->setObjectName(QString::fromUtf8("CompilerForm"));
        CompilerForm->resize(420, 300);
        linuxButton = new QRadioButton(CompilerForm);
        linuxButton->setObjectName(QString::fromUtf8("linuxButton"));
        linuxButton->setGeometry(QRect(10, 34, 400, 22));
        otherButton = new QRadioButton(CompilerForm);
        otherButton->setObjectName(QString::fromUtf8("otherButton"));
        otherButton->setGeometry(QRect(10, 62, 400, 22));
        windowsButton = new QRadioButton(CompilerForm);
        windowsButton->setObjectName(QString::fromUtf8("windowsButton"));
        windowsButton->setGeometry(QRect(10, 6, 400, 22));
        windowsButton->setAutoExclusive(true);
        compilerSettingsWidget = new QWidget(CompilerForm);
        compilerSettingsWidget->setObjectName(QString::fromUtf8("compilerSettingsWidget"));
        compilerSettingsWidget->setEnabled(false);
        compilerSettingsWidget->setGeometry(QRect(10, 90, 400, 200));
        gridLayout_2 = new QGridLayout(compilerSettingsWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_13 = new QLabel(compilerSettingsWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 0, 0, 1, 1);

        pathToQmake = new QLineEdit(compilerSettingsWidget);
        pathToQmake->setObjectName(QString::fromUtf8("pathToQmake"));

        gridLayout_2->addWidget(pathToQmake, 0, 1, 1, 1);

        label_17 = new QLabel(compilerSettingsWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 1, 0, 1, 1);

        pathToMake = new QLineEdit(compilerSettingsWidget);
        pathToMake->setObjectName(QString::fromUtf8("pathToMake"));

        gridLayout_2->addWidget(pathToMake, 1, 1, 1, 1);

        label_19 = new QLabel(compilerSettingsWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_2->addWidget(label_19, 2, 0, 1, 1);

        pluginExtension = new QLineEdit(compilerSettingsWidget);
        pluginExtension->setObjectName(QString::fromUtf8("pluginExtension"));

        gridLayout_2->addWidget(pluginExtension, 2, 1, 1, 1);

        prefix = new QLineEdit(compilerSettingsWidget);
        prefix->setObjectName(QString::fromUtf8("prefix"));

        gridLayout_2->addWidget(prefix, 3, 1, 1, 1);

        label_20 = new QLabel(compilerSettingsWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_2->addWidget(label_20, 3, 0, 1, 1);


        retranslateUi(CompilerForm);

        QMetaObject::connectSlotsByName(CompilerForm);
    } // setupUi

    void retranslateUi(QWidget *CompilerForm)
    {
        CompilerForm->setWindowTitle(QApplication::translate("CompilerForm", "Form", 0, QApplication::UnicodeUTF8));
        linuxButton->setText(QApplication::translate("CompilerForm", "Linux", 0, QApplication::UnicodeUTF8));
        otherButton->setText(QApplication::translate("CompilerForm", "Other", 0, QApplication::UnicodeUTF8));
        windowsButton->setText(QApplication::translate("CompilerForm", "Windows", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("CompilerForm", "path to qmake", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("CompilerForm", "path to make", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("CompilerForm", "plugin file extension", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("CompilerForm", "prefix", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CompilerForm: public Ui_CompilerForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESCOMPILERFORM_H
