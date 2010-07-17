#include "shapeEdit.h"
#include "colorlisteditor.h"
#include "ui_shapeEdit.h"
#include "../../../utils/outFile.h"
#include "../../../utils/xmlUtils.h"
#include "xmlLoader.h"

#include <QtGui/QFileDialog>
#include <QtGui/QGraphicsItem>
#include <QtCore/QList>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent)
	: QWidget(parent), mUi(new Ui::ShapeEdit), mRole(0)

{
	init();
}

ShapeEdit::ShapeEdit(const QPersistentModelIndex &index, const int &role)
	: QWidget(NULL), mUi(new Ui::ShapeEdit),mIndex(index), mRole(role)
{
	init();
	mUi->saveButton->setEnabled(true);
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
	mUi->penColorComboBox->setColor(QColor("black"));

	QStringList brushStyleList = Item::getBrushStyleList();
	mUi->brushStyleComboBox->addItems(brushStyleList);
	mUi->brushColorComboBox->setColor(QColor("white"));

	connect(mUi->drawLineButton, SIGNAL(clicked(bool)), mScene, SLOT(drawLine(bool)));
	connect(mUi->drawEllipseButton, SIGNAL(clicked(bool)), mScene, SLOT(drawEllipse(bool)));
	connect(mUi->drawCurveButton, SIGNAL(clicked(bool)), mScene, SLOT(drawCurve(bool)));
	connect(mUi->drawRectButton, SIGNAL(clicked(bool)), mScene, SLOT(drawRectangle(bool)));
	connect(mUi->addTextButton, SIGNAL(clicked(bool)), mScene, SLOT(addText(bool)));
	connect(mUi->addDynamicTextButton, SIGNAL(clicked(bool)), mScene, SLOT(addDynamicText(bool)));
	connect(mUi->addPointPortButton, SIGNAL(clicked(bool)), mScene, SLOT(addPointPort(bool)));
	connect(mUi->addLinePortButton, SIGNAL(clicked(bool)), mScene, SLOT(addLinePort(bool)));
	connect(mUi->stylusButton, SIGNAL(clicked(bool)), mScene, SLOT(addStylus(bool)));
	connect(mUi->noneButton, SIGNAL(clicked(bool)), mScene, SLOT(addNone(bool)));
	connect(mUi->penStyleComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePenStyle(const QString &)));
	connect(mUi->penWidthSpinBox, SIGNAL(valueChanged(int)), mScene, SLOT(changePenWidth(int)));
	connect(mUi->penColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePenColor(const QString &)));
	connect(mUi->brushStyleComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeBrushStyle(const QString &)));
	connect(mUi->brushColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeBrushColor(const QString &)));
	connect(mUi->deleteItemButton, SIGNAL(clicked()), mScene, SLOT(deleteItem()));
	connect(mUi->clearButton, SIGNAL(clicked()), mScene, SLOT(clearScene()));
	connect(mUi->saveToXmlButton, SIGNAL(clicked()), this, SLOT(saveToXml()));
	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(mUi->openButton, SIGNAL(clicked()), this, SLOT(open()));

	connect(mScene, SIGNAL(noSelectedItems()), this, SLOT(setNoPalette()));
	connect(mScene, SIGNAL(existSelectedItems(QPen const &, QBrush const &)), this, SLOT(setItemPalette(QPen const&, QBrush const&)));
}

void ShapeEdit::initPalette()
{
	mUi->penStyleComboBox->setCurrentIndex(0);
	mUi->penWidthSpinBox->setValue(0);
	mUi->penColorComboBox->setColor(QColor("black"));

	mUi->brushStyleComboBox->setCurrentIndex(0);
	mUi->brushColorComboBox->setColor(QColor("white"));
}

ShapeEdit::~ShapeEdit()
{
	delete mUi;
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

QList<QDomElement> ShapeEdit::generateGraphics()
{
	QDomElement picture = mDocument.createElement("picture");
	QDomElement label = mDocument.createElement("labels");
	QDomElement ports = mDocument.createElement("ports");

	mScene->removeItem(mScene->mEmptyRect);
	mTopLeftPicture = mScene->itemsBoundingRect().topLeft();

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
	picture.setAttribute("sizex", static_cast<int>(mScene->itemsBoundingRect().width()));
	picture.setAttribute("sizey", static_cast<int>(mScene->itemsBoundingRect().height()));

	QList<QDomElement> domList;
	domList.push_back(picture);
	domList.push_back(label);
	domList.push_back(ports);
	mScene->mEmptyRect = mScene->addRect(0, 0, sizeEmrtyRectX, sizeEmrtyRectY, QPen(Qt::white));

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
	emit shapeSaved(mDocument.toString(4), mIndex, mRole);
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

void ShapeEdit::setNoPalette()
{
	initPalette();
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
