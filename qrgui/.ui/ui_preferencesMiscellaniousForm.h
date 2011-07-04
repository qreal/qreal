/********************************************************************************
** Form generated from reading UI file 'preferencesMiscellaniousForm.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESMISCELLANIOUSFORM_H
#define UI_PREFERENCESMISCELLANIOUSFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MiscellaniousForm
{
public:
    QFrame *frame_4;
    QCheckBox *splashScreenCheckBox;
    QCheckBox *chaoticEditionCheckBox;
    QFrame *frame_3;
    QCheckBox *antialiasingCheckBox;
    QCheckBox *openGLCheckBox;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QWidget *MiscellaniousForm)
    {
        if (MiscellaniousForm->objectName().isEmpty())
            MiscellaniousForm->setObjectName(QString::fromUtf8("MiscellaniousForm"));
        MiscellaniousForm->resize(420, 194);
        frame_4 = new QFrame(MiscellaniousForm);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(10, 120, 400, 61));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        splashScreenCheckBox = new QCheckBox(frame_4);
        splashScreenCheckBox->setObjectName(QString::fromUtf8("splashScreenCheckBox"));
        splashScreenCheckBox->setGeometry(QRect(10, 10, 380, 20));
        chaoticEditionCheckBox = new QCheckBox(frame_4);
        chaoticEditionCheckBox->setObjectName(QString::fromUtf8("chaoticEditionCheckBox"));
        chaoticEditionCheckBox->setGeometry(QRect(10, 30, 380, 20));
        frame_3 = new QFrame(MiscellaniousForm);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(10, 30, 400, 61));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        antialiasingCheckBox = new QCheckBox(frame_3);
        antialiasingCheckBox->setObjectName(QString::fromUtf8("antialiasingCheckBox"));
        antialiasingCheckBox->setGeometry(QRect(10, 10, 380, 20));
        openGLCheckBox = new QCheckBox(frame_3);
        openGLCheckBox->setObjectName(QString::fromUtf8("openGLCheckBox"));
        openGLCheckBox->setGeometry(QRect(10, 30, 380, 20));
        label_5 = new QLabel(MiscellaniousForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 10, 141, 16));
        label_6 = new QLabel(MiscellaniousForm);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 100, 151, 16));

        retranslateUi(MiscellaniousForm);

        QMetaObject::connectSlotsByName(MiscellaniousForm);
    } // setupUi

    void retranslateUi(QWidget *MiscellaniousForm)
    {
        MiscellaniousForm->setWindowTitle(QApplication::translate("MiscellaniousForm", "Form", 0, QApplication::UnicodeUTF8));
        splashScreenCheckBox->setText(QApplication::translate("MiscellaniousForm", "Show splashscreen", 0, QApplication::UnicodeUTF8));
        chaoticEditionCheckBox->setText(QApplication::translate("MiscellaniousForm", "Chaotic Edition", 0, QApplication::UnicodeUTF8));
        antialiasingCheckBox->setText(QApplication::translate("MiscellaniousForm", "Antialiasing", 0, QApplication::UnicodeUTF8));
        openGLCheckBox->setText(QApplication::translate("MiscellaniousForm", "OpenGL rendering", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MiscellaniousForm", "Graphics", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MiscellaniousForm", "Other", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MiscellaniousForm: public Ui_MiscellaniousForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESMISCELLANIOUSFORM_H
