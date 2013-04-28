#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtWidgets/QButtonGroup>

#include "scene.h"
#include "item.h"
#include "../common/controlButtons.h"
#include "../common/templateDocumentBuilder.h"
#include "../../../qrutils/graphicsUtils/abstractItemView.h"
#include "../../../qrutils/navigationUtils/navigationPageWithMenu.h"

namespace Ui {
	class ShapeEdit;
}

class ShapeEdit : public QWidget, public navigation::NavigationPageWithMenu
{
	Q_OBJECT
public:
	ShapeEdit(bool isIconEditor, QWidget *parent = NULL);
	~ShapeEdit();

	graphicsUtils::AbstractView *getView();
	void load(QString const &text);
	void load(QDomDocument const &document);
	QDomDocument currentShape();
	qReal::elementEdit::ControlButtons *controlButtons() const;

signals:
	void shapeSaved();
	void saveSignal();
	void saveToXmlSignal();
	void openSignal();
	void switchToWidgetsEditor(QDomDocument const &document);

protected:
	void onShown(navigation::NavigationState *state);
	void changeEvent(QEvent *e);
	virtual void keyPressEvent(QKeyEvent *event);

private slots:
	void drawLine(bool checked);
	void drawEllipse(bool checked);
	void drawCurve(bool checked);
	void drawRectangle(bool checked);
	void addText(bool checked);
	void addDynamicText(bool checked);
	void addTextPicture(bool checked);
	void addPointPort(bool checked);
	void addLinePort(bool checked);
	void addStylus(bool checked);

	void savePicture();
	void saveToXml();
	void save();
	void open();
	void switchToWidgets();

	void addImage(bool checked);
	void setNoPalette();
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);
	void setNoFontPalette();
	void setItemFontPalette(QPen const &penItem, QFont const &fontItem, QString const &name);
	void changeTextName();
	void resetHighlightAllButtons();

private:
	void initButtonGroup();
	void initFontPalette();
	void initPalette();
	void initControlButtons();
	void init();

	void setHighlightOneButton(QAbstractButton *oneButton);

	void setValuePenStyleComboBox(Qt::PenStyle const penStyle);
	void setValuePenColorComboBox(QColor const &penColor);
	void setValuePenWidthSpinBox(int width);
	void setValueBrushStyleComboBox(Qt::BrushStyle brushStyle);
	void setValueBrushColorComboBox(QColor const &brushColor);

	void setValueTextFamilyFontComboBox(QFont const &fontItem);
	void setValueTextPixelSizeSpinBox(int size);
	void setValueTextColorComboBox(QColor const &penColor);
	void setValueItalicCheckBox(bool check);
	void setValueBoldCheckBox(bool check);
	void setValueUnderlineCheckBox(bool check);
	void setValueTextNameLineEdit(QString const& name);

	void generateDom();
	void exportToXml(QString const &fileName);
	QList<QDomElement> generateGraphics();

	bool mIsIconEditor;
	Scene *mScene;
	QGraphicsItemGroup mItemGroup;
	QList<QAbstractButton *> mButtonGroup;
	QDomDocument mDocument;
	QPoint mTopLeftPicture;
	Ui::ShapeEdit *mUi;
	qReal::elementEdit::ControlButtons *mControlButtons;
	qReal::elementEdit::TemplateDocumentBuilder *mDocumentBuilder;
};
