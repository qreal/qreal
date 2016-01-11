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

#include "mainWindow/shapeEdit/xmlLoader.h"
#include "mainWindow/mainWindow.h"

using namespace qReal;
using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent)
		: QWidget(parent), mUi(new Ui::ShapeEdit), mRole(0)
{
	init();
	connect(this, SIGNAL(saveSignal()), this, SLOT(saveToXml()));
}

ShapeEdit::ShapeEdit(
	qReal::models::details::LogicalModel *model
	, const QPersistentModelIndex &index
	, const int &role
	, bool useTypedPorts
	)
	: QWidget(nullptr)
	, mUi(new Ui::ShapeEdit)
	, mModel(model)
	, mIndex(index)
	, mRole(role)
	, mUseTypedPorts(useTypedPorts)
{
	init();
	mUi->saveButton->setEnabled(true);
	connect(this, SIGNAL(saveSignal()), this, SLOT(save()));
}

ShapeEdit::ShapeEdit(
	const Id &id
	, const EditorManagerInterface &editorManager
	, const qrRepo::GraphicalRepoApi &graphicalRepoApi
	, MainWindow *mainWindow
	, qReal::gui::editor::EditorView *editorView
	, bool useTypedPorts
	)
	: QWidget(nullptr)
	, mUi(new Ui::ShapeEdit)
	, mRole(0)
	, mId(id)
	, mEditorManager(&editorManager)
	, mMainWindow(mainWindow)
	, mEditorView(editorView)
	, mUseTypedPorts(useTypedPorts)
{
	mGraphicalElements = graphicalRepoApi.graphicalElements(Id(mId.editor(), mId.diagram(), mId.element()));
	init();
	mUi->saveButton->setEnabled(true);
	connect(this, SIGNAL(saveSignal()), this, SLOT(save()));
}

void ShapeEdit::init()
{
	mUi->setupUi(this);

	mScene = new Scene(mUi->graphicsView, this);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	mUi->graphicsView->setMouseTracking(true);

	QStringList penStyleList = Item::getPenStyleList();
	mUi->penStyleComboBox->addItems(penStyleList);
	mUi->penWidthSpinBox->setRange(0, 13);
	mUi->penColorComboBox->setColorList(QColor::colorNames());
	mUi->penColorComboBox->setColor(QColor("black"));

	QStringList brushStyleList = Item::getBrushStyleList();
	mUi->brushStyleComboBox->addItems(brushStyleList);
	mUi->brushColorComboBox->setColorList(QColor::colorNames());
	mUi->brushColorComboBox->setColor(QColor("white"));

	mUi->portsComboBox->addItems(getPortTypes());

	mUi->textPixelSizeSpinBox->setRange(5, 72);
	initFontPalette();

	initButtonGroup();
	connect(mUi->drawLineButton, SIGNAL(clicked(bool)), this, SLOT(drawLine(bool)));
	connect(mUi->drawEllipseButton, SIGNAL(clicked(bool)), this, SLOT(drawEllipse(bool)));
	connect(mUi->drawCurveButton, SIGNAL(clicked(bool)), this, SLOT(drawCurve(bool)));
	connect(mUi->drawRectButton, SIGNAL(clicked(bool)), this, SLOT(drawRectangle(bool)));
	connect(mUi->addTextButton, SIGNAL(clicked(bool)), this, SLOT(addText(bool)));
	connect(mUi->addDynamicTextButton, SIGNAL(clicked(bool)), this, SLOT(addDynamicText(bool)));
	connect(mUi->addTextPictureButton, SIGNAL(clicked(bool)), this, SLOT(addTextPicture(bool)));
	connect(mUi->addPointPortButton, SIGNAL(clicked(bool)), this, SLOT(addPointPort(bool)));
	connect(mUi->addLinePortButton, SIGNAL(clicked(bool)), this, SLOT(addLinePort(bool)));
	connect(mUi->stylusButton, SIGNAL(clicked(bool)), this, SLOT(addStylus(bool)));
	connect(mUi->addImageButton, SIGNAL(clicked(bool)), this, SLOT(addImage(bool)));

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
	connect(mUi->saveAsPictureButton, SIGNAL(clicked()), this, SLOT(savePicture()));
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
	mScene->addNone(true);
}

void ShapeEdit::setHighlightOneButton(QAbstractButton *oneButton)
{
	foreach (QAbstractButton *button, mButtonGroup) {
		if (button != oneButton) {
			button->setChecked(false);
		}
	}
}

void ShapeEdit::initButtonGroup()
{
	mButtonGroup.append(mUi->drawLineButton);
	mButtonGroup.append(mUi->drawEllipseButton);
	mButtonGroup.append(mUi->drawCurveButton);
	mButtonGroup.append(mUi->drawRectButton);
	mButtonGroup.append(mUi->addTextButton);
	mButtonGroup.append(mUi->addDynamicTextButton);
	mButtonGroup.append(mUi->addTextPictureButton);
	mButtonGroup.append(mUi->addPointPortButton);
	mButtonGroup.append(mUi->addLinePortButton);
	mButtonGroup.append(mUi->stylusButton);
	mButtonGroup.append(mUi->addImageButton);
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

graphicsUtils::AbstractView* ShapeEdit::getView()
{
	return mUi->graphicsView;
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

QList<QDomElement> ShapeEdit::generateGraphics()
{
	QDomElement picture = mDocument.createElement("picture");
	QDomElement label = mDocument.createElement("labels");
	QDomElement ports = mDocument.createElement("ports");

	QRect sceneBoundingRect = mScene->realItemsBoundingRect();
	mTopLeftPicture = sceneBoundingRect.topLeft();

	QList<QGraphicsItem *> list = mScene->items();
	foreach (QGraphicsItem *graphicsItem, list) {

		Item* item = dynamic_cast<Item*>(graphicsItem);
		if (item != nullptr) {
			QPair<QDomElement, Item::DomElementTypes> genItem = item->generateDom(mDocument, mTopLeftPicture);
			QDomElement domItem = genItem.first;
			Item::DomElementTypes domType = genItem.second;
			switch (domType) {
			case Item::pictureType:
				picture.appendChild(domItem);
				break;
			case Item::labelType:
				label.appendChild(domItem);
				break;
			case Item::portType:
				ports.appendChild(domItem);
				break;
			default:
				break;
			}
		}
	}

	picture.setAttribute("sizex", static_cast<int>(sceneBoundingRect.width() + 1));
	picture.setAttribute("sizey", static_cast<int>(sceneBoundingRect.height() + 1));

	QList<QDomElement> domList;
	domList.push_back(picture);
	domList.push_back(label);
	domList.push_back(ports);

	return domList;
}

void ShapeEdit::generateDom()
{
	QDomElement graphics = mDocument.createElement("graphics");
	mDocument.appendChild(graphics);

	QList<QDomElement> list = generateGraphics();
	foreach (QDomElement domItem, list) {
		graphics.appendChild(domItem);
	}
}

void ShapeEdit::exportToXml(const QString &fileName)
{
	OutFile file(fileName);
	generateDom();
	file() << "<?xml version='1.0' encoding='utf-8'?>\n";
	file() << mDocument.toString(4);
	file() << "\n";
}

void ShapeEdit::saveToXml()
{
	mDocument.clear();
	QString fileName = QRealFileDialog::getSaveFileName("SaveShapeEditorXml", this);
	if (fileName.isEmpty()) {
		return;
	}

	exportToXml(fileName);
}

void ShapeEdit::save()
{
	generateDom();
	if (mIndex.isValid()) {
		emit shapeSaved(mDocument.toString(4), mIndex, mRole);
	} else {
		mEditorManager->updateShape(mId, mDocument.toString(4));
		foreach (const Id graphicalElement, mGraphicalElements) {
			mEditorManager->updateShape(graphicalElement, mDocument.toString(4));
			for (QGraphicsItem * const item : mEditorView->editorViewScene().items()) {
				qReal::gui::editor::NodeElement * const element = dynamic_cast<qReal::gui::editor::NodeElement *>(item);
				if (element && element->id().type() == mId.type()) {
					element->updateShape(mDocument.toString(4));
				}
			}
		}

		mMainWindow->loadPlugins();
	}

	QMessageBox::information(this, tr("Saving"), "Saved successfully");
	mDocument.clear();
}

void ShapeEdit::savePicture()
{
	QString fileName = QRealFileDialog::getSaveFileName("SaveShapeEditorPicture", this);
	if (fileName.isEmpty()) {
		return;
	}

	QRectF sceneRect = mScene->itemsBoundingRect();
	QImage image(sceneRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::black));
	painter.drawRect(sceneRect);

	mScene->render(&painter);
	image.save(fileName);
}

void ShapeEdit::open()
{
	mDocument.clear();
	QString fileName = QRealFileDialog::getOpenFileName("OpenShapeEditorXml", this);
	if (fileName.isEmpty()) {
		return;
	}

	XmlLoader loader(mScene);
	loader.readFile(fileName);
}

void ShapeEdit::load(const QString &text)
{
	if (text.isEmpty()) {
		return;
	}

	XmlLoader loader(mScene);
	loader.readString(text);
}

void ShapeEdit::addImage(bool checked)
{
	if (checked) {
		setHighlightOneButton(mUi->addImageButton);
		QString fileName = QRealFileDialog::getOpenFileName("OpenShapeEditorImage", this);
		if (fileName.isEmpty()) {
			return;
		}

		mScene->addImage(fileName);
	}
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


void ShapeEdit::drawLine(bool checked)
{
	mScene->drawLine(checked);
	if (checked) {
		setHighlightOneButton(mUi->drawLineButton);
	}
}

void ShapeEdit::drawEllipse(bool checked)
{
	mScene->drawEllipse(checked);
	if (checked) {
		setHighlightOneButton(mUi->drawEllipseButton);
	}
}

void ShapeEdit::drawCurve(bool checked)
{
	mScene->drawCurve(checked);
	if (checked) {
		setHighlightOneButton(mUi->drawCurveButton);
	}
}

void ShapeEdit::drawRectangle(bool checked)
{
	mScene->drawRectangle(checked);
	if (checked) {
		setHighlightOneButton(mUi->drawRectButton);
	}
}

void ShapeEdit::addText(bool checked)
{
	mScene->addText(checked);
	if (checked) {
		setHighlightOneButton(mUi->addTextButton);
	}
}

void ShapeEdit::addDynamicText(bool checked)
{
	mScene->addDynamicText(checked);
	if (checked) {
		setHighlightOneButton(mUi->addDynamicTextButton);
	}
}

void ShapeEdit::addTextPicture(bool checked)
{
	mScene->addTextPicture(checked);
	if (checked) {
		setHighlightOneButton(mUi->addTextPictureButton);
	}
}

void ShapeEdit::addPointPort(bool checked)
{
	mScene->addPointPort(checked);
	if (checked) {
		setHighlightOneButton(mUi->addPointPortButton);
	}
}

void ShapeEdit::addLinePort(bool checked)
{
	mScene->addLinePort(checked);
	if (checked) {
		setHighlightOneButton(mUi->addLinePortButton);
	}
}

void ShapeEdit::addStylus(bool checked)
{
	mScene->addStylus(checked);
	if (checked) {
		setHighlightOneButton(mUi->stylusButton);
	}
}

void ShapeEdit::visibilityButtonClicked()
{
	QList<Item *> selectedItems = mScene->selectedSceneItems();
	if (selectedItems.isEmpty()) {
		return;
	}
	VisibilityConditionsDialog vcDialog(getProperties(), selectedItems);
	vcDialog.exec();
}

QMap<QString, VisibilityConditionsDialog::PropertyInfo> ShapeEdit::getProperties() const
{
	typedef VisibilityConditionsDialog::PropertyInfo PropertyInfo;

	QMap<QString, PropertyInfo> result;

	qrRepo::RepoApi *repoApi = dynamic_cast<qrRepo::RepoApi *>(&mModel->mutableApi());
	qReal::IdList enums = repoApi->elementsByType("MetaEntityEnum");

	foreach (const qReal::Id &child, repoApi->children(mModel->idByIndex(mIndex))) {
		if (child.element() != "MetaEntity_Attribute") {
			continue;
		}

		QString type = repoApi->stringProperty(child, "attributeType");
		if (type == "int") {
			result.insert(repoApi->name(child), PropertyInfo(VisibilityConditionsDialog::Int, QStringList()));
		} else if (type == "bool") {
			result.insert(repoApi->name(child), PropertyInfo(VisibilityConditionsDialog::Bool
					, QStringList() << "true" << "false"));
		} else if (type == "string") {
			result.insert(repoApi->name(child), PropertyInfo(VisibilityConditionsDialog::String, QStringList()));
		} else {
			foreach (const qReal::Id &enumElement, enums) {
				if (!repoApi->isLogicalElement(enumElement)) {
					continue;
				}

				if (repoApi->name(enumElement) == type) {
					QStringList enumValues;
					foreach (const qReal::Id &value, repoApi->children(enumElement)) {
						enumValues << repoApi->stringProperty(value, "valueName");
					}

					result.insert(repoApi->name(child), PropertyInfo(VisibilityConditionsDialog::Enum, enumValues));
				}
			}
		}
	}
	return result;
}

QStringList ShapeEdit::getPortTypes() const
{
	QStringList result;
	result << "NonTyped";

	if (mUseTypedPorts) {
		qrRepo::RepoApi *repoApi = dynamic_cast<qrRepo::RepoApi *>(&mModel->mutableApi());
		if (repoApi) {
			foreach (const qReal::Id &port, repoApi->elementsByType("MetaEntityPort")) {
				if (repoApi->isLogicalElement(port)) {
					result << repoApi->name(port);
				}
			}
		}
	}

	return result;
}
