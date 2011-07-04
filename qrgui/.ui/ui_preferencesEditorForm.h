/********************************************************************************
** Form generated from reading UI file 'preferencesEditorForm.ui'
**
** Created: Fri Jul 1 16:43:01 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESEDITORFORM_H
#define UI_PREFERENCESEDITORFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorForm
{
public:
    QLabel *label_2;
    QFrame *frame_2;
    QSlider *embeddedLinkerSizeSlider;
    QLabel *label_3;
    QSlider *embeddedLinkerIndentSlider;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_19;
    QFrame *frame_8;
    QLabel *label_17;
    QSlider *zoomFactorSlider;
    QFrame *frame;
    QCheckBox *showGridCheckBox;
    QCheckBox *activateGridCheckBox;
    QSlider *gridWidthSlider;
    QSlider *indexGridSlider;
    QLabel *label_13;
    QLabel *label_20;
    QCheckBox *activateAlignmentCheckBox;
    QCheckBox *showAlignmentCheckBox;

    void setupUi(QWidget *EditorForm)
    {
        if (EditorForm->objectName().isEmpty())
            EditorForm->setObjectName(QString::fromUtf8("EditorForm"));
        EditorForm->resize(420, 300);
        label_2 = new QLabel(EditorForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 150, 201, 16));
        frame_2 = new QFrame(EditorForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 170, 400, 51));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        embeddedLinkerSizeSlider = new QSlider(frame_2);
        embeddedLinkerSizeSlider->setObjectName(QString::fromUtf8("embeddedLinkerSizeSlider"));
        embeddedLinkerSizeSlider->setGeometry(QRect(10, 10, 241, 16));
        embeddedLinkerSizeSlider->setMinimum(4);
        embeddedLinkerSizeSlider->setMaximum(20);
        embeddedLinkerSizeSlider->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(263, 10, 111, 16));
        embeddedLinkerIndentSlider = new QSlider(frame_2);
        embeddedLinkerIndentSlider->setObjectName(QString::fromUtf8("embeddedLinkerIndentSlider"));
        embeddedLinkerIndentSlider->setGeometry(QRect(10, 30, 241, 16));
        embeddedLinkerIndentSlider->setMinimum(5);
        embeddedLinkerIndentSlider->setMaximum(20);
        embeddedLinkerIndentSlider->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(263, 30, 111, 16));
        label = new QLabel(EditorForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 211, 16));
        label_19 = new QLabel(EditorForm);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(20, 230, 191, 16));
        frame_8 = new QFrame(EditorForm);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setGeometry(QRect(10, 250, 400, 41));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        label_17 = new QLabel(frame_8);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(223, 10, 151, 16));
        zoomFactorSlider = new QSlider(frame_8);
        zoomFactorSlider->setObjectName(QString::fromUtf8("zoomFactorSlider"));
        zoomFactorSlider->setGeometry(QRect(10, 10, 201, 16));
        zoomFactorSlider->setMinimum(0);
        zoomFactorSlider->setMaximum(10);
        zoomFactorSlider->setPageStep(1);
        zoomFactorSlider->setValue(4);
        zoomFactorSlider->setOrientation(Qt::Horizontal);
        frame = new QFrame(EditorForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 30, 400, 111));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        showGridCheckBox = new QCheckBox(frame);
        showGridCheckBox->setObjectName(QString::fromUtf8("showGridCheckBox"));
        showGridCheckBox->setGeometry(QRect(10, 10, 186, 20));
        showGridCheckBox->setChecked(true);
        activateGridCheckBox = new QCheckBox(frame);
        activateGridCheckBox->setObjectName(QString::fromUtf8("activateGridCheckBox"));
        activateGridCheckBox->setGeometry(QRect(205, 10, 186, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(activateGridCheckBox->sizePolicy().hasHeightForWidth());
        activateGridCheckBox->setSizePolicy(sizePolicy1);
        activateGridCheckBox->setTristate(false);
        gridWidthSlider = new QSlider(frame);
        gridWidthSlider->setObjectName(QString::fromUtf8("gridWidthSlider"));
        gridWidthSlider->setGeometry(QRect(10, 70, 241, 19));
        gridWidthSlider->setMinimum(3);
        gridWidthSlider->setMaximum(150);
        gridWidthSlider->setSingleStep(1);
        gridWidthSlider->setPageStep(10);
        gridWidthSlider->setOrientation(Qt::Horizontal);
        indexGridSlider = new QSlider(frame);
        indexGridSlider->setObjectName(QString::fromUtf8("indexGridSlider"));
        indexGridSlider->setGeometry(QRect(10, 90, 241, 19));
        indexGridSlider->setMinimum(5);
        indexGridSlider->setMaximum(150);
        indexGridSlider->setSingleStep(1);
        indexGridSlider->setPageStep(10);
        indexGridSlider->setOrientation(Qt::Horizontal);
        label_13 = new QLabel(frame);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(263, 70, 111, 16));
        label_20 = new QLabel(frame);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(263, 90, 111, 16));
        activateAlignmentCheckBox = new QCheckBox(frame);
        activateAlignmentCheckBox->setObjectName(QString::fromUtf8("activateAlignmentCheckBox"));
        activateAlignmentCheckBox->setGeometry(QRect(205, 40, 186, 17));
        sizePolicy1.setHeightForWidth(activateAlignmentCheckBox->sizePolicy().hasHeightForWidth());
        activateAlignmentCheckBox->setSizePolicy(sizePolicy1);
        activateAlignmentCheckBox->setChecked(true);
        showAlignmentCheckBox = new QCheckBox(frame);
        showAlignmentCheckBox->setObjectName(QString::fromUtf8("showAlignmentCheckBox"));
        showAlignmentCheckBox->setGeometry(QRect(10, 40, 186, 20));
        showAlignmentCheckBox->setMinimumSize(QSize(130, 0));
        showAlignmentCheckBox->setChecked(true);

        retranslateUi(EditorForm);

        QMetaObject::connectSlotsByName(EditorForm);
    } // setupUi

    void retranslateUi(QWidget *EditorForm)
    {
        EditorForm->setWindowTitle(QApplication::translate("EditorForm", "Form", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EditorForm", "Embedded Linkers", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("EditorForm", "Size", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("EditorForm", "Indent", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EditorForm", "Grid", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("EditorForm", "Zoom", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("EditorForm", "Zoom factor", 0, QApplication::UnicodeUTF8));
        showGridCheckBox->setText(QApplication::translate("EditorForm", "Show grid", 0, QApplication::UnicodeUTF8));
        activateGridCheckBox->setText(QApplication::translate("EditorForm", "Activate grid", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("EditorForm", "Width", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("EditorForm", "Cell size", 0, QApplication::UnicodeUTF8));
        activateAlignmentCheckBox->setText(QApplication::translate("EditorForm", "Activate alignment", 0, QApplication::UnicodeUTF8));
        showAlignmentCheckBox->setText(QApplication::translate("EditorForm", "Show alignment", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditorForm: public Ui_EditorForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESEDITORFORM_H
