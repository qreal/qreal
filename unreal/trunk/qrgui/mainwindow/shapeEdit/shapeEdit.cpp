#include "shapeEdit.h"

#include "colorlisteditor.h"
#include "ui_shapeEdit.h"
#include "../../../utils/outFile.h"
#include "../../../utils/xmlUtils.h"
#include "XmlLoader.h"

#include <QtGui/QFileDialog>
#include <QtGui/QGraphicsItem>
#include <QtCore/QList>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent) :
		QWidget(parent),
		mUi(new Ui::ShapeEdit)
{
	mUi->setupUi(this);

	mScene = new Scene(mUi->graphicsView, this);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

	QStringList penStyleList = Item::getPenStyleList();
	mUi->penStyleComboBox->addItems(penStyleList);
	mUi->penWidthSpinBox->setRange(0, 13);
	mUi->penColorComboBox->setColor(QColor("black"));

	QStringList brushStyleList = Item::getBrushStyleList();
	mUi->brushStyleComboBox->addItems(brushStyleList);
	mUi->brushColorComboBox->setColor(QColor("black"));

	connect(mUi->drawLineButton, SIGNAL(pressed()), mScene, SLOT(drawLine()));
	connect(mUi->drawEllipseButton, SIGNAL(pressed()), mScene, SLOT(drawEllipse()));
	connect(mUi->drawArcButton, SIGNAL(pressed()), mScene, SLOT(drawArc()));
	connect(mUi->drawRectButton, SIGNAL(pressed()), mScene, SLOT(drawRectangle()));
	connect(mUi->addTextButton, SIGNAL(pressed()), mScene, SLOT(addText()));
	connect(mUi->addDynamicTextButton, SIGNAL(pressed()), mScene, SLOT(addDynamicText()));
	connect(mUi->addPointPortButton, SIGNAL(pressed()), mScene, SLOT(addPointPort()));
	connect(mUi->addLinePortButton, SIGNAL(pressed()), mScene, SLOT(addLinePort()));
	connect(mUi->stylusButton, SIGNAL(pressed()), mScene, SLOT(addStylus()));
	connect(mUi->penStyleComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePenStyle(const QString &)));
	connect(mUi->penWidthSpinBox, SIGNAL(valueChanged(int)), mScene, SLOT(changePenWidth(int)));
	connect(mUi->penColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changePenColor(const QString &)));
	connect(mUi->brushStyleComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeBrushStyle(const QString &)));
	connect(mUi->brushColorComboBox, SIGNAL(activated(const QString &)), mScene, SLOT(changeBrushColor(const QString &)));
	connect(mUi->deleteItemButton, SIGNAL(pressed()), mScene, SLOT(deleteItem()));
	connect(mUi->clearButton, SIGNAL(pressed()), mScene, SLOT(clearScene()));
	connect(mUi->saveToXmlButton, SIGNAL(clicked()), this, SLOT(saveToXml()));
	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(mUi->openButton, SIGNAL(clicked()), this, SLOT(open()));
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
	emit shapeSaved(mDocument.toString(4));
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
