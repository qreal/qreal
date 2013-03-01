#pragma once

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtGui/QButtonGroup>

#include "scene.h"
#include "item.h"
#include "../../../qrutils/graphicsUtils/abstractItemView.h"
#include "../../qrkernel/ids.h"
#include "../pluginManager/editorManagerInterface.h"
#include "ui_mainWindow.h"

namespace Ui {
	class ShapeEdit;
}

namespace qReal {

class ShapeEdit : public QWidget {
	Q_OBJECT
public:
	explicit ShapeEdit(QWidget *parent = NULL);
	ShapeEdit(QPersistentModelIndex const &index, int const &role);
	ShapeEdit(Id const &id, EditorManagerInterface *editorManagerProxy, qrRepo::GraphicalRepoApi const &graphicalRepoApi, MainWindow *mainWindow, EditorView *editorView);
	graphicsUtils::AbstractView* getView();
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
	void addImage(bool checked);
	void setNoPalette();
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);
	void setNoFontPalette();
	void setItemFontPalette(QPen const &penItem, QFont const &fontItem, QString const &name);
	void changeTextName();
	void resetHighlightAllButtons();

private:
	Scene *mScene;
	QGraphicsItemGroup mItemGroup;
	QList<QAbstractButton *> mButtonGroup;
	QDomDocument mDocument;
	QPoint mTopLeftPicture;
	Ui::ShapeEdit *mUi;
	QPersistentModelIndex const mIndex;
	int const mRole;
	Id mId;
	EditorManagerInterface *mEditorManagerProxy;
	IdList mGraphicalElements;
	MainWindow *mMainWindow;
	EditorView *mEditorView;
	void initButtonGroup();
	void initFontPalette();
	void initPalette();
	void init();

	void setHighlightOneButton(QAbstractButton *oneButton);

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
}
