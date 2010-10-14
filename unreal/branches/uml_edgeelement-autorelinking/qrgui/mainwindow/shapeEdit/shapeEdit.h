#pragma once

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include "view.h"
#include "scene.h"
#include "item.h"

namespace Ui {
	class ShapeEdit;
}

class ShapeEdit : public QWidget {
	Q_OBJECT
public:
	explicit ShapeEdit(QWidget *parent = NULL);
	ShapeEdit(QPersistentModelIndex const &index, int const &role);
	View* getView();
	void load(const QString &text);
	~ShapeEdit();
signals:
	void shapeSaved(QString const &shape, QPersistentModelIndex const &index, int const &role);
	void saveSignal();
	void saveToXmlSignal();
	void openSignal();

protected:
	void changeEvent(QEvent *e);
	virtual void keyPressEvent(QKeyEvent *event);

private slots:
	void savePicture();
	void saveToXml();
	void save();
	void open();
	void setNoPalette();
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);
	void setNoFontPalette();
	void setItemFontPalette(QPen const &penItem, QFont const &fontItem, QString const &name);
	void changeTextName();

private:
	Scene *mScene;
	QGraphicsItemGroup mItemGroup;
	QDomDocument mDocument;
	QPoint mTopLeftPicture;
	Ui::ShapeEdit *mUi;
	QPersistentModelIndex const mIndex;
	int const mRole;
	void initFontPalette();
	void initPalette();
	void init();

	void setValuePenStyleComboBox(Qt::PenStyle penStyle);
	void setValuePenColorComboBox(QColor penColor);
	void setValuePenWidthSpinBox(int width);
	void setValueBrushStyleComboBox(Qt::BrushStyle brushStyle);
	void setValueBrushColorComboBox(QColor brushColor);

	void setValueTextFamilyFontComboBox(QFont const &fontItem);
	void setValueTextPixelSizeSpinBox(int size);
	void setValueTextColorComboBox(QColor penColor);
	void setValueItalicCheckBox(bool check);
	void setValueBoldCheckBox(bool check);
	void setValueUnderlineCheckBox(bool check);
	void setValueTextNameLineEdit(QString const& name);

	void generateDom();
	void exportToXml(QString const &fileName);
	QList<QDomElement> generateGraphics();
};
