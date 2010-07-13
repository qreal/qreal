#include "shapeEdit.h"

#include "ui_shapeEdit.h"
#include "../../../utils/outFile.h"

#include <QtGui/QFileDialog>
#include <QtGui/QGraphicsItem>
#include <QList>

using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent) :
		QWidget(parent),
		mUi(new Ui::ShapeEdit)
{
	mUi->setupUi(this);

	mScene = new Scene(this);
	mUi->graphicsView->setScene(mScene);

	connect(mUi->drawLineButton, SIGNAL(pressed()), mScene, SLOT(drawLine()));
	connect(mUi->drawEllipseButton, SIGNAL(pressed()), mScene, SLOT(drawEllipse()));
	connect(mUi->drawArcButton, SIGNAL(pressed()), mScene, SLOT(drawArc()));
	connect(mUi->drawRectButton, SIGNAL(pressed()), mScene, SLOT(drawRectangle()));
	connect(mUi->addTextButton, SIGNAL(pressed()), mScene, SLOT(addText()));
	connect(mUi->addDynamicTextButton, SIGNAL(pressed()), mScene, SLOT(addDynamicText()));
	connect(mUi->addPointPortButton, SIGNAL(pressed()), mScene, SLOT(addPointPort()));
	connect(mUi->addLinePortButton, SIGNAL(pressed()), mScene, SLOT(addLinePort()));
	connect(mUi->deleteItemButton, SIGNAL(pressed()), mScene, SLOT(deleteItem()));
	connect(mUi->clearButton, SIGNAL(pressed()), mScene, SLOT(clearScene()));
	connect(mUi->saveToXmlButton, SIGNAL(clicked()), this, SLOT(saveToXml()));
	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(save()));
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
