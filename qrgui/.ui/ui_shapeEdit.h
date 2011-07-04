/********************************************************************************
** Form generated from reading UI file 'shapeEdit.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAPEEDIT_H
#define UI_SHAPEEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>
#include "colorlisteditor.h"
#include "view.h"

QT_BEGIN_NAMESPACE

class Ui_ShapeEdit
{
public:
    QGridLayout *gridLayout;
    QPushButton *drawLineButton;
    QPushButton *drawCurveButton;
    View *graphicsView;
    QPushButton *drawRectButton;
    QPushButton *drawEllipseButton;
    QPushButton *addTextButton;
    QPushButton *addDynamicTextButton;
    QFrame *line;
    QPushButton *stylusButton;
    QPushButton *addTextPictureButton;
    QPushButton *addLinePortButton;
    QPushButton *addPointPortButton;
    QToolBox *fontToolBox;
    QWidget *page;
    QWidget *widget_6;
    QLabel *label_6;
    QFontComboBox *textFamilyFontComboBox;
    QWidget *justSomeWidget_2;
    QSpinBox *textPixelSizeSpinBox;
    QLabel *label_7;
    QWidget *widget_7;
    ColorListEditor *textColorComboBox;
    QLabel *label_8;
    QWidget *page_3;
    QCheckBox *italicCheckBox;
    QCheckBox *boldCheckBox;
    QCheckBox *underlineCheckBox;
    QWidget *widget_8;
    QLabel *label_9;
    QTextEdit *textEditField;
    QPushButton *openButton;
    QPushButton *saveToXmlButton;
    QPushButton *saveButton;
    QPushButton *deleteItemButton;
    QPushButton *clearButton;
    QPushButton *saveAsPictureButton;
    QToolBox *penBrushToolBox;
    QWidget *widget;
    QWidget *widget_3;
    QComboBox *penStyleComboBox;
    QLabel *label_3;
    QWidget *widget_2;
    ColorListEditor *penColorComboBox;
    QLabel *label_2;
    QWidget *justSomeWidget;
    QSpinBox *penWidthSpinBox;
    QLabel *label;
    QWidget *page_2;
    QWidget *widget_4;
    QComboBox *brushStyleComboBox;
    QLabel *label_4;
    QWidget *widget_5;
    ColorListEditor *brushColorComboBox;
    QLabel *label_5;
    QPushButton *noneButton;
    QPushButton *addImageButton;

    void setupUi(QWidget *ShapeEdit)
    {
        if (ShapeEdit->objectName().isEmpty())
            ShapeEdit->setObjectName(QString::fromUtf8("ShapeEdit"));
        ShapeEdit->resize(717, 650);
        gridLayout = new QGridLayout(ShapeEdit);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        drawLineButton = new QPushButton(ShapeEdit);
        drawLineButton->setObjectName(QString::fromUtf8("drawLineButton"));
        drawLineButton->setMaximumSize(QSize(65, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawLineButton->setIcon(icon);
        drawLineButton->setIconSize(QSize(45, 16));
        drawLineButton->setCheckable(true);
        drawLineButton->setChecked(false);
        drawLineButton->setAutoExclusive(true);
        drawLineButton->setAutoDefault(false);
        drawLineButton->setDefault(false);

        gridLayout->addWidget(drawLineButton, 0, 0, 1, 1);

        drawCurveButton = new QPushButton(ShapeEdit);
        drawCurveButton->setObjectName(QString::fromUtf8("drawCurveButton"));
        drawCurveButton->setEnabled(true);
        drawCurveButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/curve.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawCurveButton->setIcon(icon1);
        drawCurveButton->setIconSize(QSize(45, 16));
        drawCurveButton->setCheckable(true);
        drawCurveButton->setChecked(false);
        drawCurveButton->setAutoExclusive(true);
        drawCurveButton->setAutoDefault(false);
        drawCurveButton->setDefault(false);

        gridLayout->addWidget(drawCurveButton, 0, 1, 1, 1);

        graphicsView = new View(ShapeEdit);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setMinimumSize(QSize(150, 2));

        gridLayout->addWidget(graphicsView, 0, 2, 17, 1);

        drawRectButton = new QPushButton(ShapeEdit);
        drawRectButton->setObjectName(QString::fromUtf8("drawRectButton"));
        drawRectButton->setMinimumSize(QSize(0, 0));
        drawRectButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/rect.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawRectButton->setIcon(icon2);
        drawRectButton->setIconSize(QSize(45, 16));
        drawRectButton->setCheckable(true);
        drawRectButton->setChecked(false);
        drawRectButton->setAutoExclusive(true);
        drawRectButton->setAutoDefault(false);
        drawRectButton->setDefault(false);

        gridLayout->addWidget(drawRectButton, 1, 0, 1, 1);

        drawEllipseButton = new QPushButton(ShapeEdit);
        drawEllipseButton->setObjectName(QString::fromUtf8("drawEllipseButton"));
        drawEllipseButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/ellipse.png"), QSize(), QIcon::Normal, QIcon::Off);
        drawEllipseButton->setIcon(icon3);
        drawEllipseButton->setIconSize(QSize(45, 16));
        drawEllipseButton->setCheckable(true);
        drawEllipseButton->setChecked(false);
        drawEllipseButton->setAutoExclusive(true);
        drawEllipseButton->setAutoDefault(false);
        drawEllipseButton->setDefault(false);

        gridLayout->addWidget(drawEllipseButton, 1, 1, 1, 1);

        addTextButton = new QPushButton(ShapeEdit);
        addTextButton->setObjectName(QString::fromUtf8("addTextButton"));
        addTextButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/staticText.png"), QSize(), QIcon::Normal, QIcon::Off);
        addTextButton->setIcon(icon4);
        addTextButton->setIconSize(QSize(45, 16));
        addTextButton->setCheckable(true);
        addTextButton->setChecked(false);
        addTextButton->setAutoExclusive(true);
        addTextButton->setAutoDefault(false);
        addTextButton->setDefault(false);

        gridLayout->addWidget(addTextButton, 2, 0, 1, 1);

        addDynamicTextButton = new QPushButton(ShapeEdit);
        addDynamicTextButton->setObjectName(QString::fromUtf8("addDynamicTextButton"));
        addDynamicTextButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/dynamicText.png"), QSize(), QIcon::Normal, QIcon::Off);
        addDynamicTextButton->setIcon(icon5);
        addDynamicTextButton->setIconSize(QSize(45, 16));
        addDynamicTextButton->setCheckable(true);
        addDynamicTextButton->setChecked(false);
        addDynamicTextButton->setAutoExclusive(true);
        addDynamicTextButton->setAutoDefault(false);
        addDynamicTextButton->setDefault(false);

        gridLayout->addWidget(addDynamicTextButton, 2, 1, 1, 1);

        line = new QFrame(ShapeEdit);
        line->setObjectName(QString::fromUtf8("line"));
        line->setEnabled(false);
        line->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        line->setFont(font);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 12, 0, 1, 2);

        stylusButton = new QPushButton(ShapeEdit);
        stylusButton->setObjectName(QString::fromUtf8("stylusButton"));
        stylusButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
        stylusButton->setIcon(icon6);
        stylusButton->setIconSize(QSize(45, 16));
        stylusButton->setCheckable(true);
        stylusButton->setChecked(false);
        stylusButton->setAutoExclusive(true);
        stylusButton->setAutoDefault(false);
        stylusButton->setDefault(false);

        gridLayout->addWidget(stylusButton, 3, 1, 1, 1);

        addTextPictureButton = new QPushButton(ShapeEdit);
        addTextPictureButton->setObjectName(QString::fromUtf8("addTextPictureButton"));
        addTextPictureButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/textPicture.png"), QSize(), QIcon::Normal, QIcon::Off);
        addTextPictureButton->setIcon(icon7);
        addTextPictureButton->setIconSize(QSize(45, 16));
        addTextPictureButton->setCheckable(true);
        addTextPictureButton->setAutoExclusive(true);

        gridLayout->addWidget(addTextPictureButton, 3, 0, 1, 1);

        addLinePortButton = new QPushButton(ShapeEdit);
        addLinePortButton->setObjectName(QString::fromUtf8("addLinePortButton"));
        addLinePortButton->setMaximumSize(QSize(65, 16777215));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/linePort.png"), QSize(), QIcon::Normal, QIcon::Off);
        addLinePortButton->setIcon(icon8);
        addLinePortButton->setIconSize(QSize(45, 16));
        addLinePortButton->setCheckable(true);
        addLinePortButton->setChecked(false);
        addLinePortButton->setAutoExclusive(true);
        addLinePortButton->setAutoDefault(false);
        addLinePortButton->setDefault(false);

        gridLayout->addWidget(addLinePortButton, 4, 0, 1, 1);

        addPointPortButton = new QPushButton(ShapeEdit);
        addPointPortButton->setObjectName(QString::fromUtf8("addPointPortButton"));
        addPointPortButton->setMaximumSize(QSize(65, 16777215));
        addPointPortButton->setIcon(icon3);
        addPointPortButton->setIconSize(QSize(6, 6));
        addPointPortButton->setCheckable(true);
        addPointPortButton->setChecked(false);
        addPointPortButton->setAutoExclusive(true);
        addPointPortButton->setAutoDefault(false);
        addPointPortButton->setDefault(false);

        gridLayout->addWidget(addPointPortButton, 4, 1, 1, 1);

        fontToolBox = new QToolBox(ShapeEdit);
        fontToolBox->setObjectName(QString::fromUtf8("fontToolBox"));
        fontToolBox->setEnabled(false);
        fontToolBox->setMinimumSize(QSize(0, 135));
        fontToolBox->setMaximumSize(QSize(16777215, 150));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 130, 81));
        widget_6 = new QWidget(page);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        widget_6->setGeometry(QRect(-10, 0, 151, 21));
        label_6 = new QLabel(widget_6);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 0, 41, 16));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_6->setFont(font1);
        textFamilyFontComboBox = new QFontComboBox(widget_6);
        textFamilyFontComboBox->setObjectName(QString::fromUtf8("textFamilyFontComboBox"));
        textFamilyFontComboBox->setGeometry(QRect(47, 0, 92, 22));
        justSomeWidget_2 = new QWidget(page);
        justSomeWidget_2->setObjectName(QString::fromUtf8("justSomeWidget_2"));
        justSomeWidget_2->setGeometry(QRect(0, 30, 141, 21));
        textPixelSizeSpinBox = new QSpinBox(justSomeWidget_2);
        textPixelSizeSpinBox->setObjectName(QString::fromUtf8("textPixelSizeSpinBox"));
        textPixelSizeSpinBox->setGeometry(QRect(40, 0, 88, 21));
        label_7 = new QLabel(justSomeWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(0, 0, 31, 16));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignCenter);
        widget_7 = new QWidget(page);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        widget_7->setGeometry(QRect(0, 60, 141, 21));
        textColorComboBox = new ColorListEditor(widget_7);
        textColorComboBox->setObjectName(QString::fromUtf8("textColorComboBox"));
        textColorComboBox->setGeometry(QRect(40, 0, 88, 20));
        label_8 = new QLabel(widget_7);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(0, 0, 31, 16));
        label_8->setFont(font1);
        label_8->setScaledContents(false);
        label_8->setAlignment(Qt::AlignCenter);
        fontToolBox->addItem(page, QString::fromUtf8("Font"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setGeometry(QRect(0, 0, 98, 28));
        italicCheckBox = new QCheckBox(page_3);
        italicCheckBox->setObjectName(QString::fromUtf8("italicCheckBox"));
        italicCheckBox->setGeometry(QRect(0, 0, 51, 17));
        boldCheckBox = new QCheckBox(page_3);
        boldCheckBox->setObjectName(QString::fromUtf8("boldCheckBox"));
        boldCheckBox->setGeometry(QRect(60, 0, 51, 17));
        underlineCheckBox = new QCheckBox(page_3);
        underlineCheckBox->setObjectName(QString::fromUtf8("underlineCheckBox"));
        underlineCheckBox->setGeometry(QRect(0, 20, 71, 17));
        widget_8 = new QWidget(page_3);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        widget_8->setGeometry(QRect(0, 40, 121, 41));
        label_9 = new QLabel(widget_8);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(0, 10, 31, 16));
        textEditField = new QTextEdit(widget_8);
        textEditField->setObjectName(QString::fromUtf8("textEditField"));
        textEditField->setGeometry(QRect(30, 0, 91, 41));
        fontToolBox->addItem(page_3, QString::fromUtf8("Text format"));

        gridLayout->addWidget(fontToolBox, 6, 0, 2, 2);

        openButton = new QPushButton(ShapeEdit);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        gridLayout->addWidget(openButton, 16, 0, 1, 2);

        saveToXmlButton = new QPushButton(ShapeEdit);
        saveToXmlButton->setObjectName(QString::fromUtf8("saveToXmlButton"));

        gridLayout->addWidget(saveToXmlButton, 14, 0, 1, 2);

        saveButton = new QPushButton(ShapeEdit);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setEnabled(false);

        gridLayout->addWidget(saveButton, 15, 0, 1, 2);

        deleteItemButton = new QPushButton(ShapeEdit);
        deleteItemButton->setObjectName(QString::fromUtf8("deleteItemButton"));
        deleteItemButton->setMaximumSize(QSize(64, 16777215));

        gridLayout->addWidget(deleteItemButton, 11, 0, 1, 1);

        clearButton = new QPushButton(ShapeEdit);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(clearButton, 11, 1, 1, 1);

        saveAsPictureButton = new QPushButton(ShapeEdit);
        saveAsPictureButton->setObjectName(QString::fromUtf8("saveAsPictureButton"));

        gridLayout->addWidget(saveAsPictureButton, 13, 0, 1, 2);

        penBrushToolBox = new QToolBox(ShapeEdit);
        penBrushToolBox->setObjectName(QString::fromUtf8("penBrushToolBox"));
        penBrushToolBox->setEnabled(true);
        penBrushToolBox->setMinimumSize(QSize(130, 130));
        penBrushToolBox->setMaximumSize(QSize(142, 150));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Times New Roman"));
        font2.setPointSize(8);
        font2.setBold(false);
        font2.setWeight(50);
        font2.setKerning(true);
        penBrushToolBox->setFont(font2);
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 130, 94));
        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(0, 0, 141, 21));
        penStyleComboBox = new QComboBox(widget_3);
        penStyleComboBox->setObjectName(QString::fromUtf8("penStyleComboBox"));
        penStyleComboBox->setGeometry(QRect(30, 0, 98, 21));
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(true);
        label_3->setGeometry(QRect(0, 0, 28, 16));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignCenter);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(0, 60, 141, 21));
        penColorComboBox = new ColorListEditor(widget_2);
        penColorComboBox->setObjectName(QString::fromUtf8("penColorComboBox"));
        penColorComboBox->setGeometry(QRect(30, 0, 98, 20));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, -60, 29, 142));
        label_2->setMaximumSize(QSize(64, 16777215));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);
        justSomeWidget = new QWidget(widget);
        justSomeWidget->setObjectName(QString::fromUtf8("justSomeWidget"));
        justSomeWidget->setGeometry(QRect(0, 30, 141, 21));
        penWidthSpinBox = new QSpinBox(justSomeWidget);
        penWidthSpinBox->setObjectName(QString::fromUtf8("penWidthSpinBox"));
        penWidthSpinBox->setGeometry(QRect(30, 0, 98, 21));
        label = new QLabel(justSomeWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 31, 16));
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        penBrushToolBox->addItem(widget, QString::fromUtf8("Pen"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 98, 28));
        widget_4 = new QWidget(page_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(0, 0, 141, 21));
        brushStyleComboBox = new QComboBox(widget_4);
        brushStyleComboBox->setObjectName(QString::fromUtf8("brushStyleComboBox"));
        brushStyleComboBox->setGeometry(QRect(30, 0, 101, 20));
        label_4 = new QLabel(widget_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 0, 31, 16));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignCenter);
        widget_5 = new QWidget(page_2);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(0, 30, 141, 21));
        brushColorComboBox = new ColorListEditor(widget_5);
        brushColorComboBox->setObjectName(QString::fromUtf8("brushColorComboBox"));
        brushColorComboBox->setGeometry(QRect(30, 0, 98, 20));
        label_5 = new QLabel(widget_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 0, 31, 16));
        label_5->setFont(font1);
        label_5->setScaledContents(false);
        label_5->setAlignment(Qt::AlignCenter);
        penBrushToolBox->addItem(page_2, QString::fromUtf8("Brush"));

        gridLayout->addWidget(penBrushToolBox, 8, 0, 3, 2);

        noneButton = new QPushButton(ShapeEdit);
        noneButton->setObjectName(QString::fromUtf8("noneButton"));
        noneButton->setMaximumSize(QSize(60, 16777215));
        noneButton->setCheckable(true);
        noneButton->setChecked(true);
        noneButton->setAutoExclusive(true);

        gridLayout->addWidget(noneButton, 5, 1, 1, 1);

        addImageButton = new QPushButton(ShapeEdit);
        addImageButton->setObjectName(QString::fromUtf8("addImageButton"));
        addImageButton->setMaximumSize(QSize(60, 16777215));
        addImageButton->setCheckable(true);
        addImageButton->setAutoExclusive(true);

        gridLayout->addWidget(addImageButton, 5, 0, 1, 1);


        retranslateUi(ShapeEdit);

        fontToolBox->setCurrentIndex(0);
        penBrushToolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ShapeEdit);
    } // setupUi

    void retranslateUi(QWidget *ShapeEdit)
    {
        ShapeEdit->setWindowTitle(QApplication::translate("ShapeEdit", "Form", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        drawLineButton->setToolTip(QApplication::translate("ShapeEdit", "Draw line", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        drawLineButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        drawCurveButton->setToolTip(QApplication::translate("ShapeEdit", "Draw curve", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        drawCurveButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        drawRectButton->setToolTip(QApplication::translate("ShapeEdit", "Draw rectangle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        drawRectButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        drawEllipseButton->setToolTip(QApplication::translate("ShapeEdit", "Draw ellipse", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        drawEllipseButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addTextButton->setToolTip(QApplication::translate("ShapeEdit", "Add static text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addTextButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addDynamicTextButton->setToolTip(QApplication::translate("ShapeEdit", "Add dynamic text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addDynamicTextButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        stylusButton->setToolTip(QApplication::translate("ShapeEdit", "Stylus", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stylusButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addTextPictureButton->setToolTip(QApplication::translate("ShapeEdit", "Add picture text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addTextPictureButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addLinePortButton->setToolTip(QApplication::translate("ShapeEdit", "Add line port", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addLinePortButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addPointPortButton->setToolTip(QApplication::translate("ShapeEdit", "Add point port", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addPointPortButton->setText(QString());
        label_6->setText(QApplication::translate("ShapeEdit", "Family", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ShapeEdit", "Size", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ShapeEdit", "Color", 0, QApplication::UnicodeUTF8));
        fontToolBox->setItemText(fontToolBox->indexOf(page), QApplication::translate("ShapeEdit", "Font", 0, QApplication::UnicodeUTF8));
        italicCheckBox->setText(QApplication::translate("ShapeEdit", "Italic", 0, QApplication::UnicodeUTF8));
        boldCheckBox->setText(QApplication::translate("ShapeEdit", " Bold", 0, QApplication::UnicodeUTF8));
        underlineCheckBox->setText(QApplication::translate("ShapeEdit", "Underline", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ShapeEdit", "Name", 0, QApplication::UnicodeUTF8));
        fontToolBox->setItemText(fontToolBox->indexOf(page_3), QApplication::translate("ShapeEdit", "Text format", 0, QApplication::UnicodeUTF8));
        openButton->setText(QApplication::translate("ShapeEdit", "Open", 0, QApplication::UnicodeUTF8));
        saveToXmlButton->setText(QApplication::translate("ShapeEdit", "Save to Xml", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("ShapeEdit", "Save", 0, QApplication::UnicodeUTF8));
        deleteItemButton->setText(QApplication::translate("ShapeEdit", "Delete Item", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("ShapeEdit", "Clear", 0, QApplication::UnicodeUTF8));
        saveAsPictureButton->setText(QApplication::translate("ShapeEdit", "Save as picture", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ShapeEdit", "Style", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ShapeEdit", "Color", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ShapeEdit", "Width", 0, QApplication::UnicodeUTF8));
        penBrushToolBox->setItemText(penBrushToolBox->indexOf(widget), QApplication::translate("ShapeEdit", "Pen", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ShapeEdit", "Style", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ShapeEdit", "Color", 0, QApplication::UnicodeUTF8));
        penBrushToolBox->setItemText(penBrushToolBox->indexOf(page_2), QApplication::translate("ShapeEdit", "Brush", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        noneButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        noneButton->setText(QApplication::translate("ShapeEdit", "None", 0, QApplication::UnicodeUTF8));
        addImageButton->setText(QApplication::translate("ShapeEdit", "Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ShapeEdit: public Ui_ShapeEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAPEEDIT_H
