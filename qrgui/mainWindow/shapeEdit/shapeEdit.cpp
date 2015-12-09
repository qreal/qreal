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

#include "shapeEdit.h"
#include "ui_shapeEdit.h"

#include <QtWidgets/QGraphicsItem>
#include <QtCore/QList>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtGui/QImage>
#include <QtWidgets/QMessageBox>

#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/qRealFileDialog.h>
#include <qrutils/widgets/colorListEditor.h>

#include "mainWindow/shapeEdit/item/createItemPushButton.h"
#include "mainWindow/shapeEdit/saveLoadLogicUsingModel.h"
#include "mainWindow/shapeEdit/saveLoadLogicForInterpreter.h"

using namespace qReal;
using namespace qReal::shapeEdit;
using namespace utils;

ShapeEdit::ShapeEdit(Controller *controller)
        : mUi(new Ui::ShapeEdit)
{
    mUi->setupUi(this);
    mScene = new Scene(mUi->graphicsView, controller, this);

    mId = Id::createElementId("", mDiagramId, "");
    Q_ASSERT(mId.isNull());

    mSaveLoadLogic = new SaveLoadLogic(this, mScene);

    init();
    connect(this, SIGNAL(saveSignal()), this, SLOT(saveToXml()));
}

ShapeEdit::ShapeEdit(const QString &propertyValue
        , models::LogicalModelAssistApi &modelApi
        , const QPersistentModelIndex &index
        , const int &role
        , Controller *controller
        , bool isUsingTypedPorts)
        : mUi(new Ui::ShapeEdit)
{
    mUi->setupUi(this);
    mScene = new Scene(mUi->graphicsView, controller, this);

    Id tmpId = modelApi.idByIndex(index);
    mId = Id::createElementId(tmpId.editor(), mDiagramId, tmpId.element());

    mSaveLoadLogic = new SaveLoadLogicUsingModel(this, mScene, modelApi, index, role, isUsingTypedPorts);

    init();
    mUi->saveButton->setEnabled(true);
    connect(this, SIGNAL(saveSignal()), this, SLOT(save()));
    if (!propertyValue.isEmpty()) {
        mSaveLoadLogic->loadFromText(propertyValue);
    }
}

ShapeEdit::ShapeEdit(const QString &propertyValue
        , const Id &id
        , const EditorManagerInterface &editorManager
        , const qrRepo::GraphicalRepoApi &graphicalRepoApi
        , EditorView *editorView
        , Controller *controller
        , bool isUsingTypedPorts)
        : mUi(new Ui::ShapeEdit)
{
    mUi->setupUi(this);
    mScene = new Scene(mUi->graphicsView, controller, this);

    mId = Id::createElementId(id.editor(), mDiagramId, id.element());

    IdList graphicalElements = graphicalRepoApi.graphicalElements(Id(id.editor(), id.diagram(), id.element()));
    mSaveLoadLogic = new SaveLoadLogicForInterpreter(this, mScene, id, editorManager, graphicalElements, editorView, isUsingTypedPorts);

    init();
    mUi->saveButton->setEnabled(true);
    connect(this, SIGNAL(saveSignal()), this, SLOT(save()));
    if (!propertyValue.isEmpty()) {
        mSaveLoadLogic->loadFromText(propertyValue);
    }
}

void ShapeEdit::init()
{
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	mUi->graphicsView->setMouseTracking(true);

    SettingsListener::listen("IndexGrid", mScene, &Scene::redraw);
    SettingsListener::listen("GridWidth", mScene, &Scene::redraw);

	QStringList penStyleList = Item::getPenStyleList();
	mUi->penStyleComboBox->addItems(penStyleList);
	mUi->penWidthSpinBox->setRange(0, 13);
	mUi->penColorComboBox->setColorList(QColor::colorNames());
	mUi->penColorComboBox->setColor(QColor("black"));

	QStringList brushStyleList = Item::getBrushStyleList();
	mUi->brushStyleComboBox->addItems(brushStyleList);
	mUi->brushColorComboBox->setColorList(QColor::colorNames());
	mUi->brushColorComboBox->setColor(QColor("white"));

    mUi->portsComboBox->addItems(mSaveLoadLogic->loadPortTypes());

	mUi->textPixelSizeSpinBox->setRange(5, 72);
	initFontPalette();

    initAvailableItems();
    initItemButtons();

	connect(mUi->penStyleComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePenStyle(const QString &)));
	connect(mUi->penWidthSpinBox, SIGNAL(valueChanged(int)), mScene, SLOT(changePenWidth(int)));
	connect(mUi->penColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePenColor(const QString &)));
	connect(mUi->brushStyleComboBox, SIGNAL(activated(const QString &))
			, mScene, SLOT(changeBrushStyle(const QString &)));
	connect(mUi->brushColorComboBox, SIGNAL(activated(const QString &))
			, mScene, SLOT(changeBrushColor(const QString &)));

	connect(mUi->textFamilyFontComboBox, SIGNAL(currentFontChanged(const QFont&))
			, mScene, SLOT(changeFontFamily(const QFont&)));
	connect(mUi->textPixelSizeSpinBox, SIGNAL(valueChanged(int)), mScene, SLOT(changeFontPixelSize(int)));
	connect(mUi->textColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeFontColor(const QString &)));
	connect(mUi->textEditField, SIGNAL(textChanged()), this, SLOT(changeTextName()));
	connect(mUi->italicCheckBox, SIGNAL(toggled(bool)), mScene, SLOT(changeFontItalic(bool)));
	connect(mUi->boldCheckBox, SIGNAL(toggled(bool)), mScene, SLOT(changeFontBold(bool)));
	connect(mUi->underlineCheckBox, SIGNAL(toggled(bool)), mScene, SLOT(changeFontUnderline(bool)));

	connect(mUi->portsComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePortsType(const QString &)));

	connect(mUi->visibilityConditionsButton, SIGNAL(clicked()), this, SLOT(visibilityButtonClicked()));
	connect(mUi->deleteItemButton, SIGNAL(clicked()), mScene, SLOT(deleteItem()));
	connect(mUi->graphicsView, SIGNAL(deleteItem()), mScene, SLOT(deleteItem()));
    connect(mUi->clearButton, SIGNAL(clicked()), mScene, SLOT(clearScene()));
    connect(mUi->saveAsPictureButton, SIGNAL(clicked()), this, SLOT(saveAsPicture()));
	connect(mUi->saveToXmlButton, SIGNAL(clicked()), this, SLOT(saveToXml()));
	connect(this, SIGNAL(saveToXmlSignal()), this, SLOT(saveToXml()));
	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(mUi->openButton, SIGNAL(clicked()), this, SLOT(open()));
	connect(this, SIGNAL(openSignal()), this, SLOT(open()));

	connect(mScene, SIGNAL(noSelectedItems()), this, SLOT(setNoPalette()));
	connect(mScene, SIGNAL(existSelectedItems(const QPen &, const QBrush &))
			, this, SLOT(setItemPalette(const QPen&, const QBrush&)));
	connect(mScene, SIGNAL(resetHighlightAllButtons()), this, SLOT(resetHighlightAllButtons()));
	connect(mScene, SIGNAL(noSelectedTextPictureItems()), this, SLOT(setNoFontPalette()));
	connect(mScene, SIGNAL(existSelectedTextPictureItems(const QPen &, const QFont &, const QString &))
			, this, SLOT(setItemFontPalette(const QPen&, const QFont&, const QString &)));
	connect(mScene, SIGNAL(noSelectedPortItems()), this, SLOT(setNoPortType()));
	connect(mScene, SIGNAL(existSelectedPortItems(const QString &)), this, SLOT(setPortType(const QString &)));
}

void ShapeEdit::resetHighlightAllButtons()
{
	foreach (QAbstractButton *button, mButtonGroup) {
		button->setChecked(false);
	}
    mScene->addNone();
}

void ShapeEdit::setHighlightOneButton(QAbstractButton *oneButton)
{
	foreach (QAbstractButton *button, mButtonGroup) {
		if (button != oneButton) {
			button->setChecked(false);
		}
	}
}

void ShapeEdit::initItemButtons()
{
    const int numButtons = mAvailableItems.size();
    const int numRows = 6;
    const int numColumns = numButtons / numRows + 1;

    int i = 0;
    for (int iRow = 0; iRow < numRows; ++iRow) {
        for (int iColumn = 0; iColumn < numColumns; ++iColumn) {
            if (i >= numButtons) {
                return;
            }

            Item *item = mAvailableItems.at(i);
            ++i;
            CreateItemPushButton *itemButton = item->createButton();
            mUi->itemButtonsGridLayout->addWidget(itemButton, iRow, iColumn, 1, 1);
            connect(itemButton, SIGNAL(clickedItemButton(bool, Item *)), this, SLOT(addShapeEditItem(bool, Item *)));

            if (dynamic_cast<Image *>(item)) {
                connect(itemButton, SIGNAL(clicked(bool)), this, SLOT(addImage(bool)));
            }

            mButtonGroup.append(itemButton);
        }
    }
}

void ShapeEdit::initAvailableItems()
{
    mAvailableItems.append(new QRealEllipse(0, 0, 0, 0));
    mAvailableItems.append(new QRealRectangle(0, 0, 0, 0));
    mAvailableItems.append(new Line(0, 0, 0, 0));
    QPointF p;
    mAvailableItems.append(new Curve(p, p, p));
    mAvailableItems.append(new Stylus(0, 0, nullptr));
    mAvailableItems.append(new LinePort(0, 0, 0, 0));
    mAvailableItems.append(new PointPort(0, 0));
    mAvailableItems.append(new Text());
    mAvailableItems.append(new Text(true));
    mAvailableItems.append(new TextPicture());
    mAvailableItems.append(new Image("", 0, 0));
}

void ShapeEdit::initPalette()
{
	mUi->penStyleComboBox->setCurrentIndex(0);
	mUi->penWidthSpinBox->setValue(0);
	mUi->penColorComboBox->setColor(QColor("black"));

	mUi->brushStyleComboBox->setCurrentIndex(0);
	mUi->brushColorComboBox->setColor(QColor("white"));
}

void ShapeEdit::initFontPalette()
{
	mUi->textFamilyFontComboBox->setCurrentFont(QFont("MS Shell Dlg 2"));
	mUi->textPixelSizeSpinBox->setValue(15);

	mUi->textColorComboBox->setColorList(QColor::colorNames());
	mUi->textColorComboBox->setColor(QColor("black"));

	mUi->textEditField->setPlainText("text");
	mUi->italicCheckBox->setChecked(false);
	mUi->boldCheckBox->setChecked(false);
	mUi->underlineCheckBox->setChecked(false);
}

ShapeEdit::~ShapeEdit()
{
	delete mScene;
	delete mUi;
}

void ShapeEdit::setDrawSceneGrid(bool show)
{
    mScene->setNeedDrawGrid(show);
    mScene->invalidate();
}

graphicsUtils::AbstractView* ShapeEdit::getView() const
{
	return mUi->graphicsView;
}

Id ShapeEdit::getId() const
{
    return mId;
}

void ShapeEdit::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void ShapeEdit::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
	if (event->matches(QKeySequence::Save)) {
		emit saveToXmlSignal();
	} else if (event->key() == Qt::Key_F2) {
		emit saveSignal();
	} if (event->matches(QKeySequence::Open)) {
		emit openSignal();
	} if (event->matches(QKeySequence::ZoomIn)) {
		mScene->mainView()->zoomIn();
	} else if (event->matches(QKeySequence::ZoomOut)) {
		mScene->mainView()->zoomOut();
	}
}

void ShapeEdit::addShapeEditItem(bool checked, Item* newItem)
{
    mScene->addShapeEditItem(checked, newItem);
    if (checked) {
        CreateItemPushButton *buttonSender = dynamic_cast<CreateItemPushButton *>(sender());
        if (buttonSender) {
            setHighlightOneButton(buttonSender);
        }
    }
}

void ShapeEdit::addImage(bool checked)
{
    if (checked) {
        QString fileName = QRealFileDialog::getOpenFileName("OpenShapeEditorImage", this);
        if (fileName.isEmpty()) {
            return;
        }

        mScene->addImage(fileName);
    }
}

void ShapeEdit::save()
{
    mSaveLoadLogic->save();
}

void ShapeEdit::saveToXml()
{
    QString fileName = QRealFileDialog::getSaveFileName("SaveShapeEditorXml", this);
    mSaveLoadLogic->saveToXml(fileName);
}

void ShapeEdit::saveAsPicture()
{
	QString fileName = QRealFileDialog::getSaveFileName("SaveShapeEditorPicture", this);
    mSaveLoadLogic->saveAsPicture(fileName);
}

void ShapeEdit::open()
{
	QString fileName = QRealFileDialog::getOpenFileName("OpenShapeEditorXml", this);
    mSaveLoadLogic->loadFromFile(fileName);
}

void ShapeEdit::setValuePenStyleComboBox(Qt::PenStyle penStyle)
{
	if (penStyle == Qt::SolidLine) {
		mUi->penStyleComboBox->setCurrentIndex(0);
	} else if (penStyle == Qt::DotLine) {
		mUi->penStyleComboBox->setCurrentIndex(1);
	} else if (penStyle == Qt::DashLine) {
		mUi->penStyleComboBox->setCurrentIndex(2);
	} else if (penStyle == Qt::DashDotLine) {
		mUi->penStyleComboBox->setCurrentIndex(3);
	} else if (penStyle == Qt::DashDotDotLine) {
		mUi->penStyleComboBox->setCurrentIndex(4);
	} else if (penStyle == Qt::NoPen) {
		mUi->penStyleComboBox->setCurrentIndex(5);
	}
}

void ShapeEdit::setValuePenColorComboBox(const QColor &penColor)
{
	mUi->penColorComboBox->setColor(penColor);
}

void ShapeEdit::setValueBrushStyleComboBox(Qt::BrushStyle brushStyle)
{
	if (brushStyle == Qt::SolidPattern) {
		mUi->brushStyleComboBox->setCurrentIndex(1);
	} else if (brushStyle == Qt::NoBrush) {
		mUi->brushStyleComboBox->setCurrentIndex(0);
	}
}

void ShapeEdit::setValuePenWidthSpinBox(int width)
{
	mUi->penWidthSpinBox->setValue(width);
}

void ShapeEdit::setValueBrushColorComboBox(QColor brushColor)
{
	mUi->brushColorComboBox->setColor(brushColor);
}

void ShapeEdit::setItemPalette(const QPen &penItem, const QBrush &brushItem)
{
	setValuePenStyleComboBox(penItem.style());
	setValuePenWidthSpinBox(penItem.width());
	setValuePenColorComboBox(penItem.color());

	setValueBrushStyleComboBox(brushItem.style());
	setValueBrushColorComboBox(brushItem.color());
}

void ShapeEdit::setNoPalette()
{
	initPalette();
}

void ShapeEdit::setValueTextFamilyFontComboBox(const QFont &fontItem)
{
	mUi->textFamilyFontComboBox->setCurrentFont(fontItem);
}

void ShapeEdit::setValueTextPixelSizeSpinBox(int size)
{
	mUi->textPixelSizeSpinBox->setValue(size);
}

void ShapeEdit::setValueTextColorComboBox(const QColor &penColor)
{
	mUi->textColorComboBox->setColor(penColor);
}

void ShapeEdit::setValueItalicCheckBox(bool check)
{
	mUi->italicCheckBox->setChecked(check);
}

void ShapeEdit::setValueBoldCheckBox(bool check)
{
	mUi->boldCheckBox->setChecked(check);
}

void ShapeEdit::setValueUnderlineCheckBox(bool check)
{
	mUi->underlineCheckBox->setChecked(check);
}

void ShapeEdit::setValueTextNameLineEdit(const QString &name)
{
	mUi->textEditField->setPlainText(name);
}

void ShapeEdit::setItemFontPalette(const QPen &penItem, const QFont &fontItem, const QString &name)
{
	mUi->fontToolBox->setEnabled(true);
	setValueTextFamilyFontComboBox(fontItem);
	setValueTextPixelSizeSpinBox(fontItem.pixelSize());
	setValueTextColorComboBox(penItem.color());
	setValueItalicCheckBox(fontItem.italic());
	setValueBoldCheckBox(fontItem.bold());
	setValueUnderlineCheckBox(fontItem.underline());
	setValueTextNameLineEdit(name);
}

void ShapeEdit::setNoFontPalette()
{
	initFontPalette();
	mUi->fontToolBox->setEnabled(false);
}

void ShapeEdit::setNoPortType()
{
	mUi->portsComboBox->setCurrentText("NonTyped");
}

void ShapeEdit::setPortType(const QString &type)
{
	mUi->portsComboBox->setCurrentText(type);
}

void ShapeEdit::changeTextName()
{
	QString newName = mUi->textEditField->toPlainText();
	mScene->changeTextName(newName);
}

void ShapeEdit::visibilityButtonClicked()
{
	QList<Item *> selectedItems = mScene->selectedShapeEditItems();
	if (selectedItems.isEmpty()) {
		return;
	}
    VisibilityConditionsDialog vcDialog(mSaveLoadLogic->loadProperties(), selectedItems);
	vcDialog.exec();
}
