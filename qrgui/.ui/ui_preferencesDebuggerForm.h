/********************************************************************************
** Form generated from reading UI file 'preferencesDebuggerForm.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDEBUGGERFORM_H
#define UI_PREFERENCESDEBUGGERFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DebuggerForm
{
public:
    QFrame *frame_9;
    QLabel *label_18;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QLineEdit *debuggerPathLineEdit;
    QLineEdit *builderPathLineEdit;
    QLineEdit *codeFileNameLineEdit;
    QLineEdit *buildedFileNameLineEdit;
    QPushButton *browseDebPathButton;
    QPushButton *builderPathButton;
    QLabel *label_25;
    QLineEdit *workDirLineEdit;
    QPushButton *workDirPushButton;
    QFrame *frame_7;
    QLabel *label_14;
    QLineEdit *timeoutLineEdit;
    QLabel *label_15;
    QComboBox *colorComboBox;
    QLabel *label_16;
    QLabel *label_24;

    void setupUi(QWidget *DebuggerForm)
    {
        if (DebuggerForm->objectName().isEmpty())
            DebuggerForm->setObjectName(QString::fromUtf8("DebuggerForm"));
        DebuggerForm->resize(420, 300);
        frame_9 = new QFrame(DebuggerForm);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setGeometry(QRect(10, 130, 400, 161));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        label_18 = new QLabel(frame_9);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 10, 171, 16));
        label_21 = new QLabel(frame_9);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(10, 40, 171, 16));
        label_22 = new QLabel(frame_9);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 100, 171, 16));
        label_23 = new QLabel(frame_9);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 130, 171, 16));
        debuggerPathLineEdit = new QLineEdit(frame_9);
        debuggerPathLineEdit->setObjectName(QString::fromUtf8("debuggerPathLineEdit"));
        debuggerPathLineEdit->setGeometry(QRect(200, 10, 101, 20));
        builderPathLineEdit = new QLineEdit(frame_9);
        builderPathLineEdit->setObjectName(QString::fromUtf8("builderPathLineEdit"));
        builderPathLineEdit->setGeometry(QRect(200, 40, 101, 20));
        codeFileNameLineEdit = new QLineEdit(frame_9);
        codeFileNameLineEdit->setObjectName(QString::fromUtf8("codeFileNameLineEdit"));
        codeFileNameLineEdit->setGeometry(QRect(200, 100, 191, 20));
        buildedFileNameLineEdit = new QLineEdit(frame_9);
        buildedFileNameLineEdit->setObjectName(QString::fromUtf8("buildedFileNameLineEdit"));
        buildedFileNameLineEdit->setGeometry(QRect(200, 130, 191, 20));
        browseDebPathButton = new QPushButton(frame_9);
        browseDebPathButton->setObjectName(QString::fromUtf8("browseDebPathButton"));
        browseDebPathButton->setGeometry(QRect(310, 10, 81, 23));
        builderPathButton = new QPushButton(frame_9);
        builderPathButton->setObjectName(QString::fromUtf8("builderPathButton"));
        builderPathButton->setGeometry(QRect(310, 40, 81, 23));
        label_25 = new QLabel(frame_9);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(10, 70, 171, 16));
        workDirLineEdit = new QLineEdit(frame_9);
        workDirLineEdit->setObjectName(QString::fromUtf8("workDirLineEdit"));
        workDirLineEdit->setGeometry(QRect(200, 70, 101, 20));
        workDirPushButton = new QPushButton(frame_9);
        workDirPushButton->setObjectName(QString::fromUtf8("workDirPushButton"));
        workDirPushButton->setGeometry(QRect(310, 70, 81, 23));
        frame_7 = new QFrame(DebuggerForm);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setGeometry(QRect(10, 30, 400, 71));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        label_14 = new QLabel(frame_7);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 10, 171, 16));
        timeoutLineEdit = new QLineEdit(frame_7);
        timeoutLineEdit->setObjectName(QString::fromUtf8("timeoutLineEdit"));
        timeoutLineEdit->setGeometry(QRect(190, 10, 71, 20));
        label_15 = new QLabel(frame_7);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 40, 171, 16));
        colorComboBox = new QComboBox(frame_7);
        colorComboBox->setObjectName(QString::fromUtf8("colorComboBox"));
        colorComboBox->setGeometry(QRect(190, 40, 71, 22));
        label_16 = new QLabel(DebuggerForm);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 10, 400, 16));
        label_24 = new QLabel(DebuggerForm);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(10, 110, 400, 16));

        retranslateUi(DebuggerForm);

        QMetaObject::connectSlotsByName(DebuggerForm);
    } // setupUi

    void retranslateUi(QWidget *DebuggerForm)
    {
        DebuggerForm->setWindowTitle(QApplication::translate("DebuggerForm", "Form", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("DebuggerForm", "Debugger path:", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("DebuggerForm", "Builder path:", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("DebuggerForm", "Generated code file name:", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("DebuggerForm", "Builded file name:", 0, QApplication::UnicodeUTF8));
        browseDebPathButton->setText(QApplication::translate("DebuggerForm", "Browse...", 0, QApplication::UnicodeUTF8));
        builderPathButton->setText(QApplication::translate("DebuggerForm", "Browse...", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("DebuggerForm", "Working directory:", 0, QApplication::UnicodeUTF8));
        workDirPushButton->setText(QApplication::translate("DebuggerForm", "Browse...", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("DebuggerForm", "Debug timeout (ms):", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("DebuggerForm", "Color of highlight:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("DebuggerForm", "Presentation", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("DebuggerForm", "Configuration", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DebuggerForm: public Ui_DebuggerForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDEBUGGERFORM_H
