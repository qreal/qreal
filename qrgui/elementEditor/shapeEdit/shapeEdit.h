#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtWidgets/QButtonGroup>

#include <qrutils/graphicsUtils/abstractItemView.h>
#include <qrutils/graphicsUtils/abstractItemView.h>
#include <qrutils/navigationUtils/navigationPageWithMenu.h>
#include <qrkernel/ids.h>

#include "elementEditor/shapeEdit/scene.h"
#include "elementEditor/shapeEdit/item.h"
#include "elementEditor/common/controlButtons.h"
#include "elementEditor/common/templateDocumentBuilder.h"
#include "pluginManager/editorManagerInterface.h"

#include "models/details/logicalModel.h"

#include "elementEditor/shapeEdit/visibilityConditionsDialog.h"
#include "view/editorView.h"

namespace Ui {
class ShapeEdit;
}

namespace qReal {

class ShapeEdit : public QWidget, public navigation::NavigationPageWithMenu
{
	Q_OBJECT

public:

	explicit ShapeEdit(bool isIconEditor, QWidget *parent = NULL);
	ShapeEdit(qReal::models::details::LogicalModel *model, QPersistentModelIndex const &index, int const &role
		, bool isIconEditor, bool useTypedPorts);
	ShapeEdit(Id const &id, EditorManagerInterface *editorManagerProxy
		, qrRepo::GraphicalRepoApi const &graphicalRepoApi, MainWindow *mainWindow
		, EditorView *editorView, bool isIconEditor, bool useTypedPorts);
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

	void visibilityButtonClicked();

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
	void setNoPortType();
	void setPortType(QString const &type);
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
	void setValueTextNameLineEdit(QString const &name);

	void generateDom();
	void exportToXml(QString const &fileName);
	QList<QDomElement> generateGraphics();

	QMap<QString, VisibilityConditionsDialog::PropertyInfo> getProperties() const;
	QStringList getPortTypes() const;

	bool mIsIconEditor;
	Scene *mScene;  // Has ownership.
	QGraphicsItemGroup mItemGroup;
	QList<QAbstractButton *> mButtonGroup;  // Doesn't have direct ownership (owned by mUi).
	QDomDocument mDocument;
	QPoint mTopLeftPicture;
	Ui::ShapeEdit *mUi;  // Has ownership.

	// TODO: lolwut? Use assist API instead.
	qReal::models::details::LogicalModel *mModel;  // Doesn't have ownership.
	QPersistentModelIndex const mIndex;
	int const mRole;
	Id mId;
	EditorManagerInterface *mEditorManager;  // Doesn't have ownership.
	IdList mGraphicalElements;
	MainWindow *mMainWindow;  // Doesn't have ownership.
	EditorView *mEditorView;  // Doesn't have ownership.

	bool mUseTypedPorts;

	qReal::elementEdit::ControlButtons *mControlButtons;
	qReal::elementEdit::TemplateDocumentBuilder *mDocumentBuilder;
};

}
