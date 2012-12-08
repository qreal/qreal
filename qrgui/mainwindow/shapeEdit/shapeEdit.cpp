#include "shapeEdit.h"
#include "ui_shapeEdit.h"
#include "../../../qrutils/outFile.h"
#include "../../../qrutils/xmlUtils.h"
#include "xmlLoader.h"
#include "../../../qrutils/graphicsUtils/colorlisteditor.h"

#include <QtGui/QFileDialog>
#include <QtGui/QGraphicsItem>
#include <QtCore/QList>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QImage>
#include <QtGui/QMessageBox>

#include <QDebug>

using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent)
	: QWidget(parent), mUi(new Ui::ShapeEdit), mRole(0)

{
	init();
	connect(this, SIGNAL(saveSignal()), this, SLOT(saveToXml()));
}

ShapeEdit::ShapeEdit(const QPersistentModelIndex &index, const int &role)
	: QWidget(NULL), mUi(new Ui::ShapeEdit),mIndex(index), mRole(role)
{
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
	connect(mUi->brushStyleComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeBrushStyle(const QString &)));
	connect(mUi->brushColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeBrushColor(const QString &)));

	connect(mUi->textFamilyFontComboBox, SIGNAL(currentFontChanged(const QFont&)), mScene, SLOT(changeFontFamily(const QFont&)));
	connect(mUi->textPixelSizeSpinBox, SIGNAL(valueChanged(int)), mScene, SLOT(changeFontPixelSize(int)));
	connect(mUi->textColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeFontColor(const QString &)));
	connect(mUi->textEditField, SIGNAL(textChanged()), this, SLOT(changeTextName()));
	connect(mUi->italicCheckBox, SIGNAL(toggled(bool)), mScene, SLOT(changeFontItalic(bool)));
	connect(mUi->boldCheckBox, SIGNAL(toggled(bool)), mScene, SLOT(changeFontBold(bool)));
	connect(mUi->underlineCheckBox, SIGNAL(toggled(bool)), mScene, SLOT(changeFontUnderline(bool)));

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
	connect(mScene, SIGNAL(existSelectedItems(QPen const &, QBrush const &)), this, SLOT(setItemPalette(QPen const&, QBrush const&)));
	connect(mScene, SIGNAL(resetHighlightAllButtons()), this, SLOT(resetHighlightAllButtons()));
	connect(mScene, SIGNAL(noSelectedTextPictureItems()), this, SLOT(setNoFontPalette()));
	connect(mScene, SIGNAL(existSelectedTextPictureItems(QPen const &, QFont const &, QString const &)), this, SLOT(setItemFontPalette(QPen const&, QFont const&, QString const &)));
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
		if (button != oneButton)
			button->setChecked(false);
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
	if (event->matches(QKeySequence::Save))
		emit saveToXmlSignal();
	else if (event->key() == Qt::Key_F2)
		emit saveSignal();
	if (event->matches(QKeySequence::Open))
		emit openSignal();
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
		if (item != NULL) {
			QPair<QDomElement, Item::DomElementTypes> genItem = item->generateItem(mDocument, mTopLeftPicture);
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
	picture.setAttribute("sizex", static_cast<int>(sceneBoundingRect.width()));
	picture.setAttribute("sizey", static_cast<int>(sceneBoundingRect.height()));

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
	foreach (QDomElement domItem, list)
		graphics.appendChild(domItem);
}

void ShapeEdit::exportToXml(QString const &fileName)
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
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;
	exportToXml(fileName);
}

void ShapeEdit::save()
{
	generateDom();
	QMessageBox::information(this, tr("Saving"), "Saved successfully");
	emit shapeSaved(mDocument.toString(4), mIndex, mRole);
}

void ShapeEdit::savePicture()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;
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
	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty())
		return;
	XmlLoader loader(mScene);
	loader.readFile(fileName);
}

void ShapeEdit::load(const QString &text)
{
	if (text.isEmpty())
		return;
	XmlLoader loader(mScene);
	loader.readString(text);
}

void ShapeEdit::addImage(bool checked)
{
	if (checked) {
		setHighlightOneButton(mUi->addImageButton);
		QString fileName = QFileDialog::getOpenFileName(this);
		if (fileName.isEmpty())
			return;
		mScene->addImage(fileName);
	}
}

void ShapeEdit::setValuePenStyleComboBox(Qt::PenStyle penStyle)
{
	if (penStyle == Qt::SolidLine)
		mUi->penStyleComboBox->setCurrentIndex(0);
	else if (penStyle == Qt::DotLine)
		mUi->penStyleComboBox->setCurrentIndex(1);
	else if (penStyle == Qt::DashLine)
		mUi->penStyleComboBox->setCurrentIndex(2);
	else if (penStyle == Qt::DashDotLine)
		mUi->penStyleComboBox->setCurrentIndex(3);
	else if (penStyle == Qt::DashDotDotLine)
		mUi->penStyleComboBox->setCurrentIndex(4);
	else if (penStyle == Qt::NoPen)
		mUi->penStyleComboBox->setCurrentIndex(5);
}

void ShapeEdit::setValuePenColorComboBox(QColor penColor)
{
	mUi->penColorComboBox->setColor(penColor);
}

void ShapeEdit::setValueBrushStyleComboBox(Qt::BrushStyle brushStyle)
{
	if (brushStyle == Qt::SolidPattern)
		mUi->brushStyleComboBox->setCurrentIndex(1);
	else if (brushStyle == Qt::NoBrush)
		mUi->brushStyleComboBox->setCurrentIndex(0);
}

void ShapeEdit::setValuePenWidthSpinBox(int width)
{
	mUi->penWidthSpinBox->setValue(width);
}

void ShapeEdit::setValueBrushColorComboBox(QColor brushColor)
{
	mUi->brushColorComboBox->setColor(brushColor);
}

void ShapeEdit::setItemPalette(QPen const &penItem, QBrush const &brushItem)
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

void ShapeEdit::setValueTextFamilyFontComboBox(QFont const &fontItem)
{
	mUi->textFamilyFontComboBox->setCurrentFont(fontItem);
}

void ShapeEdit::setValueTextPixelSizeSpinBox(int size)
{
	mUi->textPixelSizeSpinBox->setValue(size);
}

void ShapeEdit::setValueTextColorComboBox(QColor penColor)
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

void ShapeEdit::setValueTextNameLineEdit(QString const& name)
{
	mUi->textEditField->setPlainText(name);
}

void ShapeEdit::setItemFontPalette(QPen const &penItem, QFont const &fontItem, QString const &name)
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

void ShapeEdit::changeTextName()
{
	QString newName = mUi->textEditField->toPlainText();
	mScene->changeTextName(newName);
}


void ShapeEdit::drawLine(bool checked)
{
	mScene->drawLine(checked);
	if (checked)
		setHighlightOneButton(mUi->drawLineButton);
}

void ShapeEdit::drawEllipse(bool checked)
{
	mScene->drawEllipse(checked);
	if (checked)
		setHighlightOneButton(mUi->drawEllipseButton);
}

void ShapeEdit::drawCurve(bool checked)
{
	mScene->drawCurve(checked);
	if (checked)
		setHighlightOneButton(mUi->drawCurveButton);
}

void ShapeEdit::drawRectangle(bool checked)
{
	mScene->drawRectangle(checked);
	if (checked)
		setHighlightOneButton(mUi->drawRectButton);
}

void ShapeEdit::addText(bool checked)
{
	mScene->addText(checked);
	if (checked)
		setHighlightOneButton(mUi->addTextButton);
}

void ShapeEdit::addDynamicText(bool checked)
{
	mScene->addDynamicText(checked);
	if (checked)
		setHighlightOneButton(mUi->addDynamicTextButton);
}

void ShapeEdit::addTextPicture(bool checked)
{
	mScene->addTextPicture(checked);
	if (checked)
		setHighlightOneButton(mUi->addTextPictureButton);
}

void ShapeEdit::addPointPort(bool checked)
{
	mScene->addPointPort(checked);
	if (checked)
		setHighlightOneButton(mUi->addPointPortButton);
}

void ShapeEdit::addLinePort(bool checked)
{
	mScene->addLinePort(checked);
	if (checked)
		setHighlightOneButton(mUi->addLinePortButton);
}

void ShapeEdit::addStylus(bool checked)
{
	mScene->addStylus(checked);
	if (checked)
		setHighlightOneButton(mUi->stylusButton);
}
