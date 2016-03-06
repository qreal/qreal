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
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtWidgets/QButtonGroup>

#include <qrutils/graphicsUtils/abstractItemView.h>
#include <qrkernel/ids.h>

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/item.h"
#include "plugins/pluginManager/editorManagerInterface.h"

// TODO: lolwut?
#include "models/details/logicalModel.h"

#include "mainWindow/shapeEdit/visibilityConditionsDialog.h"
#include "editor/editorView.h"

namespace Ui {
class ShapeEdit;
}

namespace qReal {

class MainWindow;

class ShapeEdit : public QWidget
{
	Q_OBJECT

public:
	explicit ShapeEdit(QWidget *parent = nullptr);
	ShapeEdit(qReal::models::details::LogicalModel *model, const QPersistentModelIndex &index, const int &role
		, bool useTypedPorts);
	ShapeEdit(const Id &id, const EditorManagerInterface &editorManagerProxy
		, const qrRepo::GraphicalRepoApi &graphicalRepoApi, MainWindow *mainWindow
		, qReal::gui::editor::EditorView *editorView, bool useTypedPorts);
	~ShapeEdit();

	graphicsUtils::AbstractView* getView();
	void load(const QString &text);

signals:
	void shapeSaved(const QString &shape, const QPersistentModelIndex &index, const int &role);
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

	void visibilityButtonClicked();

	void savePicture();
	void saveToXml();
	void save();
	void open();
	void addImage(bool checked);
	void setNoPalette();
	void setItemPalette(const QPen &penItem, const QBrush &brushItem);
	void setNoFontPalette();
	void setItemFontPalette(const QPen &penItem, const QFont &fontItem, const QString &name);
	void setNoPortType();
	void setPortType(const QString &type);
	void changeTextName();
	void resetHighlightAllButtons();

private:
	Scene *mScene;  // Has ownership.
	QGraphicsItemGroup mItemGroup;
	QList<QAbstractButton *> mButtonGroup;  // Doesn't have direct ownership (owned by mUi).
	QDomDocument mDocument;
	QPoint mTopLeftPicture;
	Ui::ShapeEdit *mUi;  // Has ownership.

	// TODO: lolwut? Use assist API instead.
	qReal::models::details::LogicalModel *mModel;  // Doesn't have ownership.
	const QPersistentModelIndex mIndex;
	const int mRole;
	Id mId;
	const EditorManagerInterface *mEditorManager;  // Doesn't have ownership.
	IdList mGraphicalElements;
	MainWindow *mMainWindow;  // Doesn't have ownership.
	qReal::gui::editor::EditorView *mEditorView;  // Doesn't have ownership.

	bool mUseTypedPorts;

	void initButtonGroup();
	void initFontPalette();
	void initPalette();
	void init();

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

	void generateDom();
	void exportToXml(const QString &fileName);
	QList<QDomElement> generateGraphics();

	QMap<QString, VisibilityConditionsDialog::PropertyInfo> getProperties() const;
	QStringList getPortTypes() const;
};
}
