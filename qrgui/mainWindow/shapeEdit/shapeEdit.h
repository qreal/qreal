/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QButtonGroup>
#include <QtCore/QString>
#include <QtXml/QDomDocument>

#include <qrutils/graphicsUtils/abstractItemView.h>
#include <qrkernel/ids.h>
#include <qrgui/controller/controller.h>
#include <qrkernel/settingsListener.h>

#include "editor/editorView.h"
#include "plugins/pluginManager/editorManagerInterface.h"

#include "mainWindow/shapeEdit/visibilityConditionsDialog.h"

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/item/item.h"
#include "mainWindow/shapeEdit/iShapeEdit.h"
#include "mainWindow/shapeEdit/saveLoadLogic.h"


namespace Ui {
class ShapeEdit;
}

namespace qReal {
namespace shapeEdit {

class ShapeEdit : public IShapeEdit
{
	Q_OBJECT

public:
    explicit ShapeEdit(Controller *controller);
    ShapeEdit(const QString &propertyValue
              , qReal::models::LogicalModelAssistApi &model
              , const QPersistentModelIndex &index
              , const int &role
              , Controller *controller
              , bool isUsingTypedPorts
              );
    ShapeEdit(const QString &propertyValue
              , const Id &id
              , const EditorManagerInterface &editorManagerProxy
              , const qrRepo::GraphicalRepoApi &graphicalRepoApi
              , qReal::gui::editor::EditorView *editorView
              , Controller *controller
              , bool isUsingTypedPorts
              );
    ~ShapeEdit();

    void setDrawSceneGrid(bool show);
    graphicsUtils::AbstractView* getView() const;
    Id getId() const;

signals:
    void saveSignal();
	void saveToXmlSignal();
	void openSignal();

protected:
	void changeEvent(QEvent *e);
	virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void addShapeEditItem(bool checked, Item *newItem);
    void addImage(bool checked);

	void visibilityButtonClicked();

    void open();
    void save();
    void saveToXml();
    void saveAsPicture();

    void setNoPalette();
    void setItemPalette(const QPen &penItem, const QBrush &brushItem);
    void setNoFontPalette();
    void setItemFontPalette(const QPen &penItem, const QFont &fontItem, const QString &name);
    void setNoPortType();
    void setPortType(const QString &type);
    void changeTextName();
    void resetHighlightAllButtons();

private:
    SaveLoadLogic *mSaveLoadLogic; // Has ownership
    Ui::ShapeEdit *mUi;  // Has ownership.

    const QString mDiagramId = "shapeEdit";

    QList<Item *> mAvailableItems;
    void initItemButtons();
    void initAvailableItems();

	Scene *mScene;  // Has ownership.
	QGraphicsItemGroup mItemGroup;
	QList<QAbstractButton *> mButtonGroup;  // Doesn't have direct ownership (owned by mUi).

    void init();
	void initPalette();
    void initFontPalette();

    void setHighlightOneButton(QAbstractButton *oneButton);

    void setValuePenStyleComboBox(Qt::PenStyle penStyle);
    void setValuePenColorComboBox(const QColor &penColor);
    void setValuePenWidthSpinBox(int width);
    void setValueBrushStyleComboBox(Qt::BrushStyle brushStyle);
    void setValueBrushColorComboBox(QColor brushColor);

    void setValueTextFamilyFontComboBox(const QFont &fontItem);
    void setValueTextPixelSizeSpinBox(int size);
    void setValueTextColorComboBox(const QColor &penColor);
    void setValueItalicCheckBox(bool check);
    void setValueBoldCheckBox(bool check);
    void setValueUnderlineCheckBox(bool check);
    void setValueTextNameLineEdit(const QString &name);
};

}
}
