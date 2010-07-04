#include "shapeEdit.h"
#include "ui_shapeEdit.h"
#include "../../../utils/outFile.h"

#include <QtGui/QFileDialog>
#include <QGraphicsItem>

using namespace utils;

ShapeEdit::ShapeEdit(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ShapeEdit)
{
	ui->setupUi(this);

	mScene = new Scene(this);
	ui->graphicsView->setScene(mScene);

	mMinPictureX = mScene->itemsBoundingRect().width();
	mMaxPictureX = mScene->itemsBoundingRect().x();
	mMinPictureY = mScene->itemsBoundingRect().height();
	mMaxPictureY = mScene->itemsBoundingRect().y();

	connect(ui->drawLineButton, SIGNAL(pressed()), mScene, SLOT(drawLine()));
	connect(ui->drawEllipseButton, SIGNAL(pressed()), mScene, SLOT(drawEllipse()));
	connect(ui->drawArcButton, SIGNAL(pressed()), mScene, SLOT(drawArc()));
	connect(ui->clearButton, SIGNAL(pressed()), mScene, SLOT(clearScene()));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveToXml()));
}

ShapeEdit::~ShapeEdit()
{
	delete ui;
}

void ShapeEdit::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void ShapeEdit::setXandY(QDomElement& dom, qreal x1, qreal y1, qreal x2, qreal y2)
{
	mMaxPictureX = qMax(qMax(x1, x2), mMaxPictureX);
	mMaxPictureY = qMax(qMax(y1, y2), mMaxPictureY);
	mMinPictureX = qMin(qMin(x1, x2), mMinPictureX);
	mMinPictureY = qMin(qMin(y1, y2), mMinPictureY);

	dom.setAttribute("y1", y1);
	dom.setAttribute("x1", x1);
	dom.setAttribute("y2", y2);
	dom.setAttribute("x2", x2);
}

QDomElement ShapeEdit::setPenBrush(QString const &domName, QPen pen)
{
	QDomElement dom = mDocument.createElement(domName);
	dom.setAttribute("fill", pen.brush().color().name());

	if (pen.brush().style() == Qt::NoBrush)
		dom.setAttribute("fill-style", "none");
	if (pen.brush().style() == Qt::SolidPattern)
		dom.setAttribute("fill-style", "solid");

	dom.setAttribute("stroke", pen.color().name());

	dom.setAttribute("stroke-width", pen.width());

	if (pen.style() == Qt::SolidLine)
		dom.setAttribute("stroke-style", "solid");
	if (pen.style() == Qt::DotLine)
		dom.setAttribute("stroke-style", "dot");
	if (pen.style() == Qt::DashLine)
		dom.setAttribute("stroke-style", "dash");
	if (pen.style() == Qt::DashDotLine)
		dom.setAttribute("stroke-style", "dashdot");
	if (pen.style() == Qt::DashDotDotLine)
		dom.setAttribute("stroke-style", "dashdotdot");
	if (pen.style() == Qt::NoPen)
		dom.setAttribute("stroke-style", "none");

	return dom;
}

QDomElement ShapeEdit::generateLine(QGraphicsLineItem* item)
{
	qreal x1 = item->line().x1();
	qreal y1 = item->line().y1();
	qreal x2 = item->line().x2();
	qreal y2 = item->line().y2();

	QDomElement line = setPenBrush("line", item->pen());
	setXandY(line, x1, y1, x2, y2);

	return line;
}

QDomElement ShapeEdit::generateEllipse(QGraphicsEllipseItem* item)
{
	qreal x1 = item->boundingRect().x();
	qreal y1 = item->boundingRect().y();
	qreal x2 = item->boundingRect().x() + item->boundingRect().width();
	qreal y2 = item->boundingRect().y() + item->boundingRect().height();

	QDomElement ellipse = setPenBrush("ellipse", item->pen());
	setXandY(ellipse, x1, y1, x2, y2);

	return ellipse;
}

QDomElement ShapeEdit::generateArch(Arch* item)
{
	qreal x1 = item->boundingRect().x();
	qreal y1 = item->boundingRect().y();
	qreal x2 = item->boundingRect().x() + item->boundingRect().width();
	qreal y2 = item->boundingRect().y() + item->boundingRect().height();

	QDomElement arch = mDocument.createElement("arch");
	arch.setAttribute("startAngle", item->startAngle());
	arch.setAttribute("spanAngle", item->spanAngle());
	setXandY(arch, x1, y1, x2, y2);

	return arch;
}

QDomElement ShapeEdit::generatePicture()
{
	QDomElement picture = mDocument.createElement("picture");

	QList<QGraphicsItem *> list = mScene->items();
	foreach (QGraphicsItem *graphicsItem, list) {

		QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(graphicsItem);
		QGraphicsEllipseItem* ellipseItem = dynamic_cast<QGraphicsEllipseItem*>(graphicsItem);
		Arch* archItem = dynamic_cast<Arch*>(graphicsItem);

		if (lineItem != NULL) {
			QDomElement line = generateLine(lineItem);
			picture.appendChild(line);
		}  else if (archItem != NULL) {
			QDomElement arch = generateArch(archItem);
			picture.appendChild(arch);
		} else if (ellipseItem != NULL && ellipseItem->children().isEmpty()) {
			QDomElement ellipse = generateEllipse(ellipseItem);
			picture.appendChild(ellipse);
		} else
			continue;
	}

	picture.setAttribute("sizex", mMaxPictureX - mMinPictureX);
	picture.setAttribute("sizey", mMaxPictureY - mMinPictureY);

	return picture;
}

void ShapeEdit::exportToXml(QString const &fileName)
{
	OutFile file(fileName);
	QDomElement graphics = mDocument.createElement("graphics");
	mDocument.appendChild(graphics);

	QDomElement picture = generatePicture();
	graphics.appendChild(picture);

	file() << "<?xml version='1.0' encoding='utf-8'?>\n";
	file() << mDocument.toString(4);
}

void ShapeEdit::saveToXml()
{
	mDocument.clear();
	mMinPictureX = mScene->itemsBoundingRect().x() + mScene->itemsBoundingRect().width();
	mMaxPictureX = mScene->itemsBoundingRect().x();
	mMinPictureY = mScene->itemsBoundingRect().y() + mScene->itemsBoundingRect().height();
	mMaxPictureY = mScene->itemsBoundingRect().y();

	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;
	exportToXml(fileName);
}
