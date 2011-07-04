/********************************************************************************
** Form generated from reading UI file 'preferencesBehaviourForm.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESBEHAVIOURFORM_H
#define UI_PREFERENCESBEHAVIOURFORM_H

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

class Ui_BehaviourForm
{
public:
    QLabel *label_7;
    QFrame *frame_5;
    QCheckBox *paletteTabCheckBox;
    QFrame *frame_6;
    QCheckBox *diagramCreateCheckBox;
    QCheckBox *saveExitCheckBox;
    QCheckBox *chooseDiagramsToSaveCheckBox;
    QLabel *label_8;
    QFrame *frame_17;
    QCheckBox *warningWindowBox;
    QLabel *label_41;

    void setupUi(QWidget *BehaviourForm)
    {
        if (BehaviourForm->objectName().isEmpty())
            BehaviourForm->setObjectName(QString::fromUtf8("BehaviourForm"));
        BehaviourForm->resize(420, 285);
        label_7 = new QLabel(BehaviourForm);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 10, 171, 16));
        frame_5 = new QFrame(BehaviourForm);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(10, 30, 401, 41));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy);
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        paletteTabCheckBox = new QCheckBox(frame_5);
        paletteTabCheckBox->setObjectName(QString::fromUtf8("paletteTabCheckBox"));
        paletteTabCheckBox->setGeometry(QRect(10, 10, 381, 20));
        frame_6 = new QFrame(BehaviourForm);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(10, 110, 401, 81));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        diagramCreateCheckBox = new QCheckBox(frame_6);
        diagramCreateCheckBox->setObjectName(QString::fromUtf8("diagramCreateCheckBox"));
        diagramCreateCheckBox->setGeometry(QRect(10, 10, 381, 20));
        saveExitCheckBox = new QCheckBox(frame_6);
        saveExitCheckBox->setObjectName(QString::fromUtf8("saveExitCheckBox"));
        saveExitCheckBox->setGeometry(QRect(10, 30, 381, 20));
        chooseDiagramsToSaveCheckBox = new QCheckBox(frame_6);
        chooseDiagramsToSaveCheckBox->setObjectName(QString::fromUtf8("chooseDiagramsToSaveCheckBox"));
        chooseDiagramsToSaveCheckBox->setGeometry(QRect(10, 50, 381, 20));
        label_8 = new QLabel(BehaviourForm);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 90, 201, 16));
        frame_17 = new QFrame(BehaviourForm);
        frame_17->setObjectName(QString::fromUtf8("frame_17"));
        frame_17->setGeometry(QRect(10, 230, 401, 41));
        frame_17->setFrameShape(QFrame::StyledPanel);
        frame_17->setFrameShadow(QFrame::Raised);
        warningWindowBox = new QCheckBox(frame_17);
        warningWindowBox->setObjectName(QString::fromUtf8("warningWindowBox"));
        warningWindowBox->setGeometry(QRect(10, 10, 381, 20));
        label_41 = new QLabel(BehaviourForm);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(10, 210, 211, 16));

        retranslateUi(BehaviourForm);

        QMetaObject::connectSlotsByName(BehaviourForm);
    } // setupUi

    void retranslateUi(QWidget *BehaviourForm)
    {
        BehaviourForm->setWindowTitle(QApplication::translate("BehaviourForm", "Form", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("BehaviourForm", "Automatics", 0, QApplication::UnicodeUTF8));
        paletteTabCheckBox->setText(QApplication::translate("BehaviourForm", "Palette tab switching", 0, QApplication::UnicodeUTF8));
        diagramCreateCheckBox->setText(QApplication::translate("BehaviourForm", "New diagram creating", 0, QApplication::UnicodeUTF8));
        saveExitCheckBox->setText(QApplication::translate("BehaviourForm", "Save on exit", 0, QApplication::UnicodeUTF8));
        chooseDiagramsToSaveCheckBox->setText(QApplication::translate("BehaviourForm", "Choose diagrams to save", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("BehaviourForm", "Suggestions", 0, QApplication::UnicodeUTF8));
        warningWindowBox->setText(QApplication::translate("BehaviourForm", "Show window", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("BehaviourForm", "Warning window", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BehaviourForm: public Ui_BehaviourForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESBEHAVIOURFORM_H
